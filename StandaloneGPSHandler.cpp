#include "StandaloneGPSHandler.h"
#include <QDebug>

StandaloneGPSHandler::StandaloneGPSHandler(QObject *parent)
    : IGPSHandler(parent)
{}

void StandaloneGPSHandler::handleCoordinate(const Coordinate &coord) {
    QString message = QString("Lat: %1, Lon: %2")
                          .arg(coord.latitude, 0, 'f', 6)
                          .arg(coord.longitude, 0, 'f', 6);
    qDebug() << Q_FUNC_INFO << "Standalone handler:" << message;
}
