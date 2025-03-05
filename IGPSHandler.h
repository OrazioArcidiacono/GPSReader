#ifndef IGPSHANDLER_H
#define IGPSHANDLER_H

#include <QObject>
#include "GPSReader.h" // Per la definizione di Coordinate

class IGPSHandler : public QObject {
    Q_OBJECT
public:
    explicit IGPSHandler(QObject *parent = nullptr) : QObject(parent) {}
    virtual ~IGPSHandler() = default;
    virtual void handleCoordinate(const Coordinate &coord) = 0;
};

#endif // IGPSHANDLER_H
