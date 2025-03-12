#ifndef FIFOWRITEMODE_H
#define FIFOWRITEMODE_H

#include "ReceiverMode.h"
#include "GPSReader.h"
#include "FIFOWriter.h"
#include "ParserWorker.h"
//#include "DummyFifoReaderThread.h"
#include <QThread>

class FifoWriteMode : public ReceiverMode {
public:
    FifoWriteMode(const ReceiverConfig &config, RawDataDialog &displayDialog);
    int start() override;
    void cleanup() override;
    ~FifoWriteMode();

private:
    ReceiverConfig m_config;
    RawDataDialog &m_displayDialog;
    GPSReader *m_reader;
    FIFOWriter *m_fifoWriter;
    ParserWorker *m_worker;
    QThread *m_workerThread;
    // DummyFifoReaderThread *m_dummyReaderThread;
};

#endif // FIFOWRITEMODE_H
