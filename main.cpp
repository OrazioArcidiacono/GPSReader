#include <signal.h>
#include <QApplication>
#include <QCommandLineParser>
#include "GPSReceiverConfig.h"
#include "RawDataDialog.h"
#include "ReceiverModeFactory.h"
#include <QDebug>

int main(int argc, char *argv[]) {
    // Ignora SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    QApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("GPS Receiver Application with Detailed NMEA Explorer");
    parser.addHelpOption();
    QCommandLineOption channelOption(QStringList() << "channel",
                                     "Specifica il canale di comunicazione (standalone, fifo_read, fifo_write, socket).",
                                     "channel");
    parser.addOption(channelOption);
    parser.process(app);

    QString channelParam = parser.value(channelOption).toLower();

    ReceiverConfig config = GPSReceiverConfig::loadConfig("/Users/orazioarcidiacono/GPS_Receiver/", "gpsconfig.db");
    qDebug() << "Configurazione DB:" << config.mode << config.pluginAction << config.port << config.baudRate;

    if (!channelParam.isEmpty()) {
        if (channelParam == "standalone")
            config.mode = "standalone";
        else {
            config.mode = "plugin";
            config.pluginAction = channelParam;
        }
        qDebug() << "Parametro da linea di comando --channel impostato su:" << channelParam;
    }

    RawDataDialog displayDialog;

    ReceiverMode *mode = ReceiverModeFactory::createMode(config, displayDialog);
    if (!mode) {
        qWarning() << "Modalità non riconosciuta o non implementata.";
            return 1;
    }

    int ret = mode->start();
    mode->cleanup();
    delete mode;
    return ret;
}
