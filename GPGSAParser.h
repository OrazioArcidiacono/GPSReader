#ifndef GPGSAPARSER_H
#define GPGSAPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the GPGSA NMEA sentence.
 *
 * Parses selection mode, fix type, IDs of satellites used for the fix,
 * and the PDOP, HDOP, and VDOP values.
 */
class GPGSAParser : public GenericNMEASentenceParser<GPGSAParser> {
public:
    virtual QString sentenceType() const override { return "GPGSA"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGSAPARSER_H
