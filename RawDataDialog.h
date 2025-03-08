#ifndef RAWDATADIALOG_H
#define RAWDATADIALOG_H

#include <QDialog>
#include "GPSReader.h"    // Per la definizione di Coordinate
#include "NMEAParser.h"   // Per ParsedData

class QTableWidget;
class QTextEdit;
class QLabel;
class QPushButton;

class RawDataDialog : public QDialog {
    Q_OBJECT
public:
    explicit RawDataDialog(QWidget *parent = nullptr);

public slots:
    void addRawLogEntry(int blockId, const QString &timestamp, const QString &rawNMEA, const QString &sentenceType);
    void updateProcessedData(const Coordinate &coord);
    void updateStatus(const QString &status);
    // Lo slot updateParsedData non viene chiamato automaticamente;
    // il dettaglio verrà aggiornato quando l'utente clicca su una riga.
    void updateParsedData(const NMEAParser::ParsedData &parsedData);

signals:
    void pauseRequested();
    void resumeRequested();
    void restartRequested();

private slots:
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
