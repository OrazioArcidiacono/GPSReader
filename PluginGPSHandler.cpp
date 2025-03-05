#include "PluginGPSHandler.h"
#include <QDebug>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

PluginGPSHandler::PluginGPSHandler(const QString &fifoPath, QObject *parent)
    : IGPSHandler(parent), m_fifoPath(fifoPath)
{
    if (mkfifo(m_fifoPath.toLocal8Bit().constData(), 0666) < 0) {
        qDebug() << Q_FUNC_INFO << "mkfifo (se già esiste, ignorare):" << m_fifoPath;
    }
}

void PluginGPSHandler::handleCoordinate(const Coordinate &coord) {
    QString data = QString("Lat: %1, Lon: %2\n")
                       .arg(coord.latitude, 0, 'f', 6)
                       .arg(coord.longitude, 0, 'f', 6);
    qDebug() << Q_FUNC_INFO << "Plugin handler:" << data.trimmed();
}
