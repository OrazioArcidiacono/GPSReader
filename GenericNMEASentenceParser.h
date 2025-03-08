#ifndef GENERICNMEASENTENCEPARSER_H
#define GENERICNMEASENTENCEPARSER_H

#include "INMEASentenceParser.h"
#include <QStringList>

template <typename Derived>
class GenericNMEASentenceParser : public INMEASentenceParser {
public:
    GenericNMEASentenceParser() : m_valid(false) {}

    virtual bool parse(const QString &rawSentence) override {
        m_fields.clear();
        m_errorMessage.clear();
        m_valid = false;

        if (!rawSentence.startsWith("$")) {
            m_errorMessage = "Sentence does not start with '$'";
            return false;
        }
        QString sentence = rawSentence;
        int starIndex = sentence.indexOf('*');
        if (starIndex != -1) {
            sentence = sentence.left(starIndex);
        }
        QStringList tokens = sentence.split(',');
        if (tokens.isEmpty()) {
            m_errorMessage = "No tokens found";
            return false;
        }
        QString type = tokens[0].mid(1);
        if (type != this->sentenceType()) {
            m_errorMessage = "Sentence type mismatch: expected " + this->sentenceType() + " got " + type;
            return false;
        }
        return static_cast<Derived*>(this)->parseTokens(tokens);
    }

    virtual QMap<QString, QVariant> fields() const override {
        return m_fields;
    }

    virtual QString errorMessage() const override {
        return m_errorMessage;
    }

protected:
    QMap<QString, QVariant> m_fields;
    QString m_errorMessage;
    bool m_valid;
};

#endif // GENERICNMEASENTENCEPARSER_H
