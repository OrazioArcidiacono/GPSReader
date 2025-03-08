#ifndef GENERICFALLBACKPARSER_H
#define GENERICFALLBACKPARSER_H

#include "INMEASentenceParser.h"
#include <QStringList>

class GenericFallbackParser : public INMEASentenceParser {
public:
    virtual QString sentenceType() const override { return "Generic"; }
    virtual bool parse(const QString &rawSentence) override;
    virtual QMap<QString, QVariant> fields() const override { return m_fields; }
    virtual QString errorMessage() const override { return m_errorMessage; }
private:
    QMap<QString, QVariant> m_fields;
    QString m_errorMessage;
};

#endif // GENERICFALLBACKPARSER_H
