#ifndef GPTXTPARSER_H
#define GPTXTPARSER_H

#include "GenericNMEASentenceParser.h"

class GPTXTParser : public GenericNMEASentenceParser<GPTXTParser> {
public:
    virtual QString sentenceType() const override { return "GPTXT"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPTXTPARSER_H
