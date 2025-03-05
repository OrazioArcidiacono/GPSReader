#ifndef GPSREADER_H
#define GPSREADER_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>

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

    void start();  // Avvia la lettura (tramite readyRead)
    void stop();   // Ferma la lettura, chiude la porta

signals:
    // Emesso quando viene riconosciuta una frase GPRMC valida (esempio)
    void newCoordinate(const Coordinate &coord);
    // Emesso per ogni frase NMEA letta (una per riga) insieme al Block ID
    void rawDataReceived(const QString &rawNmea, int blockId);
    // Emesso per comunicare lo stato (es. "Lock acquisito", "Porta riavviata", ecc.)
    void statusUpdated(const QString &status);

public slots:
    // Slot per controlli
    void pauseReception();
    void resumeReception();
    void restartPort();

private slots:
    void readData();

private:
    bool tryOpenPort(const QString &portName);
    void parseNMEA(const QByteArray &line); // Esempio base per GPRMC

    QSerialPort *m_serial;
    QByteArray m_buffer; // Buffer per accumulare i dati parziali
    bool m_paused;       // Indica se la ricezione è in pausa
    int m_blockCounter;  // Contatore di blocco: ogni chiamata a readData() incrementa questo valore
};

#endif // GPSREADER_H
