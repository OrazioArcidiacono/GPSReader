#ifndef GPGSAPARSER_H
#define GPGSAPARSER_H

#include "GenericNMEASentenceParser.h"

class GPGSAParser : public GenericNMEASentenceParser<GPGSAParser> {
public:
    virtual QString sentenceType() const override { return "GPGSA"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGSAPARSER_H
