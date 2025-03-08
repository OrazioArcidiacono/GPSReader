#include "GPTXTParser.h"

bool GPTXTParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 4) {
        m_errorMessage = "Incomplete GPTXT sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);       // "$GPTXT"
    m_fields.insert("totalMessages", tokens[1]);      // Total messages
    m_fields.insert("messageNumber", tokens[2]);      // Message number
    m_fields.insert("messageType", tokens[3]);        // Message type
    QString text;
    for (int i = 4; i < tokens.size(); i++) {
        text += tokens[i];
        if (i < tokens.size() - 1)
            text += ",";
    }
    m_fields.insert("text", text);
    m_valid = true;
    return true;
}
