#ifndef GPVTGPARSER_H
#define GPVTGPARSER_H

#include "GenericNMEASentenceParser.h"

class GPVTGParser : public GenericNMEASentenceParser<GPVTGParser> {
public:
    virtual QString sentenceType() const override { return "GPVTG"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPVTGPARSER_H
