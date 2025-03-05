#ifndef GPSDISPLAYDIALOG_H
#define GPSDISPLAYDIALOG_H

#include <QDialog>
#include "GPSReader.h"  // Per la definizione di Coordinate

class QLabel;

class GPSDisplayDialog : public QDialog {
    Q_OBJECT
public:
    explicit GPSDisplayDialog(QWidget *parent = nullptr);
public slots:
    void updateCoordinates(const Coordinate &coord);
    void updateStatus(const QString &status);
private:
    QLabel *m_label;
    QLabel *m_statusLabel;
};

#endif // GPSDISPLAYDIALOG_H
