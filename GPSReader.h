#ifndef GPSREADER_H
#define GPSREADER_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include "NMEAParser.h"

// Struttura per rappresentare le coordinate GPS
struct Coordinate {
    double latitude;
    double longitude;
};

class GPSReader : public QObject
{
    Q_OBJECT
public:
    explicit GPSReader(const QString &portName, int baudRate = 9600, QObject *parent = nullptr);
    ~GPSReader();

    void start();  // Avvia la lettura
    void stop();   // Ferma la lettura, chiude la porta

signals:
    void newCoordinate(const Coordinate &coord);
    void rawDataReceived(const QString &rawNmea, int blockId);
    void statusUpdated(const QString &status);
    // Il segnale parsedDataReceived non verrà usato automaticamente per aggiornare la GUI
    void parsedDataReceived(const NMEAParser::ParsedData &parsedData);

public slots:
    void pauseReception();
    void resumeReception();
    void restartPort();

private slots:
    void readData();

private:
    bool tryOpenPort(const QString &portName);

    QSerialPort *m_serial;
    QByteArray m_buffer;
    bool m_paused;
    int m_blockCounter;
};

#endif // GPSREADER_H
