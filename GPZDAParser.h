#ifndef GPZDAPARSER_H
#define GPZDAPARSER_H

#include "GenericNMEASentenceParser.h"

class GPZDAParser : public GenericNMEASentenceParser<GPZDAParser> {
public:
    virtual QString sentenceType() const override { return "GPZDA"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPZDAPARSER_H
