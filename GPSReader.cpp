#include "GPSReader.h"
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QCoreApplication>
#include <csignal>
#include <cstdio>
#include <cstdlib>

// Costruttore
GPSReader::GPSReader(const QString &portName, int baudRate, QObject *parent)
    : QObject(parent)
    , m_serial(new QSerialPort(this))
    , m_paused(false)
    , m_blockCounter(0)
{
    m_serial->setPortName(portName);
    m_serial->setBaudRate(baudRate);

    emit statusUpdated("Inizializzazione modulo GPS...");

    if (!tryOpenPort(portName)) {
        emit statusUpdated("Impossibile aprire la porta: " + m_serial->errorString());
        return;
    }

    emit statusUpdated("Connessione seriale stabilita.");
    connect(m_serial, &QSerialPort::readyRead, this, &GPSReader::readData);
}

GPSReader::~GPSReader()
{
    stop();
}

bool GPSReader::tryOpenPort(const QString &portName)
{
    if (m_serial->open(QIODevice::ReadOnly))
        return true;
    qWarning() << "Errore nell'aprire la porta" << portName << ":" << m_serial->errorString();
    return false;
}

void GPSReader::start()
{
    // La lettura è gestita da readyRead
}

void GPSReader::stop()
{
    if (m_serial->isOpen())
        m_serial->close();
}

void GPSReader::pauseReception()
{
    m_paused = true;
    emit statusUpdated("Ricezione in pausa.");
}

void GPSReader::resumeReception()
{
    m_paused = false;
    emit statusUpdated("Ricezione ripresa.");
}

void GPSReader::restartPort()
{
    if (m_serial->isOpen())
        m_serial->close();

    emit statusUpdated("Riavvio della porta seriale...");
    if (m_serial->open(QIODevice::ReadOnly)) {
        connect(m_serial, &QSerialPort::readyRead, this, &GPSReader::readData, Qt::UniqueConnection);
        emit statusUpdated("Porta riavviata e in ricezione.");
    } else {
        emit statusUpdated("Errore nel riavvio della porta: " + m_serial->errorString());
    }
}

void GPSReader::readData()
{
    if (m_paused)
        return;

    // Incrementa il contatore di blocco per ogni chiamata a readData()
    m_blockCounter++;

    QByteArray chunk = m_serial->readAll();
    m_buffer.append(chunk);

    while (true) {
        int idx = m_buffer.indexOf("\r\n");
        if (idx < 0) {
            idx = m_buffer.indexOf('\n');
        }
        if (idx < 0)
            break;

        QByteArray line = m_buffer.left(idx);
        m_buffer.remove(0, idx + 2); // Rimuove \r\n
        line = line.trimmed();
        if (line.isEmpty())
            continue;

        if (line.startsWith('$')) {
            emit rawDataReceived(QString::fromLatin1(line), m_blockCounter);
            parseNMEA(line);
        } else {
            qDebug() << "Riga non NMEA:" << line;
        }
    }
}

// Esempio di parsing base per GPRMC (puoi estendere per altre frasi)
void GPSReader::parseNMEA(const QByteArray &data)
{
    QString line = QString::fromLatin1(data).trimmed();
    if (!line.startsWith("$GPRMC"))
        return;

    QStringList tokens = line.split(',');
    if (tokens.size() < 7)
        return;

    if (tokens.at(2) != "A") // Verifica lo status
        return;

    bool okLat, okLon;
    double rawLat = tokens.at(3).toDouble(&okLat);
    double rawLon = tokens.at(5).toDouble(&okLon);
    if (!okLat || !okLon)
        return;

    int latDeg = (int)(rawLat / 100);
    double latMin = rawLat - latDeg * 100;
    double latitude = latDeg + latMin / 60.0;
    if (tokens.at(4) == "S")
        latitude = -latitude;

    int lonDeg = (int)(rawLon / 100);
    double lonMin = rawLon - lonDeg * 100;
    double longitude = lonDeg + lonMin / 60.0;
    if (tokens.at(6) == "W")
        longitude = -longitude;

    Coordinate coord { latitude, longitude };
    emit newCoordinate(coord);
}
