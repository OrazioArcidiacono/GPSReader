#include "GPGGAParser.h"

bool GPGGAParser::parseTokens(const QStringList &tokens) {
    if (tokens.size() < 15) {
        m_errorMessage = "Incomplete GPGGA sentence: " + QString::number(tokens.size()) + " tokens found.";
        return false;
    }
    m_fields.insert("sentenceID", tokens[0]);         // "$GPGGA"
    m_fields.insert("utcTime", tokens[1]);              // hhmmss.ss
    m_fields.insert("rawLatitude", tokens[2]);          // ddmm.mmmm
    m_fields.insert("latDirection", tokens[3]);         // N/S
    m_fields.insert("rawLongitude", tokens[4]);         // dddmm.mmmm
    m_fields.insert("lonDirection", tokens[5]);         // E/W
    m_fields.insert("fixQuality", tokens[6]);           // 0,1,2,...
    m_fields.insert("numSatellites", tokens[7]);        // 00-12
    m_fields.insert("hdop", tokens[8]);                 // Horizontal Dilution
    m_fields.insert("altitude", tokens[9]);             // Altitude
    m_fields.insert("altitudeUnit", tokens[10]);        // M
    m_fields.insert("geoidHeight", tokens[11]);         // Geoidal separation
    m_fields.insert("geoidHeightUnit", tokens[12]);     // M
    m_fields.insert("dgpsAge", tokens[13]);             // Age of DGPS data
    m_fields.insert("dgpsStationID", tokens[14]);       // DGPS station ID
    bool okLat = false, okLon = false;
    double rawLat = tokens[2].toDouble(&okLat);
    double rawLon = tokens[4].toDouble(&okLon);
    if (!okLat || !okLon) {
        m_errorMessage = "Invalid numeric conversion for GPGGA coordinates";
        return false;
    }
    int latDeg = static_cast<int>(rawLat / 100);
    double latMin = rawLat - latDeg * 100;
    double latitude = latDeg + latMin / 60.0;
    if (tokens[3] == "S")
        latitude = -latitude;
    int lonDeg = static_cast<int>(rawLon / 100);
    double lonMin = rawLon - lonDeg * 100;
    double longitude = lonDeg + lonMin / 60.0;
    if (tokens[5] == "W")
        longitude = -longitude;
    m_fields.insert("latitude", latitude);
    m_fields.insert("longitude", longitude);
    m_valid = true;
    return true;
}
