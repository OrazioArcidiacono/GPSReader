#include "GenericFallbackParser.h"

bool GenericFallbackParser::parse(const QString &rawSentence) {
    m_fields.clear();
    m_errorMessage.clear();
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
    for (int i = 0; i < tokens.size(); i++) {
        m_fields.insert("field" + QString::number(i), tokens[i]);
    }
    return true;
}
