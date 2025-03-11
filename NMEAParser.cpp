#include "NMEAParser.h"
#include "GPRMCParser.h"
#include "GPGGAParser.h"
#include "GPVTGParser.h"
#include "GPGLLParser.h"
#include "GPGSAParser.h"
#include "GPGSVParser.h"
#include "GPTXTParser.h"
#include "GPZDAParser.h"
#include "HDGParser.h"
#include "HDMParser.h"
#include "HDTParser.h"
#include "GenericFallbackParser.h"
#include <QStringList>
/**
 * @brief Helper function that creates a parser instance based on the sentence type.
 * @param type The type of the sentence (e.g., "GPRMC").
 * @return A pointer to an instance of a specific INMEASentenceParser.
 */
static INMEASentenceParser* createParser(const QString &type) {
    if (type == "GPRMC") {
        return new GPRMCParser();
    } else if (type == "GPGGA") {
        return new GPGGAParser();
    } else if (type == "GPVTG") {
        return new GPVTGParser();
    } else if (type == "GPGLL") {
        return new GPGLLParser();
    } else if (type == "GPGSA") {
        return new GPGSAParser();
    } else if (type == "GPGSV") {
        return new GPGSVParser();
    } else if (type == "GPTXT") {
        return new GPTXTParser();
    } else if (type == "GPZDA") {
        return new GPZDAParser();
    } else if (type == "HDG") {
        return new HDGParser();
    } else if (type == "HDM") {
        return new HDMParser();
    } else if (type == "HDT") {
        return new HDTParser();
    }
    // Per tutti gli altri, usa il parser generico
    return new GenericFallbackParser();
}

NMEAParser::ParsedData NMEAParser::parse(const QString &rawSentence) {
    ParsedData result;
    if (!rawSentence.startsWith("$")) {
        result.valid = false;
        result.errorMessage = "Sentence does not start with '$'";
        return result;
    }
    QString sentence = rawSentence;
    int starIndex = sentence.indexOf('*');
    if (starIndex != -1) {
        sentence = sentence.left(starIndex);
    }
    QStringList tokens = sentence.split(',');
    if (tokens.isEmpty()) {
        result.valid = false;
        result.errorMessage = "No tokens found";
        return result;
    }
    QString type = tokens[0].mid(1);
    result.sentenceType = type;

    INMEASentenceParser* parser = createParser(type);
    if (parser) {
        bool ok = parser->parse(rawSentence);
        result.valid = ok;
        result.errorMessage = parser->errorMessage();
        result.fields = parser->fields();
        delete parser;
    } else {
        result.valid = false;
        result.errorMessage = "No parser available for this sentence type.";
    }
    return result;
}
