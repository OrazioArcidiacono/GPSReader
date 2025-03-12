#ifndef FIFOREADMODE_H
#define FIFOREADMODE_H

#include "ReceiverMode.h"
#include "FIFODataSource.h"

class FifoReadMode : public ReceiverMode {
public:
    FifoReadMode(const QString &fifoPath, RawDataDialog &displayDialog);
    int start() override;
    void cleanup() override;
    ~FifoReadMode();

private:
    FIFODataSource *m_fifoSource;
    RawDataDialog &m_displayDialog;
};

#endif // FIFOREADMODE_H
