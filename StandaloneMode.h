#ifndef STANDALONEMODE_H
#define STANDALONEMODE_H

#include "ReceiverMode.h"
#include "GPSReader.h"
#include "ParserWorker.h"
#include <QThread>

class StandaloneMode : public ReceiverMode {
public:
    StandaloneMode(const ReceiverConfig &config, RawDataDialog &displayDialog);
    int start() override;
    void cleanup() override;
    ~StandaloneMode();

private:
    ReceiverConfig m_config;
    RawDataDialog &m_displayDialog;
    GPSReader *m_reader;
    ParserWorker *m_worker;
    QThread *m_workerThread;
};

#endif // STANDALONEMODE_H
