#ifndef INMEASENTENCEPARSER_H
#define INMEASENTENCEPARSER_H

#include <QString>
#include <QMap>
#include <QVariant>

class INMEASentenceParser {
public:
    virtual ~INMEASentenceParser() {}
    virtual QString sentenceType() const = 0;
    virtual bool parse(const QString &rawSentence) = 0;
    virtual QMap<QString, QVariant> fields() const = 0;
    virtual QString errorMessage() const = 0;
};

#endif // INMEASENTENCEPARSER_H
