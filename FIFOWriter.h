#ifndef FIFOWRITER_H
#define FIFOWRITER_H

#include <QObject>
#include "GPSReader.h" // Per la definizione di Coordinate

class FIFOWriter : public QObject {
    Q_OBJECT
public:
    explicit FIFOWriter(const QString &fifoPath, QObject *parent = nullptr);
    ~FIFOWriter();

public slots:
    void writeCoordinate(const Coordinate &coord);

private:
    QString m_fifoPath;
};

#endif // FIFOWRITER_H
