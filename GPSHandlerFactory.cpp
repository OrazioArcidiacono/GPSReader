#include "GPSHandlerFactory.h"
#include "StandaloneGPSHandler.h"
#include "PluginGPSHandler.h"

std::unique_ptr<IGPSHandler> GPSHandlerFactory::createHandler(const ReceiverConfig &config) {
    if (config.mode.toLower() == "plugin") {
        return std::make_unique<PluginGPSHandler>("/tmp/gps_fifo");
    } else {
        return std::make_unique<StandaloneGPSHandler>();
    }
}
