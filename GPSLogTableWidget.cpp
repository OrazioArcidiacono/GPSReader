#include "GPSLogTableWidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QStringList>

GPSLogTableWidget::GPSLogTableWidget(QWidget *parent)
    : QWidget(parent)
{
    // Definiamo 22 colonne per il log
    m_tableWidget = new QTableWidget(this);
    m_tableWidget->setColumnCount(22);
    QStringList headers;
    headers << "Timestamp"             // Data e ora del record
            << "Raw NMEA"              // Frase NMEA grezza
            << "Sentence Type"         // Tipo di frase (es. GPRMC, GPGGA, ecc.)
            << "Lat Raw"               // Valore grezzo della latitudine (es. 3805.03241)
            << "Lat Dir"               // Direzione della latitudine (N/S)
            << "Lon Raw"               // Valore grezzo della longitudine (es. 01538.10556)
            << "Lon Dir"               // Direzione della longitudine (E/W)
            << "Processed Lat"         // Latitudine elaborata in gradi decimali
            << "Processed Lon"         // Longitudine elaborata in gradi decimali
            << "Speed (knots)"         // Velocità in nodi
            << "Track Angle"           // Angolo di direzione (gradi)
            << "Fix Quality"           // Qualità del fix (0 = non valido, 1 = GPS, ecc.)
            << "Satellites"            // Numero di satelliti usati per il fix
            << "HDOP"                  // Horizontal Dilution Of Precision
            << "Altitude (m)"          // Altitudine in metri
            << "Geoidal Sep (m)"       // Separazione geoidale in metri
            << "PDOP"                  // Positional DOP
            << "HDOP2"                 // Horizontal DOP (se disponibile)
            << "VDOP"                  // Vertical DOP (se disponibile)
            << "Sat in View"           // Numero di satelliti in vista
            << "Status"                // Messaggi di stato (es. "Lock acquisito")
            << "Additional Info";      // Eventuali note aggiuntive
    m_tableWidget->setHorizontalHeaderLabels(headers);
    m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_tableWidget);
    setLayout(layout);
}

void GPSLogTableWidget::addLogEntry(const QString &timestamp,
                                    const QString &rawNMEA,
                                    const QString &sentenceType,
                                    const QString &latRaw,
                                    const QString &latDir,
                                    const QString &lonRaw,
                                    const QString &lonDir,
                                    double processedLatitude,
                                    double processedLongitude,
                                    double speedKnots,
                                    double trackAngle,
                                    int fixQuality,
                                    int numSatellites,
                                    double hdop,
                                    double altitude,
                                    double geoidalSeparation,
                                    double pdop,
                                    double hdop2,
                                    double vdop,
                                    int satInView,
                                    const QString &statusMessage,
                                    const QString &additionalInfo)
{
    int row = m_tableWidget->rowCount();
    m_tableWidget->insertRow(row);

    m_tableWidget->setItem(row, 0, new QTableWidgetItem(timestamp));
    m_tableWidget->setItem(row, 1, new QTableWidgetItem(rawNMEA));
    m_tableWidget->setItem(row, 2, new QTableWidgetItem(sentenceType));
    m_tableWidget->setItem(row, 3, new QTableWidgetItem(latRaw));
    m_tableWidget->setItem(row, 4, new QTableWidgetItem(latDir));
    m_tableWidget->setItem(row, 5, new QTableWidgetItem(lonRaw));
    m_tableWidget->setItem(row, 6, new QTableWidgetItem(lonDir));
    m_tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(processedLatitude, 'f', 6)));
    m_tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(processedLongitude, 'f', 6)));
    m_tableWidget->setItem(row, 9, new QTableWidgetItem(QString::number(speedKnots)));
    m_tableWidget->setItem(row, 10, new QTableWidgetItem(QString::number(trackAngle)));
    m_tableWidget->setItem(row, 11, new QTableWidgetItem(QString::number(fixQuality)));
    m_tableWidget->setItem(row, 12, new QTableWidgetItem(QString::number(numSatellites)));
    m_tableWidget->setItem(row, 13, new QTableWidgetItem(QString::number(hdop, 'f', 2)));
    m_tableWidget->setItem(row, 14, new QTableWidgetItem(QString::number(altitude, 'f', 2)));
    m_tableWidget->setItem(row, 15, new QTableWidgetItem(QString::number(geoidalSeparation, 'f', 2)));
    m_tableWidget->setItem(row, 16, new QTableWidgetItem(QString::number(pdop, 'f', 2)));
    m_tableWidget->setItem(row, 17, new QTableWidgetItem(QString::number(hdop2, 'f', 2)));
    m_tableWidget->setItem(row, 18, new QTableWidgetItem(QString::number(vdop, 'f', 2)));
    m_tableWidget->setItem(row, 19, new QTableWidgetItem(QString::number(satInView)));
    m_tableWidget->setItem(row, 20, new QTableWidgetItem(statusMessage));
    m_tableWidget->setItem(row, 21, new QTableWidgetItem(additionalInfo));
}
