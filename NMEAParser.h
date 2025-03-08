#ifndef NMEAPARSER_H
#define NMEAPARSER_H

#include <QString>
#include <QMap>
#include <QVariant>
#include "INMEASentenceParser.h"

class NMEAParser {
public:
    struct ParsedData {
        QString sentenceType;
        QMap<QString, QVariant> fields;
        bool valid;
        QString errorMessage;
    };

    // Interfaccia statica per il parsing
    static ParsedData parse(const QString &rawSentence);
};

#endif // NMEAPARSER_H
