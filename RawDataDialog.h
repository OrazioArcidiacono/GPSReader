#ifndef RAWDATADIALOG_H
#define RAWDATADIALOG_H

#include <QDialog>
#include "GPSReader.h"  // Per la definizione di Coordinate

class QTableWidget;
class QTextEdit;
class QLabel;
class QPushButton;

struct NmeaSentenceInfo {
    QMap<int, QString> descriptions;  // Mappa: indice -> descrizione del campo
    int minFields;                    // Numero minimo di campi attesi per la frase
};

class RawDataDialog : public QDialog {
    Q_OBJECT
public:
    explicit RawDataDialog(QWidget *parent = nullptr);

public slots:
    // Aggiunge una riga nella tabella dei dati grezzi, includendo il Block ID
    void addRawLogEntry(int blockId, const QString &timestamp, const QString &rawNMEA, const QString &sentenceType);
    // Aggiorna l'area dei dati elaborati (coordinate)
    void updateProcessedData(const Coordinate &coord);
    // Aggiorna l'area dello stato
    void updateStatus(const QString &status);

signals:
    // Segnali per i controlli, da collegare al GPSReader
    void pauseRequested();
    void resumeRequested();
    void restartRequested();

private slots:
    // Slot chiamato quando viene selezionata una riga nella tabella dei dati grezzi
    void onRawTableRowSelected(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    void setupLegend();
    void setupControls();

    QLabel *m_legendLabel;
    QTableWidget *m_rawTable;
    QTableWidget *m_fieldsTable;
    QTextEdit *m_processedTextEdit;
    QTextEdit *m_statusTextEdit;
    QPushButton *m_pauseButton;
    QPushButton *m_resumeButton;
    QPushButton *m_restartButton;
};

#endif // RAWDATADIALOG_H
