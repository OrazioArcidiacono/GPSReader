#ifndef HDTPARSER_H
#define HDTPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the HDT NMEA sentence.
 *
 * Parses the true heading.
 */
class HDTParser : public GenericNMEASentenceParser<HDTParser> {
public:
    virtual QString sentenceType() const override { return "HDT"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // HDTPARSER_H
