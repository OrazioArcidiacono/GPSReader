#ifndef HDGPARSER_H
#define HDGPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the HDG NMEA sentence.
 *
 * Parses the magnetic heading, deviation, and variation information.
 */
class HDGParser : public GenericNMEASentenceParser<HDGParser> {
public:
    virtual QString sentenceType() const override { return "HDG"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // HDGPARSER_H
