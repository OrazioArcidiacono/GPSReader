#include "GPSReader.h"
#include "NMEAParser.h"
#include <QDebug>
#include <QThread>

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

GPSReader::~GPSReader() {
    stop();
}

bool GPSReader::tryOpenPort(const QString &portName) {
    if (m_serial->open(QIODevice::ReadOnly))
        return true;
    qWarning() << "Errore nell'aprire la porta" << portName << ":" << m_serial->errorString();
    return false;
}

void GPSReader::start() {
    // La lettura è gestita tramite readyRead
}

void GPSReader::stop() {
    if (m_serial->isOpen())
        m_serial->close();
}

void GPSReader::pauseReception() {
    m_paused = true;
    emit statusUpdated("Ricezione in pausa.");
}

void GPSReader::resumeReception() {
    m_paused = false;
    emit statusUpdated("Ricezione ripresa.");
}

void GPSReader::restartPort() {
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

void GPSReader::readData() {
    if (m_paused)
        return;

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
        m_buffer.remove(0, idx + 2);
        line = line.trimmed();
        if (line.isEmpty())
            continue;

        if (line.startsWith('$')) {
            QString rawSentence = QString::fromLatin1(line);
            emit rawDataReceived(rawSentence, m_blockCounter);

            NMEAParser::ParsedData parsed = NMEAParser::parse(rawSentence);
            emit parsedDataReceived(parsed);

            if (parsed.valid && parsed.sentenceType == "GPRMC") {
                if (parsed.fields.contains("latitude") && parsed.fields.contains("longitude")) {
                    Coordinate coord { parsed.fields.value("latitude").toDouble(),
                                     parsed.fields.value("longitude").toDouble() };
                    emit newCoordinate(coord);
                }
            }
        } else {
            qDebug() << "Riga non NMEA:" << line;
        }
    }
}
