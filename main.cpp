#include <signal.h>
#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include "GPSReceiverConfig.h"
#include "RawDataDialog.h"
#include "GPSReader.h"
#include "FIFOWriter.h"
#include "FIFODataSource.h"
#include "SocketChannel.h"
#include "ParserWorker.h"
#include "DummyFifoReaderThread.h"
#include <QThread>
#include <QDebug>

int main(int argc, char *argv[]) {
    // Ignora SIGPIPE per evitare crash se non c'è un lettore sulla FIFO.
    signal(SIGPIPE, SIG_IGN);

    QApplication app(argc, argv);

    // Setup command-line parser.
    QCommandLineParser parser;
    parser.setApplicationDescription("GPS Receiver Application with Detailed NMEA Explorer");
    parser.addHelpOption();
    QCommandLineOption channelOption(QStringList() << "channel",
                                     "Specifica il canale di comunicazione (standalone, fifo_read, fifo_write, socket).",
                                     "channel");
    parser.addOption(channelOption);
    parser.process(app);

    QString channelParam = parser.value(channelOption).toLower();

    ReceiverConfig config = GPSReceiverConfig::loadConfig("/Users/orazioarcidiacono/GPS_Receiver/", "gpsconfig.db");
    qDebug() << "Configurazione DB:" << config.mode << config.pluginAction << config.port << config.baudRate;

    // Override configuration if command-line parameter is provided.
    if (!channelParam.isEmpty()) {
        if (channelParam == "standalone")
            config.mode = "standalone";
        else {
            config.mode = "plugin";
            config.pluginAction = channelParam;
        }
        qDebug() << "Parametro da linea di comando --channel impostato su:" << channelParam;
    }

    RawDataDialog displayDialog;

    // Modalità standalone o plugin fifo_write usano lo stesso modello Producer–Consumer.
    if (config.mode.toLower() == "standalone" ||
        (config.mode.toLower() == "plugin" && config.pluginAction.toLower() == "fifo_write"))
    {
        if (config.mode.toLower() == "plugin")
            displayDialog.updateStatus("Mode: FIFO WRITE");
        else
            displayDialog.updateStatus("Mode: STANDALONE");

        // Crea il GPSReader (Producer).
        GPSReader *reader = new GPSReader(config.port, config.baudRate);

        // Collegamenti per aggiornare la GUI con i dati raw.
        QObject::connect(reader, &GPSReader::rawDataReceived,
                         [&displayDialog](const QString &rawData, int blockId) {
                             QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                             QString sentenceType;
                             if (rawData.startsWith("$")) {
                                 int commaIndex = rawData.indexOf(",");
                                 if (commaIndex > 1)
                                     sentenceType = rawData.mid(1, commaIndex - 1);
                             }
                             displayDialog.addRawLogEntry(blockId, timestamp, rawData, sentenceType);
                         });
        QObject::connect(reader, &GPSReader::statusUpdated,
                         &displayDialog, &RawDataDialog::updateStatus);
        QObject::connect(&displayDialog, &RawDataDialog::pauseRequested,
                         reader, &GPSReader::pauseReception);
        QObject::connect(&displayDialog, &RawDataDialog::resumeRequested,
                         reader, &GPSReader::resumeReception);
        QObject::connect(&displayDialog, &RawDataDialog::restartRequested,
                         reader, &GPSReader::restartPort);

        // Crea e avvia il ParserWorker (Consumer) in un thread separato.
        QThread *workerThread = new QThread;
        ParserWorker *worker = new ParserWorker(reader->sentenceQueue());
        worker->moveToThread(workerThread);
        QObject::connect(workerThread, &QThread::started, worker, &ParserWorker::process);
        QObject::connect(worker, &ParserWorker::parsedDataAvailable,
                         &displayDialog, &RawDataDialog::updateParsedData);
        QObject::connect(worker, &ParserWorker::newCoordinate,
                         &displayDialog, &RawDataDialog::updateProcessedData);

        // Se siamo in modalità FIFO WRITE, collega anche il FIFOWriter e avvia un DummyFifoReader.
        FIFOWriter *fifoWriter = nullptr;
        DummyFifoReaderThread *dummyReaderThread = nullptr;
        if (config.mode.toLower() == "plugin" && config.pluginAction.toLower() == "fifo_write") {
            fifoWriter = new FIFOWriter("/tmp/gps_fifo");
            QObject::connect(worker, &ParserWorker::newCoordinate,
                             fifoWriter, &FIFOWriter::writeCoordinate);
            QObject::connect(fifoWriter, &FIFOWriter::dataWritten,
                             &displayDialog, &RawDataDialog::updateFifoOutput);

            // Avvia il dummy reader per mantenere aperta la FIFO.
            dummyReaderThread = new DummyFifoReaderThread("/tmp/gps_fifo");
            dummyReaderThread->start();
        }

        workerThread->start();
        reader->start();

        int ret = app.exec();

        // Cleanup.
        worker->stop();
        workerThread->quit();
        workerThread->wait();
        if (dummyReaderThread) {
            dummyReaderThread->quit();
            dummyReaderThread->wait();
            delete dummyReaderThread;
        }
        delete worker;
        delete workerThread;
        delete reader;
        if (fifoWriter)
            delete fifoWriter;
        return ret;
    }
    // Modalità plugin: fifo_read.
    else if (config.mode.toLower() == "plugin" && config.pluginAction.toLower() == "fifo_read") {
        displayDialog.updateStatus("Mode: FIFO READ");
        FIFODataSource *fifoSource = new FIFODataSource("/tmp/gps_fifo");
        QObject::connect(fifoSource, &FIFODataSource::newCoordinate,
                         &displayDialog, &RawDataDialog::updateProcessedData);
        fifoSource->start();
        int ret = app.exec();
        delete fifoSource;
        return ret;
    }
    // Modalità plugin: socket.
    else if (config.mode.toLower() == "plugin" && config.pluginAction.toLower() == "socket") {
        displayDialog.updateStatus("Mode: SOCKET");
        SocketChannel socketChannel("127.0.0.1", 12345);
        if (socketChannel.connectChannel()) {
            QString msg = socketChannel.receiveMessage();
            qDebug() << "Messaggio ricevuto dal SocketChannel:" << msg;
        }
        return app.exec();
    }
    else {
        qWarning() << "Modalità non riconosciuta:" << config.mode;
                                                      return 1;
    }
}
