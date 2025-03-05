#include "GPSReceiverConfig.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

ReceiverConfig GPSReceiverConfig::loadConfig(const QString &dbPath, const QString &dbName)
{
    ReceiverConfig config;
    // Valori di default (fallback)
    config.mode = "standalone";
    config.pluginAction = "";
    config.port = "/dev/tty.usbmodem14101";
    config.baudRate = 9600;

    // Normalizza il percorso (aggiunge "/" se necessario)
    QString normalizedPath = dbPath;
    if (!normalizedPath.endsWith("/") && !normalizedPath.endsWith("\\")) {
        normalizedPath += "/";
    }
    QString fullDbPath = normalizedPath + dbName;

    QFileInfo dbFile(fullDbPath);
    qDebug() << Q_FUNC_INFO << "DB richiesto:" << fullDbPath;
    qDebug() << Q_FUNC_INFO << "Percorso assoluto:" << dbFile.absoluteFilePath();
    qDebug() << Q_FUNC_INFO << "Il file esiste?" << dbFile.exists();
    qDebug() << Q_FUNC_INFO << "Current working directory:" << QDir::currentPath();

    // Creazione di una connessione unica al DB
    const QString connectionName = "config_connection";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(fullDbPath);

    if (!db.open()) {
        qWarning() << Q_FUNC_INFO << "Impossibile aprire il DB:" << db.lastError().text();
        return config;
    }
    qDebug() << Q_FUNC_INFO << "DB aperto correttamente.";

    QStringList tables = db.tables();
    qDebug() << Q_FUNC_INFO << "Tabelle presenti nel DB:" << tables;

    // (Opzionale) Stampa la struttura della tabella ReceiverConfig
    QSqlQuery pragmaQuery(db);
    if (pragmaQuery.exec("PRAGMA table_info(ReceiverConfig)")) {
        qDebug() << Q_FUNC_INFO << "Struttura della tabella 'ReceiverConfig':";
        while (pragmaQuery.next()) {
            qDebug() << Q_FUNC_INFO
                     << "Colonna:" << pragmaQuery.value("name").toString()
                     << "Tipo:" << pragmaQuery.value("type").toString();
        }
    } else {
        qDebug() << Q_FUNC_INFO << "PRAGMA table_info fallita:" << pragmaQuery.lastError().text();
    }

    // Esegue la query per caricare la configurazione
    QSqlQuery query(db);
    QString sql = "SELECT mode, pluginAction, port, baudRate FROM ReceiverConfig LIMIT 1";
    qDebug() << Q_FUNC_INFO << "Esecuzione query:" << sql;
    if (query.exec(sql)) {
        qDebug() << Q_FUNC_INFO << "Query eseguita con successo.";
        if (query.next()) {
            QString mode = query.value(0).toString();
            QString pluginAction = query.value(1).toString();
            QString port = query.value(2).toString();
            int baudRate = query.value(3).toInt();
            qDebug() << Q_FUNC_INFO << "Risultati dalla query:"
                     << "mode =" << mode
                     << "pluginAction =" << pluginAction
                     << "port =" << port
                     << "baudRate =" << baudRate;
            config.mode = std::move(mode);
            config.pluginAction = std::move(pluginAction);
            config.port = std::move(port);
            config.baudRate = baudRate;
        } else {
            qWarning() << Q_FUNC_INFO << "La query non ha restituito alcun record.";
        }
    } else {
        qWarning() << Q_FUNC_INFO << "Errore nell'esecuzione della query:" << query.lastError().text();
    }

    db.close();
    QSqlDatabase::removeDatabase(connectionName);

    qDebug() << Q_FUNC_INFO << "Configurazione finale caricata:"
             << config.mode << config.pluginAction << config.port << config.baudRate;
    return config;
}
