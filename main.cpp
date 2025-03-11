#include <QApplication>
#include <QCommandLineParser>
#include <QDateTime>
#include "GPSReceiverConfig.h"
#include "RawDataDialog.h"
#include "GPSReader.h"
#include "FIFODataSource.h"
#include "FIFOWriter.h"
#include "SocketChannel.h"
#include "ParserWorker.h"
#include <QThread>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("GPS Receiver Application with Detailed NMEA Explorer");
    parser.addHelpOption();

    QCommandLineOption channelOption(QStringList() << "channel",
                                     "Specifica il canale di comunicazione (fifo_read, fifo_write, socket).",
                                     "channel");
    parser.addOption(channelOption);
    parser.process(app);

    QString channelParam = parser.value(channelOption).toLower();
    if (!channelParam.isEmpty() &&
        channelParam != "fifo_read" &&
        channelParam != "fifo_write" &&
        channelParam != "socket")
    {
        qWarning() << "Parametro --channel non riconosciuto. Usa 'fifo_read', 'fifo_write' o 'socket'.";
        return 1;
    }

    ReceiverConfig config = GPSReceiverConfig::loadConfig("/Users/orazioarcidiacono/GPS_Receiver/", "gpsconfig.db");
    qDebug() << "Configurazione DB:" << config.mode << config.pluginAction << config.port << config.baudRate;

    if (!channelParam.isEmpty()) {
        config.mode = "plugin";
        config.pluginAction = channelParam;
        qDebug() << "Parametro da linea di comando --channel impostato su:" << channelParam;
    }

    RawDataDialog displayDialog;

    if (config.mode.toLower() == "standalone") {
        // Create the GPSReader (Producer)
        GPSReader *reader = new GPSReader(config.port, config.baudRate);

        // Connect rawDataReceived to update the raw data log in the GUI.
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

        // Connect GUI control signals to GPSReader.
        QObject::connect(&displayDialog, &RawDataDialog::pauseRequested,
                         reader, &GPSReader::pauseReception);
        QObject::connect(&displayDialog, &RawDataDialog::resumeRequested,
                         reader, &GPSReader::resumeReception);
        QObject::connect(&displayDialog, &RawDataDialog::restartRequested,
                         reader, &GPSReader::restartPort);

        // Create and start the ParserWorker (Consumer) in a separate thread.
        QThread *workerThread = new QThread;
        ParserWorker *worker = new ParserWorker(reader->sentenceQueue());
        worker->moveToThread(workerThread);
        QObject::connect(workerThread, &QThread::started, worker, &ParserWorker::process);
        QObject::connect(worker, &ParserWorker::parsedDataAvailable,
                         &displayDialog, &RawDataDialog::updateParsedData);
        // Connect newCoordinate signal with two parameters to the GUI slot.
        QObject::connect(worker, &ParserWorker::newCoordinate,
                         &displayDialog, &RawDataDialog::updateProcessedData);

        workerThread->start();
        reader->start();

        int ret = app.exec();

        // Cleanup before exit.
        worker->stop();
        workerThread->quit();
        workerThread->wait();
        delete worker;
        delete workerThread;
        delete reader;
        return ret;
    }
    else if (config.mode.toLower() == "plugin") {
        if (config.pluginAction.toLower() == "fifo_write") {
            GPSReader *reader = new GPSReader(config.port, config.baudRate);
            FIFOWriter *fifoWriter = new FIFOWriter("/tmp/gps_fifo");
            QObject::connect(reader, &GPSReader::newCoordinate,
                             fifoWriter, &FIFOWriter::writeCoordinate);
            reader->start();
        }
        else if (config.pluginAction.toLower() == "fifo_read") {
            FIFODataSource *fifoSource = new FIFODataSource("/tmp/gps_fifo");
            QObject::connect(fifoSource, &FIFODataSource::newCoordinate,
                             &displayDialog, &RawDataDialog::updateProcessedData);
            fifoSource->start();
        }
        else if (config.pluginAction.toLower() == "socket") {
            SocketChannel socketChannel("127.0.0.1", 12345);
            if (socketChannel.connectChannel()) {
                QString msg = socketChannel.receiveMessage();
                qDebug() << "Messaggio ricevuto dal SocketChannel:" << msg;
                // Implementa il ciclo di lettura
            }
        }
        else {
            qWarning() << "PluginAction non riconosciuto:" << config.pluginAction;
            return 1;
        }
    }
    else {
        qWarning() << "Modalità non riconosciuta:" << config.mode;
                                                      return 1;
    }

    return app.exec();
}
