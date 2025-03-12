#ifndef DUMMYFIFOREADERTHREAD_H
#define DUMMYFIFOREADERTHREAD_H

#include <QThread>
#include "DummyFifoReader.h"

class DummyFifoReaderThread : public QThread {
public:
    explicit DummyFifoReaderThread(const QString &fifoPath, QObject *parent = nullptr)
        : QThread(parent), m_reader(fifoPath) {}
    void run() override {
        m_reader.start();
        // Avvia il loop di process() per mantenere la FIFO aperta.
        m_reader.process();
    }
    void stop() { m_reader.stop(); }
private:
    DummyFifoReader m_reader;
};

#endif // DUMMYFIFOREADERTHREAD_H
