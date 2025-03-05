#ifndef GPSLOGTABLEWIDGET_H
#define GPSLOGTABLEWIDGET_H

#include <QWidget>
#include <QTableWidget>
#include "GPSReader.h"  // Per la definizione di Coordinate

class GPSLogTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GPSLogTableWidget(QWidget *parent = nullptr);
    // Aggiunge una riga al log con tutti i dettagli del dato GPS
    void addLogEntry(const QString &timestamp,
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
                     const QString &additionalInfo);
private:
    QTableWidget *m_tableWidget;
};

#endif // GPSLOGTABLEWIDGET_H
