#ifndef FIFOWRITER_H
#define FIFOWRITER_H

#include <QObject>
#include "GPSReader.h" // For Coordinate definition

/**
 * @brief FIFOWriter writes GPS coordinate data into a FIFO (named pipe).
 *
 * It receives the coordinate and the message type (e.g. "GPRMC") and writes a formatted string
 * to the FIFO. The expected format is:
 * "Type: <messageType>, Lat: <latitude>, Lon: <longitude>\n"
 */
class FIFOWriter : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructor.
     * @param fifoPath The path to the FIFO.
     * @param parent Parent QObject.
     */
    explicit FIFOWriter(const QString &fifoPath, QObject *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~FIFOWriter();

public slots:
    /**
     * @brief Writes the coordinate data to the FIFO.
     * @param coord The GPS coordinate.
     * @param messageType The type of NMEA sentence (e.g., "GPRMC").
     */
    void writeCoordinate(const Coordinate &coord, const QString &messageType);

signals:
    // Signal emitted after writing to the FIFO, with the formatted data.
    void dataWritten(const QString &data);

private:
    QString m_fifoPath;
};

#endif // FIFOWRITER_H
