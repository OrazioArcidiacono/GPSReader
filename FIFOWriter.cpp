#include "FIFOWriter.h"
#include <QDebug>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

FIFOWriter::FIFOWriter(const QString &fifoPath, QObject *parent)
    : QObject(parent), m_fifoPath(fifoPath)
{
    if (mkfifo(m_fifoPath.toLocal8Bit().constData(), 0666) < 0) {
        qDebug() << Q_FUNC_INFO << "mkfifo (se già esiste, ignorare):" << m_fifoPath;
    }
}

FIFOWriter::~FIFOWriter() {
    // Non eliminiamo la FIFO per non interrompere altri processi
}

void FIFOWriter::writeCoordinate(const Coordinate &coord)
{
    QString data = QString("Latitudine: %1, Longitudine: %2\n")
                       .arg(coord.latitude, 0, 'f', 6)
                       .arg(coord.longitude, 0, 'f', 6);
    int fd = open(m_fifoPath.toLocal8Bit().constData(), O_WRONLY);
    if (fd == -1) {
        qWarning() << Q_FUNC_INFO << "Errore nell'aprire la FIFO in scrittura:" << m_fifoPath;
        return;
    }
    QByteArray bytes = data.toUtf8();
    if (write(fd, bytes.constData(), bytes.size()) == -1) {
        qWarning() << Q_FUNC_INFO << "Errore nella scrittura sulla FIFO:" << m_fifoPath;
    } else {
        qDebug() << Q_FUNC_INFO << "Dati scritti sulla FIFO:" << data.trimmed();
    }
    close(fd);
}
