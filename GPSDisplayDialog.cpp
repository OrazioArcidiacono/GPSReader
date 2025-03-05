#include "GPSDisplayDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>

GPSDisplayDialog::GPSDisplayDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Dati GPS: Grezzi e Processati");
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_statusLabel = new QLabel("Stato: In attesa...", this);
    layout->addWidget(m_statusLabel);

    m_label = new QLabel("Coordinate: N/D", this);
    layout->addWidget(m_label);

    setLayout(layout);
    resize(300, 120);
    show();
}

void GPSDisplayDialog::updateCoordinates(const Coordinate &coord)
{
    QString text = QString("Latitudine: %1, Longitudine: %2")
                       .arg(coord.latitude, 0, 'f', 6)
                       .arg(coord.longitude, 0, 'f', 6);
    m_label->setText(text);
    qDebug() << Q_FUNC_INFO << "Coordinate aggiornate:" << text;
}

void GPSDisplayDialog::updateStatus(const QString &status)
{
    m_statusLabel->setText("Stato: " + status);
    qDebug() << Q_FUNC_INFO << "Stato aggiornato:" << status;
}
