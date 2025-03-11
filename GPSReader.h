#ifndef GPSREADER_H
#define GPSREADER_H

#include <QObject>
#include <QByteArray>
#include <QSerialPort>
#include "NMEAParser.h"

/**
 * @brief Structure representing a GPS coordinate.
 */
struct Coordinate {
    double latitude;
    double longitude;
};
/**
 * @brief GPSReader handles the reading of GPS data from a serial port.
 *
 * It continuously reads raw data, buffers it, extracts complete NMEA sentences,
 * and uses the NMEAParser facade to parse them. It emits signals with the raw sentence,
 * parsed data, and if available, a new coordinate.
 */
class GPSReader : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Constructs a GPSReader.
     * @param portName The name of the serial port.
     * @param baudRate The baud rate for the serial communication.
     * @param parent The parent QObject.
     */
    explicit GPSReader(const QString &portName, int baudRate = 9600, QObject *parent = nullptr);
    /**
     * @brief Destructor. Closes the serial port.
     */
    ~GPSReader();
    /**
     * @brief Starts the data reading process.
     */
    void start();  // Avvia la lettura
    /**
     * @brief Stops the data reading process and closes the serial port.
     */
    void stop();   // Ferma la lettura, chiude la porta

signals:
    /**
     * @brief Emitted when a valid coordinate is extracted (from a GPRMC sentence).
     */
    void newCoordinate(const Coordinate &coord);
    /**
     * @brief Emitted when a raw NMEA sentence is received.
     */
    void rawDataReceived(const QString &rawNmea, int blockId);
    /**
     * @brief Emitted to update status messages.
     */
    void statusUpdated(const QString &status);
    /**
     * @brief Emitted with the parsed data for each sentence.
     */
    // Il segnale parsedDataReceived non verrà usato automaticamente per aggiornare la GUI
    void parsedDataReceived(const NMEAParser::ParsedData &parsedData);

public slots:
    /**
     * @brief Pauses the reception of data.
     */
    void pauseReception();
    /**
     * @brief Resumes the reception of data.
     */
    void resumeReception();
    /**
     * @brief Restarts the serial port.
     */
    void restartPort();

private slots:
    /**
     * @brief Reads data from the serial port, buffers it, and processes complete sentences.
     */
    void readData();

private:
    /**
     * @brief Tries to open the serial port.
     * @param portName The name of the port.
     * @return True if the port is successfully opened.
     */
    bool tryOpenPort(const QString &portName);

    QSerialPort *m_serial;  ///< Serial port for data reception.
    QByteArray m_buffer;    ///< Buffer to accumulate raw data.
    bool m_paused;          ///< Flag indicating whether reception is paused.
    int m_blockCounter;     ///< Counter for received data blocks.
    Coordinate m_lastCoordinate; ///< Stored coordinate

};

#endif // GPSREADER_H
