#ifndef RAWDATADIALOG_H
#define RAWDATADIALOG_H

#include <QDialog>
#include "GPSReader.h"    // For Coordinate structure.
#include "NMEAParser.h"   // For ParsedData structure.
/**
 * @brief RawDataDialog provides the graphical user interface for displaying NMEA data.
 *
 * It shows a log of raw NMEA sentences and, upon selecting a row,
 * displays detailed parsed fields in a separate table.
 */
class QTableWidget;
class QTextEdit;
class QLabel;
class QPushButton;

class RawDataDialog : public QDialog {
    Q_OBJECT
public:
    /**
     * @brief Constructs the dialog.
     * @param parent The parent widget.
     */
    explicit RawDataDialog(QWidget *parent = nullptr);

public slots:
    /**
     * @brief Adds a new entry to the raw data log.
     * @param blockId The sequential block identifier.
     * @param timestamp The timestamp of reception.
     * @param rawNMEA The raw NMEA sentence.
     * @param sentenceType The detected sentence type.
     */
    void addRawLogEntry(int blockId, const QString &timestamp, const QString &rawNMEA, const QString &sentenceType);
    /**
     * @brief Updates the processed data area with a coordinate.
     * @param coord The processed GPS coordinate.
     */
    void updateProcessedData(const Coordinate &coord);
    /**
     * @brief Updates the status area with a message.
     * @param status The status message.
     */
    void updateStatus(const QString &status);
    /**
     * @brief (Not called automatically) Intended to update parsed data details.
     * @param parsedData The complete parsed data.
     */
    void updateParsedData(const NMEAParser::ParsedData &parsedData);

signals:

    void pauseRequested();
    void resumeRequested();
    void restartRequested();

private slots:
    /**
     * @brief Called when a row in the raw data table is selected.
     * Retrieves the raw NMEA sentence and updates the details table.
     */
    void onRawTableRowSelected(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    /**
     * @brief Sets up the legend label.
     */
    void setupLegend();
    /**
     * @brief Sets up the control buttons.
     */
    void setupControls();

    QLabel *m_legendLabel;           ///< Label for the legend.
    QTableWidget *m_rawTable;        ///< Table showing the raw NMEA sentences.
    QTableWidget *m_fieldsTable;     ///< Table showing detailed parsed fields.
    QTextEdit *m_processedTextEdit;  ///< Text area for processed coordinates.
    QTextEdit *m_statusTextEdit;     ///< Text area for status messages.
    QPushButton *m_pauseButton;      ///< Pause button.
    QPushButton *m_resumeButton;     ///< Resume button.
    QPushButton *m_restartButton;    ///< Restart port button.
};

#endif // RAWDATADIALOG_H
