#include "StandaloneMode.h"
#include <QDateTime>
#include <QDebug>
#include <QApplication>

StandaloneMode::StandaloneMode(const ReceiverConfig &config, RawDataDialog &displayDialog)
    : m_config(config), m_displayDialog(displayDialog),
    m_reader(new GPSReader(config.port, config.baudRate)),
    m_worker(nullptr), m_workerThread(new QThread)
{
    m_displayDialog.updateStatus("Mode: STANDALONE");

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

int StandaloneMode::start() {
    // Crea e avvia il ParserWorker in un thread separato.
    m_worker = new ParserWorker(m_reader->sentenceQueue());
    m_worker->moveToThread(m_workerThread);
    QObject::connect(m_workerThread, &QThread::started, m_worker, &ParserWorker::process);
    QObject::connect(m_worker, &ParserWorker::parsedDataAvailable,
                     &m_displayDialog, &RawDataDialog::updateParsedData);
    QObject::connect(m_worker, &ParserWorker::newCoordinate,
                     &m_displayDialog, &RawDataDialog::updateProcessedData);

    m_workerThread->start();
    m_reader->start();

    return QApplication::exec();
}

void StandaloneMode::cleanup() {
    if(m_worker) {
        m_worker->stop();
    }
    if(m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
    }
    delete m_worker;
    delete m_workerThread;
    delete m_reader;
}

StandaloneMode::~StandaloneMode() {
    // Cleanup è già chiamato nel main (o da un manager).
}
