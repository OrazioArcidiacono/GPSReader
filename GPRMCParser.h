#ifndef GPRMCPARSER_H
#define GPRMCPARSER_H

#include "GenericNMEASentenceParser.h"

class GPRMCParser : public GenericNMEASentenceParser<GPRMCParser> {
public:
    virtual QString sentenceType() const override { return "GPRMC"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPRMCPARSER_H
