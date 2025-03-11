#ifndef GPGSVPARSER_H
#define GPGSVPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the GPGSV NMEA sentence.
 *
 * Parses the total number of messages, message number, total satellites in view,
 * and details for each satellite (ID, elevation, azimuth, and SNR).
 */
class GPGSVParser : public GenericNMEASentenceParser<GPGSVParser> {
public:
    virtual QString sentenceType() const override { return "GPGSV"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGSVPARSER_H
