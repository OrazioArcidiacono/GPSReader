#ifndef RECEIVERMODE_H
#define RECEIVERMODE_H

#include "GPSReceiverConfig.h"
#include "RawDataDialog.h"

class ReceiverMode {
public:
    virtual int start() = 0;      // Avvia la modalità e restituisce l’esito dell’applicazione (es. il valore restituito da QApplication::exec())
    virtual void cleanup() = 0;   // Esegue il cleanup dei componenti utilizzati
    virtual ~ReceiverMode() {}
};

#endif // RECEIVERMODE_H
