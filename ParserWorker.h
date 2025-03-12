#ifndef PARSERWORKER_H
#define PARSERWORKER_H

#include <QObject>
#include "NMEAParser.h"
#include "ThreadSafeQueue.h"
#include "GPSReader.h" // For the Coordinate structure

/**
 * @brief ParserWorker processes raw NMEA sentences from a ThreadSafeQueue.
 *
 * It continuously dequeues sentences, parses them using NMEAParser, and emits signals with:
 * - The complete parsed data (ParsedData)
 * - A new coordinate (if available), along with the message type.
 */
class ParserWorker : public QObject {
    Q_OBJECT
public:
    explicit ParserWorker(ThreadSafeQueue *queue, QObject *parent = nullptr);
    ~ParserWorker();

public slots:
    /**
     * @brief Main processing loop. Blocks waiting for new sentences.
     */
    void process();

    /**
     * @brief Stops the processing loop.
     */
    void stop();

signals:
    /**
     * @brief Emitted when parsed data is available.
     * @param parsedData The structured parsed data.
     */
    void parsedDataAvailable(const NMEAParser::ParsedData &parsedData);

    /**
     * @brief Emitted when a new coordinate is available.
     * @param coord The parsed coordinate.
     * @param messageType The type of the NMEA sentence that produced the coordinate.
     */
    void newCoordinate(const Coordinate &coord, const QString &messageType);

private:
    ThreadSafeQueue *m_queue;
    bool m_stop;
    Coordinate m_lastCoordinate; // Last emitted coordinate for filtering

};

#endif // PARSERWORKER_H
