#include "HDTParser.h"

bool HDTParser::parseTokens(const QStringList &tokens) {
    // HDT: "$--HDT,x.x,T*hh"
    if (tokens.size() < 3) {
        m_errorMessage = "Incomplete HDT sentence.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);
    m_fields.insert("trueHeading", tokens[1]);
    m_fields.insert("unit", tokens[2]); // "T"
    m_valid = true;
    return true;
}
