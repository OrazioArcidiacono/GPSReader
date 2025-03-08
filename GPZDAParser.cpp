#include "GPZDAParser.h"

bool GPZDAParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 7) {
        m_errorMessage = "Incomplete GPZDA sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);         // "$GPZDA"
    m_fields.insert("utcTime", tokens[1]);              // hhmmss.ss
    m_fields.insert("day", tokens[2]);                  // dd
    m_fields.insert("month", tokens[3]);                // mm
    m_fields.insert("year", tokens[4]);                 // yyyy
    m_fields.insert("localZoneHours", tokens[5]);       // Local zone hours
    m_fields.insert("localZoneMinutes", tokens[6]);     // Local zone minutes
    m_valid = true;
    return true;
}
