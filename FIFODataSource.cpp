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

// readLoop(): continuously reads from the FIFO while m_running is true.
void FIFODataSource::readLoop() {
    while (m_running) {
        // Open the FIFO for reading.
        int fd = open(m_fifoPath.toLocal8Bit().constData(), O_RDONLY);
        if (fd == -1) {
            qWarning() << Q_FUNC_INFO << "Error opening FIFO for reading:" << m_fifoPath;
            QThread::sleep(1);
            continue;
        }

        // Read up to 255 bytes.
        char buffer[256] = {0};
        ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            QString data(buffer);
            qDebug() << Q_FUNC_INFO << "Data received from FIFO:" << data;
            // Expected format: "Type: <messageType>, Lat: <latitude>, Lon: <longitude>"
            QStringList parts = data.split(",");
            if (parts.size() >= 3) {
                // Extract message type from the first part.
                QString messageType = parts[0].split(":").value(1).trimmed();
                // Extract latitude from the second part.
                QString latStr = parts[1].split(":").value(1).trimmed();
                // Extract longitude from the third part.
                QString lonStr = parts[2].split(":").value(1).trimmed();
                bool ok1, ok2;
                double lat = latStr.toDouble(&ok1);
                double lon = lonStr.toDouble(&ok2);
                if (ok1 && ok2) {
                    Coordinate coord { lat, lon };
                    // Emit the newCoordinate signal with both the coordinate and the message type.
                    emit newCoordinate(coord, messageType);
                } else {
                    qWarning() << Q_FUNC_INFO << "Error parsing coordinates:" << latStr << lonStr;
                }
            } else {
                qWarning() << Q_FUNC_INFO << "Unrecognized FIFO data format:" << data;
            }
        }
        close(fd);
        QThread::msleep(100);
    }
}
