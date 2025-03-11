#ifndef GPVTGPARSER_H
#define GPVTGPARSER_H

#include "GenericNMEASentenceParser.h"

/**
 * @brief Parser for the GPVTG NMEA sentence.
 *
 * Parses fields such as Track True, Track Magnetic, Speed in knots, Speed in km/h, and an optional mode.
 */
class GPVTGParser : public GenericNMEASentenceParser<GPVTGParser> {
public:
    virtual QString sentenceType() const override { return "GPVTG"; }
    bool parseTokens(const QStringList &tokens);
};

#endif // GPVTGPARSER_H
