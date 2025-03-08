#include "GPGSVParser.h"

bool GPGSVParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 4) {
        m_errorMessage = "Incomplete GPGSV sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);       // "$GPGSV"
    m_fields.insert("totalMessages", tokens[1]);      // Total messages
    m_fields.insert("messageNumber", tokens[2]);      // Message number
    m_fields.insert("totalSatellites", tokens[3]);    // Total satellites in view
    int groupCount = (tokens.size() - 4) / 4;
    for (int i = 0; i < groupCount; i++) {
        int base = 4 + i * 4;
        QString prefix = "satellite" + QString::number(i + 1);
        m_fields.insert(prefix + "_ID", tokens[base]);
        m_fields.insert(prefix + "_elevation", tokens[base + 1]);
        m_fields.insert(prefix + "_azimuth", tokens[base + 2]);
        m_fields.insert(prefix + "_SNR", tokens[base + 3]);
    }
    m_valid = true;
    return true;
}
