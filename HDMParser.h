#ifndef HDMPARSER_H
#define HDMPARSER_H

#include "GenericNMEASentenceParser.h"

class HDMParser : public GenericNMEASentenceParser<HDMParser> {
public:
    virtual QString sentenceType() const override { return "HDM"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // HDMPARSER_H
