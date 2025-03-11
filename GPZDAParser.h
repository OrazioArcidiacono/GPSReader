#ifndef GPZDAPARSER_H
#define GPZDAPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the GPZDA NMEA sentence.
 *
 * Extracts UTC time, day, month, year, and local time zone information.
 */
class GPZDAParser : public GenericNMEASentenceParser<GPZDAParser> {
public:
    virtual QString sentenceType() const override { return "GPZDA"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPZDAPARSER_H
