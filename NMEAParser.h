#ifndef NMEAPARSER_H
#define NMEAPARSER_H

#include <QString>
#include <QMap>
#include <QVariant>
/**
 * @brief Facade class for parsing NMEA sentences.
 *
 * This class provides a static interface for parsing raw NMEA sentences. It
 * determines the sentence type and creates a specific parser (or a fallback parser)
 * to perform the parsing. The result is returned in a ParsedData structure containing:
 *  - the sentence type,
 *  - a map of parsed fields (with descriptive keys),
 *  - a validity flag,
 *  - and an error message if parsing fails.
 */
class NMEAParser {
public:
    /**
     * @brief Structure to hold parsed NMEA data.
     *
     * This structure contains the type of the sentence, a map with the parsed fields,
     * a flag indicating whether the parsing was successful, and an error message (if any).
     */
    struct ParsedData {
        QString sentenceType;           ///< The type of the NMEA sentence (e.g., "GPRMC")
        QMap<QString, QVariant> fields; ///< Map of parsed fields (e.g., "utcTime", "latitude", "fixQuality", etc.)
        bool valid;                     ///< True if parsing succeeded, false otherwise
        QString errorMessage;           ///< Detailed error message if parsing fails
    };

    /**
     * @brief Parses a raw NMEA sentence.
     *
     * This static function serves as the main interface for NMEA parsing.
     * It takes a raw NMEA sentence, determines its type, creates the appropriate parser,
     * and returns a ParsedData structure with all the relevant information.
     *
     * @param rawSentence The raw NMEA sentence to be parsed (including the starting '$' and checksum).
     * @return A ParsedData structure containing the sentence type, parsed fields, validity flag, and error message.
     */
    static ParsedData parse(const QString &rawSentence);
};

#endif // NMEAPARSER_H
