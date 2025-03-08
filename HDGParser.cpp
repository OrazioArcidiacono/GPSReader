#include "HDGParser.h"

bool HDGParser::parseTokens(const QStringList &tokens) {
    // HDG: "$--HDG,x.x,x.x,a,x.x,a*hh"
    if (tokens.size() < 6) {
        m_errorMessage = "Incomplete HDG sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);         // "$HDG"
    m_fields.insert("magneticHeading", tokens[1]);      // Magnetic heading in degrees
    m_fields.insert("magneticDeviation", tokens[2]);    // Deviation
    m_fields.insert("deviationDirection", tokens[3]);   // E/W
    m_fields.insert("magneticVariation", tokens[4]);    // Variation
    m_fields.insert("variationDirection", tokens[5]);   // E/W
    m_valid = true;
    return true;
}
