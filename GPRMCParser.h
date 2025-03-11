#ifndef GPRMCPARSER_H
#define GPRMCPARSER_H

#include "GenericNMEASentenceParser.h"

/**
 * @brief Parser for the GPRMC NMEA sentence.
 *
 * Parses fields such as UTC time, status, raw latitude/longitude, speed, course, date,
 * magnetic variation, and converts latitude and longitude to decimal degrees.
 */
class GPRMCParser : public GenericNMEASentenceParser<GPRMCParser> {
public:
    virtual QString sentenceType() const override { return "GPRMC"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPRMCPARSER_H
