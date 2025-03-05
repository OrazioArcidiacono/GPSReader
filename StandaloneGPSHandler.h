#ifndef STANDALONEGPSHANDLER_H
#define STANDALONEGPSHANDLER_H

#include "IGPSHandler.h"

class StandaloneGPSHandler : public IGPSHandler {
    Q_OBJECT
public:
    explicit StandaloneGPSHandler(QObject *parent = nullptr);
    void handleCoordinate(const Coordinate &coord) override;
};

#endif // STANDALONEGPSHANDLER_H
