#ifndef GENERICNMEASENTENCEPARSER_H
#define GENERICNMEASENTENCEPARSER_H

#include "INMEASentenceParser.h"
#include <QStringList>
/**
 * @brief Templated base class that implements common pre-parsing for NMEA sentences.
 *
 * It uses CRTP to delegate specific parsing to the derived class through the parseTokens() method.
 */
template <typename Derived>
class GenericNMEASentenceParser : public INMEASentenceParser {
public:
    GenericNMEASentenceParser() : m_valid(false) {}
    /**
     * @brief Parses the raw NMEA sentence.
     *
     * This method performs basic checks (e.g., that the sentence starts with '$', removes the checksum,
     * tokenizes the sentence, and validates the sentence type) before delegating to parseTokens().
     *
     * @param rawSentence The raw NMEA sentence.
     * @return True if parsing (including specific token parsing) is successful.
     */

    virtual bool parse(const QString &rawSentence) override {
        m_fields.clear();
        m_errorMessage.clear();
        m_valid = false;
        // Check that the sentence starts with '$'
        if (!rawSentence.startsWith("$")) {
            m_errorMessage = "Sentence does not start with '$'";
            return false;
        }
        QString sentence = rawSentence;
        // Remove checksum portion if present

        int starIndex = sentence.indexOf('*');
        if (starIndex != -1) {
            sentence = sentence.left(starIndex);
        }
        // Split sentence into tokens using comma as delimiter

        QStringList tokens = sentence.split(',');
        if (tokens.isEmpty()) {
            m_errorMessage = "No tokens found";
            return false;
        }
        // Verify sentence type matches expected type

        QString type = tokens[0].mid(1);
        if (type != this->sentenceType()) {
            m_errorMessage = "Sentence type mismatch: expected " + this->sentenceType() + " got " + type;
            return false;
        }
        // Delegate specific token parsing to derived class

        return static_cast<Derived*>(this)->parseTokens(tokens);
    }
    /// Returns the parsed fields.

    virtual QMap<QString, QVariant> fields() const override {
        return m_fields;
    }
    /// Returns any error message encountered during parsing.

    virtual QString errorMessage() const override {
        return m_errorMessage;
    }

protected:
    QMap<QString, QVariant> m_fields; ///< Map to store parsed field names and values.
    QString m_errorMessage;           ///< Error message if parsing fails.
    bool m_valid;                     ///< Flag indicating whether parsing was successful.
};

#endif // GENERICNMEASENTENCEPARSER_H
