#ifndef HDTPARSER_H
#define HDTPARSER_H

#include "GenericNMEASentenceParser.h"

class HDTParser : public GenericNMEASentenceParser<HDTParser> {
public:
    virtual QString sentenceType() const override { return "HDT"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // HDTPARSER_H
