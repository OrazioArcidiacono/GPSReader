#ifndef GPGSVPARSER_H
#define GPGSVPARSER_H

#include "GenericNMEASentenceParser.h"

class GPGSVParser : public GenericNMEASentenceParser<GPGSVParser> {
public:
    virtual QString sentenceType() const override { return "GPGSV"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGSVPARSER_H
