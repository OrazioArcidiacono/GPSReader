#include "GPRMCParser.h"

bool GPRMCParser::parseTokens(const QStringList &tokens) {
    // Verify minimum token count for GPRMC
    if (tokens.size() < 12) {
        m_errorMessage = "Incomplete GPRMC sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    // Store original tokens with descriptive field names
    m_fields.insert("sentenceID", tokens[0]);         // "$GPRMC"
    m_fields.insert("utcTime", tokens[1]);              // UTC Time in hhmmss.sss format
    m_fields.insert("status", tokens[2]);               // Status: A = valid, V = invalid
    m_fields.insert("rawLatitude", tokens[3]);          // Latitude in ddmm.mmmm format
    m_fields.insert("latDirection", tokens[4]);         // Latitude direction: N/S
    m_fields.insert("rawLongitude", tokens[5]);         // Longitude in dddmm.mmmm format
    m_fields.insert("lonDirection", tokens[6]);         // Longitude direction: E/W
    m_fields.insert("speedKnots", tokens[7]);           // Speed over ground in knots
    m_fields.insert("course", tokens[8]);               // Course (track angle) in degrees
    m_fields.insert("date", tokens[9]);                 // Date in ddmmyy format
    m_fields.insert("magVariation", tokens[10]);        // Magnetic variation in degrees
    if (tokens.size() > 11)
        m_fields.insert("magVariationDirection", tokens[11]); // Variation direction: E/W
    // If status is not valid, fail parsing.
    if (tokens[2] != "A") {
        m_errorMessage = "GPRMC status invalid: " + tokens[2];
        return false;
    }
    // Convert raw latitude and longitude to decimal degrees
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

    // Save converted coordinates
    m_fields.insert("latitude", latitude);
    m_fields.insert("longitude", longitude);
    m_valid = true;
    return true;
}
