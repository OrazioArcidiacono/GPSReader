#ifndef PLUGINGPSHANDLER_H
#define PLUGINGPSHANDLER_H

#include "IGPSHandler.h"

class PluginGPSHandler : public IGPSHandler {
    Q_OBJECT
public:
    explicit PluginGPSHandler(const QString &fifoPath, QObject *parent = nullptr);
    void handleCoordinate(const Coordinate &coord) override;

private:
    QString m_fifoPath;
};

#endif // PLUGINGPSHANDLER_H
