#ifndef GPGGAPARSER_H
#define GPGGAPARSER_H

#include "GenericNMEASentenceParser.h"

/**
 * @brief Parser for the GPGGA NMEA sentence.
 *
 * Parses information such as UTC time, latitude, longitude, fix quality, number of satellites,
 * HDOP, altitude, and geoid separation, and converts coordinates to decimal degrees.
 */
class GPGGAParser : public GenericNMEASentenceParser<GPGGAParser> {
public:
    virtual QString sentenceType() const override { return "GPGGA"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGGAPARSER_H
