#ifndef HDGPARSER_H
#define HDGPARSER_H

#include "GenericNMEASentenceParser.h"

class HDGParser : public GenericNMEASentenceParser<HDGParser> {
public:
    virtual QString sentenceType() const override { return "HDG"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // HDGPARSER_H
