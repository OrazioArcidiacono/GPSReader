#include "ParserWorker.h"
#include "NMEAParser.h"
#include <QDebug>
#include <cmath>

ParserWorker::ParserWorker(ThreadSafeQueue *queue, QObject *parent)
    : QObject(parent), m_queue(queue), m_stop(false)
{
}

ParserWorker::~ParserWorker() {
}

void ParserWorker::process() {
    while (!m_stop) {
        // Block until a new sentence is available
        QString rawSentence = m_queue->dequeue();
        // If an empty sentence is dequeued, it's a signal to stop.
        if (rawSentence.isEmpty())
            break;

        // Parse the sentence using the NMEAParser facade.
        NMEAParser::ParsedData parsed = NMEAParser::parse(rawSentence);
        emit parsedDataAvailable(parsed);

        // If the sentence type is one that contains coordinates (e.g., GPRMC, GPGGA, GPGLL)
        if (parsed.valid &&
            (parsed.sentenceType == "GPRMC" ||
             parsed.sentenceType == "GPGGA" ||
             parsed.sentenceType == "GPGLL"))
        {
            if (parsed.fields.contains("latitude") && parsed.fields.contains("longitude")) {
                Coordinate coord { parsed.fields.value("latitude").toDouble(),
                                 parsed.fields.value("longitude").toDouble() };
                // Emit the new coordinate along with the message type
                emit newCoordinate(coord, parsed.sentenceType);
            }
        }
    }
}

void ParserWorker::stop() {
    m_stop = true;
    // Enqueue an empty string to unblock dequeue()
    m_queue->enqueue(QString());
}
