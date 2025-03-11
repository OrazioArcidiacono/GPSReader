#ifndef INMEASENTENCEPARSER_H
#define INMEASENTENCEPARSER_H

#include <QString>
#include <QMap>
#include <QVariant>
/**
 * @brief Base interface for parsing NMEA sentences.
 *
 * This interface defines the essential methods that every NMEA sentence parser must implement.
 * An NMEA parser is responsible for interpreting an NMEA sentence, splitting it into its components,
 * and providing access to the extracted data in an organized structure (using QMap).
 */
class INMEASentenceParser {
public:
    /**
     * @brief Virtual destructor.
     *
     * Ensures that derived objects are properly destroyed when deleted via a pointer
     * to INMEASentenceParser.
     */
    virtual ~INMEASentenceParser() {}
    /**
     * @brief Returns the type of the NMEA sentence.
     *
     * For example, for a sentence that begins with "$GPRMC", this method should return "GPRMC".
     *
     * @return The sentence type as a QString.
     */
    virtual QString sentenceType() const = 0;
    /**
     * @brief Parses a raw NMEA sentence.
     *
     * This method analyzes the rawSentence string, splits it into tokens, and populates the
     * internal structure with the parsed data.
     *
     * @param rawSentence The NMEA sentence to be parsed.
     * @return True if parsing was successful, false otherwise.
     */
    virtual bool parse(const QString &rawSentence) = 0;
    /**
     * @brief Returns the map of parsed fields.
     *
     * The map contains key/value pairs where the keys are descriptive names of the fields
     * (for example "utcTime", "latitude", "fixQuality", etc.) and the values are stored as QVariants.
     *
     * @return A QMap containing the parsed fields.
     */
    virtual QMap<QString, QVariant> fields() const = 0;
    /**
     * @brief Returns any error messages produced during parsing.
     *
     * If the parse() method fails, this method provides additional details on the error.
     *
     * @return The error message as a QString.
     */
    virtual QString errorMessage() const = 0;
};

#endif // INMEASENTENCEPARSER_H
