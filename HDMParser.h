#ifndef HDMPARSER_H
#define HDMPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the HDM NMEA sentence.
 *
 * Parses the magnetic heading.
 */
class HDMParser : public GenericNMEASentenceParser<HDMParser> {
public:
    virtual QString sentenceType() const override { return "HDM"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // HDMPARSER_H
