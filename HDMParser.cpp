#include "HDMParser.h"

bool HDMParser::parseTokens(const QStringList &tokens) {
    // HDM: "$--HDM,x.x,M*hh"
    if (tokens.size() < 3) {
        m_errorMessage = "Incomplete HDM sentence.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);
    m_fields.insert("magneticHeading", tokens[1]);
    m_fields.insert("unit", tokens[2]); // "M"
    m_valid = true;
    return true;
}
