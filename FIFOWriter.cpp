#include "FIFOWriter.h"
#include <QDebug>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

FIFOWriter::FIFOWriter(const QString &fifoPath, QObject *parent)
    : QObject(parent), m_fifoPath(fifoPath)
{
    // Attempt to create the FIFO with read/write permissions (0666).
    // Se la FIFO esiste già, verrà stampato un messaggio di debug.
    if (mkfifo(m_fifoPath.toLocal8Bit().constData(), 0666) < 0) {
        qDebug() << Q_FUNC_INFO << "mkfifo (if already exists, ignore):" << m_fifoPath;
    }
}

FIFOWriter::~FIFOWriter() {
    // Non rimuovere la FIFO per non interrompere altri processi
}

void FIFOWriter::writeCoordinate(const Coordinate &coord, const QString &messageType) {
    QString data = QString("Type: %1, Lat: %2, Lon: %3\n")
                       .arg(messageType)
                       .arg(coord.latitude, 0, 'f', 6)
                       .arg(coord.longitude, 0, 'f', 6);

    // Apri la FIFO in modalità non-bloccante
    int fd = open(m_fifoPath.toLocal8Bit().constData(), O_WRONLY | O_NONBLOCK);
    if (fd == -1) {
        if (errno == ENXIO) {
            qWarning() << Q_FUNC_INFO << "No reading process on the FIFO:" << m_fifoPath;
            // Puoi decidere di ritentare dopo un certo intervallo o semplicemente ignorare la scrittura.
        } else {
            qWarning() << Q_FUNC_INFO << "Error opening FIFO for writing:" << strerror(errno);
        }
        return;
    }

    QByteArray bytes = data.toUtf8();
    ssize_t written = write(fd, bytes.constData(), bytes.size());
    if (written == -1) {
        if (errno == EPIPE) {
            qWarning() << Q_FUNC_INFO << "Broken pipe (EPIPE) when writing to FIFO:" << m_fifoPath;
        } else {
            qWarning() << Q_FUNC_INFO << "Error writing to FIFO:" << strerror(errno);
        }
    } else {
        qDebug() << Q_FUNC_INFO << "Data written to FIFO:" << data.trimmed();
        emit dataWritten(data.trimmed());
    }
    close(fd);
}
