#ifndef GPSHANDLERFACTORY_H
#define GPSHANDLERFACTORY_H

#include "IGPSHandler.h"
#include "GPSReceiverConfig.h"
#include <memory>

class GPSHandlerFactory {
public:
    static std::unique_ptr<IGPSHandler> createHandler(const ReceiverConfig &config);
};

#endif // GPSHANDLERFACTORY_H
