#ifndef GPGGAPARSER_H
#define GPGGAPARSER_H

#include "GenericNMEASentenceParser.h"

class GPGGAParser : public GenericNMEASentenceParser<GPGGAParser> {
public:
    virtual QString sentenceType() const override { return "GPGGA"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGGAPARSER_H
