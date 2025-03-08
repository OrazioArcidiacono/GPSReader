#ifndef GPGLLPARSER_H
#define GPGLLPARSER_H

#include "GenericNMEASentenceParser.h"

class GPGLLParser : public GenericNMEASentenceParser<GPGLLParser> {
public:
    virtual QString sentenceType() const override { return "GPGLL"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPGLLPARSER_H
