#include "GPRMCParser.h"

bool GPRMCParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 12) {
        m_errorMessage = "Incomplete GPRMC sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    // Mappa completa dei campi secondo lo standard NMEA 0183
    m_fields.insert("sentenceID", tokens[0]);         // "$GPRMC"
    m_fields.insert("utcTime", tokens[1]);              // hhmmss.sss
    m_fields.insert("status", tokens[2]);               // A = valid, V = invalid
    m_fields.insert("rawLatitude", tokens[3]);          // ddmm.mmmm
    m_fields.insert("latDirection", tokens[4]);         // N/S
    m_fields.insert("rawLongitude", tokens[5]);         // dddmm.mmmm
    m_fields.insert("lonDirection", tokens[6]);         // E/W
    m_fields.insert("speedKnots", tokens[7]);           // in knots
    m_fields.insert("course", tokens[8]);               // degrees true
    m_fields.insert("date", tokens[9]);                 // ddmmyy
    m_fields.insert("magVariation", tokens[10]);        // degrees (optional)
    if (tokens.size() > 11)
        m_fields.insert("magVariationDirection", tokens[11]); // E/W
    if (tokens[2] != "A") {
        m_errorMessage = "GPRMC status invalid: " + tokens[2];
        return false;
    }
    bool okLat = false, okLon = false;
    double rawLat = tokens[3].toDouble(&okLat);
    double rawLon = tokens[5].toDouble(&okLon);
    if (!okLat || !okLon) {
        m_errorMessage = "Invalid numeric conversion for latitude/longitude";
        return false;
    }
    int latDeg = static_cast<int>(rawLat / 100);
    double latMin = rawLat - latDeg * 100;
    double latitude = latDeg + latMin / 60.0;
    if (tokens[4] == "S")
        latitude = -latitude;
    int lonDeg = static_cast<int>(rawLon / 100);
    double lonMin = rawLon - lonDeg * 100;
    double longitude = lonDeg + lonMin / 60.0;
    if (tokens[6] == "W")
        longitude = -longitude;
    m_fields.insert("latitude", latitude);
    m_fields.insert("longitude", longitude);
    m_valid = true;
    return true;
}
