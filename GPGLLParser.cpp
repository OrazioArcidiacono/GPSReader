#include "GPGLLParser.h"

bool GPGLLParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 7) {
        m_errorMessage = "Incomplete GPGLL sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);         // "$GPGLL"
    m_fields.insert("rawLatitude", tokens[1]);          // ddmm.mmmm
    m_fields.insert("latDirection", tokens[2]);         // N/S
    m_fields.insert("rawLongitude", tokens[3]);         // dddmm.mmmm
    m_fields.insert("lonDirection", tokens[4]);         // E/W
    m_fields.insert("utcTime", tokens[5]);              // UTC time
    m_fields.insert("status", tokens[6]);               // A = valid, V = invalid
    if (tokens.size() > 7)
        m_fields.insert("mode", tokens[7]);           // Optional mode
    bool okLat = false, okLon = false;
    double rawLat = tokens[1].toDouble(&okLat);
    double rawLon = tokens[3].toDouble(&okLon);
    if (!okLat || !okLon) {
        m_errorMessage = "Invalid numeric conversion for GPGLL coordinates";
        return false;
    }
    int latDeg = static_cast<int>(rawLat / 100);
    double latMin = rawLat - latDeg * 100;
    double latitude = latDeg + latMin / 60.0;
    if (tokens[2] == "S")
        latitude = -latitude;
    int lonDeg = static_cast<int>(rawLon / 100);
    double lonMin = rawLon - lonDeg * 100;
    double longitude = lonDeg + lonMin / 60.0;
    if (tokens[4] == "W")
        longitude = -longitude;
    m_fields.insert("latitude", latitude);
    m_fields.insert("longitude", longitude);
    m_valid = true;
    return true;
}
