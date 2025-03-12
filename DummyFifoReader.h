#ifndef DUMMYFIFOREADER_H
#define DUMMYFIFOREADER_H

#include <QObject>
#include <QString>

class DummyFifoReader : public QObject {
    Q_OBJECT
public:
    explicit DummyFifoReader(const QString &fifoPath, QObject *parent = nullptr)
        : QObject(parent), m_fifoPath(fifoPath), m_running(false), m_fd(-1) {}
    ~DummyFifoReader() { stop(); }

    void start();
    void stop();

public slots:
    void process();

private:
    QString m_fifoPath;
    bool m_running;
    int m_fd;
};

#endif // DUMMYFIFOREADER_H
