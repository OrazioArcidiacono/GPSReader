#include "DummyFifoReader.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <QThread>
#include <QDebug>
#include <errno.h>
#include <cstring>

void DummyFifoReader::start() {
    m_running = true;
    // Apri la FIFO in modalità lettura bloccante
    m_fd = open(m_fifoPath.toLocal8Bit().constData(), O_RDONLY);
    if (m_fd == -1) {
        qWarning() << Q_FUNC_INFO << "Error opening FIFO for dummy reading:"
                   << m_fifoPath << ":" << strerror(errno);
    }
}

void DummyFifoReader::stop() {
    m_running = false;
    if (m_fd != -1) {
        close(m_fd);
        m_fd = -1;
    }
}

void DummyFifoReader::process() {
    char buffer[256];
    while (m_running) {
        if (m_fd == -1) {
            // Se l'fd è invalido, riprova ad aprire la FIFO
            m_fd = open(m_fifoPath.toLocal8Bit().constData(), O_RDONLY);
            if (m_fd == -1) {
                qWarning() << Q_FUNC_INFO << "Error re-opening FIFO:"
                           << m_fifoPath << ":" << strerror(errno);
                QThread::msleep(1000);
                continue;
            }
        }
        ssize_t n = read(m_fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            qDebug() << "DummyFifoReader read:" << buffer;
        } else if (n == 0) {
            // EOF: chiudi e riapri la FIFO
            qDebug() << Q_FUNC_INFO << "EOF reached, re-opening FIFO.";
            close(m_fd);
            m_fd = -1;
            QThread::msleep(100);
        } else { // n < 0
            qWarning() << Q_FUNC_INFO << "Error reading FIFO:" << strerror(errno);
            close(m_fd);
            m_fd = -1;
            QThread::msleep(1000);
        }
        QThread::msleep(100);
    }
}
