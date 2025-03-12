#include "FifoWriteMode.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>

FifoWriteMode::FifoWriteMode(const ReceiverConfig &config, RawDataDialog &displayDialog)
    : m_config(config), m_displayDialog(displayDialog),
    m_reader(new GPSReader(config.port, config.baudRate)),
    m_fifoWriter(new FIFOWriter("/tmp/gps_fifo")),
    m_worker(nullptr), m_workerThread(new QThread)
   // m_dummyReaderThread(nullptr)  // Se vuoi disabilitare il dummy reader, imposta a nullptr.
{
    m_displayDialog.updateStatus("Mode: FIFO WRITE");

    // Connetti il GPSReader per aggiornare la GUI.
    QObject::connect(m_reader, &GPSReader::rawDataReceived,
                     [this](const QString &rawData, int blockId) {
                         QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
                         QString sentenceType;
                         if (rawData.startsWith("$")) {
                             int commaIndex = rawData.indexOf(",");
                             if (commaIndex > 1)
                                 sentenceType = rawData.mid(1, commaIndex - 1);
                         }
                         m_displayDialog.addRawLogEntry(blockId, timestamp, rawData, sentenceType);
                     });
    QObject::connect(m_reader, &GPSReader::statusUpdated,
                     &m_displayDialog, &RawDataDialog::updateStatus);
    QObject::connect(&m_displayDialog, &RawDataDialog::pauseRequested,
                     m_reader, &GPSReader::pauseReception);
    QObject::connect(&m_displayDialog, &RawDataDialog::resumeRequested,
                     m_reader, &GPSReader::resumeReception);
    QObject::connect(&m_displayDialog, &RawDataDialog::restartRequested,
                     m_reader, &GPSReader::restartPort);
}

int FifoWriteMode::start() {
    // Crea e avvia il ParserWorker.
    m_worker = new ParserWorker(m_reader->sentenceQueue());
    m_worker->moveToThread(m_workerThread);
    QObject::connect(m_workerThread, &QThread::started, m_worker, &ParserWorker::process);
    QObject::connect(m_worker, &ParserWorker::parsedDataAvailable,
                     &m_displayDialog, &RawDataDialog::updateParsedData);
    QObject::connect(m_worker, &ParserWorker::newCoordinate,
                     &m_displayDialog, &RawDataDialog::updateProcessedData);
    // Collega il writer per scrivere sulla FIFO.
    QObject::connect(m_worker, &ParserWorker::newCoordinate,
                     m_fifoWriter, &FIFOWriter::writeCoordinate);
    QObject::connect(m_fifoWriter, &FIFOWriter::dataWritten,
                     &m_displayDialog, &RawDataDialog::updateFifoOutput);

    // Se si desidera usare il dummy reader, altrimenti lascia m_dummyReaderThread a nullptr.
    // m_dummyReaderThread = new DummyFifoReaderThread("/tmp/gps_fifo");
    // m_dummyReaderThread->start();

    m_workerThread->start();
    m_reader->start();

    return QApplication::exec();
}

void FifoWriteMode::cleanup() {
    if (m_worker) m_worker->stop();
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
    delete m_worker;
    delete m_workerThread;
    delete m_reader;
    delete m_fifoWriter;
//    if(m_dummyReaderThread) {
//        m_dummyReaderThread->quit();
//        m_dummyReaderThread->wait();
//        delete m_dummyReaderThread;
//    }
}

FifoWriteMode::~FifoWriteMode() {
    // cleanup già effettuato
}
