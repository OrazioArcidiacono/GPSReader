#include "FIFODataSource.h"
#include <QDebug>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <QStringList>
#include <QThread>
#include <QtConcurrent>

FIFODataSource::FIFODataSource(const QString &fifoPath, QObject *parent)
    : QObject(parent), m_fifoPath(fifoPath), m_running(false)
{
    if (mkfifo(m_fifoPath.toLocal8Bit().constData(), 0666) < 0) {
        qDebug() << Q_FUNC_INFO << "mkfifo (se già esiste, ignorare):" << m_fifoPath;
    }
}

FIFODataSource::~FIFODataSource() {
    stop();
}

void FIFODataSource::start() {
    m_running = true;
   // QtConcurrent::run([this]() { readLoop(); });
    QThreadPool::globalInstance()->start([this]() { readLoop(); });

}

void FIFODataSource::stop() {
    m_running = false;
}

void FIFODataSource::readLoop() {
    while (m_running) {
        int fd = open(m_fifoPath.toLocal8Bit().constData(), O_RDONLY);
        if (fd == -1) {
            qWarning() << Q_FUNC_INFO << "Errore nell'aprire la FIFO in lettura:" << m_fifoPath;
            QThread::sleep(1);
            continue;
        }
        char buffer[256] = {0};
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            QString data(buffer);
            qDebug() << Q_FUNC_INFO << "Dati ricevuti dalla FIFO:" << data;
            QStringList parts = data.split(",");
            if (parts.size() >= 2) {
                QString latStr = parts[0].split(":").value(1).trimmed();
                QString lonStr = parts[1].split(":").value(1).trimmed();
                bool ok1, ok2;
                double lat = latStr.toDouble(&ok1);
                double lon = lonStr.toDouble(&ok2);
                if (ok1 && ok2) {
                    Coordinate coord { lat, lon };
                    emit newCoordinate(coord);
                } else {
                    qWarning() << Q_FUNC_INFO << "Errore nel parsing delle coordinate:" << latStr << lonStr;
                }
            } else {
                qWarning() << Q_FUNC_INFO << "Formato dati FIFO non riconosciuto:" << data;
            }
        }
        close(fd);
        QThread::msleep(100);
    }
}
