#ifndef GPTXTPARSER_H
#define GPTXTPARSER_H

#include "GenericNMEASentenceParser.h"
/**
 * @brief Parser for the GPTXT NMEA sentence.
 *
 * Extracts the total number of messages, current message number, message type,
 * and the text content.
 */
class GPTXTParser : public GenericNMEASentenceParser<GPTXTParser> {
public:
    virtual QString sentenceType() const override { return "GPTXT"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPTXTPARSER_H
