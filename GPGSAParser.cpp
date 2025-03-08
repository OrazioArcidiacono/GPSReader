#include "GPGSAParser.h"

bool GPGSAParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 18) {
        m_errorMessage = "Incomplete GPGSA sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);       // "$GPGSA"
    m_fields.insert("selectionMode", tokens[1]);      // A/M
    m_fields.insert("fixType", tokens[2]);            // 1=no fix,2=2D,3=3D
    // Sat IDs (token 3-14)
    for (int i = 3; i <= 14; i++) {
        m_fields.insert("satellite" + QString::number(i-2), tokens[i]);
    }
    m_fields.insert("PDOP", tokens[15]);
    m_fields.insert("HDOP", tokens[16]);
    m_fields.insert("VDOP", tokens[17]);
    m_valid = true;
    return true;
}
