#ifndef RECEIVERMODEFACTORY_H
#define RECEIVERMODEFACTORY_H

#include "ReceiverMode.h"
#include "StandaloneMode.h"
#include "FifoWriteMode.h"
#include "FifoReadMode.h"
//#include "SocketMode.h" // da implementare in modo simile

class ReceiverModeFactory {
public:
    static ReceiverMode* createMode(const ReceiverConfig &config, RawDataDialog &displayDialog) {
        if (config.mode.toLower() == "standalone")
            return new StandaloneMode(config, displayDialog);
        else if (config.mode.toLower() == "plugin") {
            if (config.pluginAction.toLower() == "fifo_write")
                return new FifoWriteMode(config, displayDialog);
            else if (config.pluginAction.toLower() == "fifo_read")
                return new FifoReadMode("/tmp/gps_fifo", displayDialog);
            else if (config.pluginAction.toLower() == "socket") {
                // Implementare e restituire un SocketMode
            }
        }
        return nullptr;
    }
};

#endif // RECEIVERMODEFACTORY_H
