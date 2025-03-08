#include "GPVTGParser.h"

bool GPVTGParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 9) {
        m_errorMessage = "Incomplete GPVTG sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);       // "$GPVTG"
    m_fields.insert("trackTrue", tokens[1]);          // True track in degrees
    m_fields.insert("literalT", tokens[2]);           // "T"
    m_fields.insert("trackMagnetic", tokens[3]);      // Magnetic track
    m_fields.insert("literalM", tokens[4]);           // "M"
    m_fields.insert("speedKnots", tokens[5]);         // Speed (knots)
    m_fields.insert("literalN", tokens[6]);           // "N"
    m_fields.insert("speedKmh", tokens[7]);           // Speed (km/h)
    m_fields.insert("literalK", tokens[8]);           // "K"
    if (tokens.size() > 9)
        m_fields.insert("mode", tokens[9]);          // Optional mode
    m_valid = true;
    return true;
}
