#include "FifoReadMode.h"
#include <QApplication>
#include <QDebug>

FifoReadMode::FifoReadMode(const QString &fifoPath, RawDataDialog &displayDialog)
    : m_displayDialog(displayDialog)
{
    m_displayDialog.updateStatus("Mode: FIFO READ");
    m_fifoSource = new FIFODataSource(fifoPath);
    QObject::connect(m_fifoSource, &FIFODataSource::newCoordinate,
                     &m_displayDialog, &RawDataDialog::updateProcessedData);
}

int FifoReadMode::start() {
    m_fifoSource->start();
    return QApplication::exec();
}

void FifoReadMode::cleanup() {
    delete m_fifoSource;
}

FifoReadMode::~FifoReadMode() {
}
