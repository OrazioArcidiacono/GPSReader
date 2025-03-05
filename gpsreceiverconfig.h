#ifndef GPSRECEIVECONFIG_H
#define GPSRECEIVECONFIG_H

#include <QString>

// Struttura che contiene i parametri di configurazione
struct ReceiverConfig {
    QString mode;         // "standalone" oppure "plugin"
    QString pluginAction; // "fifo_read", "fifo_write", "socket" (se mode=="plugin")
    QString port;         // Porta seriale, es. "/dev/tty.usbmodem14101"
    int baudRate;         // Baud rate, es. 9600
};

class GPSReceiverConfig {
public:
    // Carica la configurazione dal DB combinando il percorso (dbPath) e il nome del file (dbName)
    static ReceiverConfig loadConfig(const QString &dbPath, const QString &dbName);
};

#endif // GPSRECEIVECONFIG_H
