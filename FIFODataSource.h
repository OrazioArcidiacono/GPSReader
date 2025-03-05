#ifndef FIFODATASOURCE_H
#define FIFODATASOURCE_H

#include <QObject>
#include "GPSReader.h"  // Per la definizione di Coordinate

class FIFODataSource : public QObject {
    Q_OBJECT
public:
    explicit FIFODataSource(const QString &fifoPath, QObject *parent = nullptr);
    ~FIFODataSource();

    void start();
    void stop();

signals:
    void newCoordinate(const Coordinate &coord);

private:
    void readLoop();

    QString m_fifoPath;
    bool m_running;
};

#endif // FIFODATASOURCE_H
