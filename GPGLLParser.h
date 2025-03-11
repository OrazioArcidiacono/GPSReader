#ifndef GPGLLPARSER_H
#define GPGLLPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the GPGLL NMEA sentence.
 *
 * Parses latitude, longitude, UTC time, and status (A = valid, V = invalid).
 */
class GPGLLParser : public GenericNMEASentenceParser<GPGLLParser> {
public:
    virtual QString sentenceType() const override { return "GPGLL"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGLLPARSER_H
