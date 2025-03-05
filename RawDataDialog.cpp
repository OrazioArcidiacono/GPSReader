#include "RawDataDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QDateTime>
#include <QMap>
#include <QStringList>
#include <QDebug>

// Definizione delle mappe di descrizione per le principali frasi NMEA

static const NmeaSentenceInfo gprmcInfo = {
    {
        {0, "Sentence ID ($GPRMC)"},
        {1, "UTC Time (hhmmss.sss)"},
        {2, "Status (A=valid, V=invalid)"},
        {3, "Latitude (ddmm.mmmm)"},
        {4, "Lat Direction (N/S)"},
        {5, "Longitude (dddmm.mmmm)"},
        {6, "Lon Direction (E/W)"},
        {7, "Speed over ground (knots)"},
        {8, "Track angle (degrees)"},
        {9, "Date (ddmmyy)"},
        {10, "Magnetic variation"},
        {11, "Magnetic variation E/W"}
    },
    12
};

static const NmeaSentenceInfo gpggaInfo = {
    {
        {0, "Sentence ID ($GPGGA)"},
        {1, "UTC Time (hhmmss.sss)"},
        {2, "Latitude (ddmm.mmmm)"},
        {3, "Lat Direction (N/S)"},
        {4, "Longitude (dddmm.mmmm)"},
        {5, "Lon Direction (E/W)"},
        {6, "Fix Quality (0=invalid,1=GPS,2=DGPS,...)"},
        {7, "Number of Satellites"},
        {8, "HDOP"},
        {9, "Altitude (m)"},
        {10, "Altitude Units"},
        {11, "Geoidal Separation (m)"},
        {12, "Separation Units"},
        {13, "Age of DGPS data"},
        {14, "DGPS Station ID"}
    },
    15
};

static const NmeaSentenceInfo gpgsaInfo = {
    {
        {0, "Sentence ID ($GPGSA)"},
        {1, "Mode (A=Auto, M=Manual)"},
        {2, "Fix type (1=No fix, 2=2D, 3=3D)"},
        {15, "PDOP"},
        {16, "HDOP"},
        {17, "VDOP"}
    },
    18
};

static const NmeaSentenceInfo gpgsvInfo = {
    {
        {0, "Sentence ID ($GPGSV)"},
        {1, "Total messages"},
        {2, "Message number"},
        {3, "Satellites in view"}
        // I campi successivi vengono ripetuti per ogni satellite
    },
    20
};

static const NmeaSentenceInfo gpvtgInfo = {
    {
        {0, "Sentence ID ($GPVTG)"},
        {1, "Track True (degrees)"},
        {2, "T (literal)"},
        {3, "Track Magnetic (degrees)"},
        {4, "M (literal)"},
        {5, "Speed over ground (knots)"},
        {6, "N (literal)"},
        {7, "Speed over ground (km/h)"},
        {8, "K (literal)"},
        {9, "Mode (A, D, E, N, etc.)"}
    },
    10
};

static const NmeaSentenceInfo gpgllInfo = {
    {
        {0, "Sentence ID ($GPGLL)"},
        {1, "Latitude (ddmm.mmmm)"},
        {2, "Lat Direction (N/S)"},
        {3, "Longitude (dddmm.mmmm)"},
        {4, "Lon Direction (E/W)"},
        {5, "UTC Time (hhmmss.sss)"},
        {6, "Status (A=valid, V=invalid)"},
        {7, "Mode (A=Autonomous, D=DGPS, etc.)"}
    },
    8
};

static const NmeaSentenceInfo gptxtInfo = {
    {
        {0, "Sentence ID ($GPTXT)"},
        {1, "Total messages"},
        {2, "Message number"},
        {3, "Message type"}
        // Da 4 in poi: testo del messaggio
    },
    4
};

static const NmeaSentenceInfo* getSentenceInfo(const QString &type)
{
    if (type == "GPRMC") return &gprmcInfo;
    if (type == "GPGGA") return &gpggaInfo;
    if (type == "GPGSA") return &gpgsaInfo;
    if (type == "GPGSV") return &gpgsvInfo;
    if (type == "GPVTG") return &gpvtgInfo;
    if (type == "GPGLL") return &gpgllInfo;
    if (type == "GPTXT") return &gptxtInfo;
    return nullptr;
}

RawDataDialog::RawDataDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("GPS Data Viewer");

    m_legendLabel = new QLabel(this);
    m_legendLabel->setWordWrap(true);
    setupLegend();

    // Tabella per le frasi NMEA con 4 colonne: Block ID, Timestamp, Raw NMEA, Sentence Type
    m_rawTable = new QTableWidget(this);
    m_rawTable->setColumnCount(4);
    m_rawTable->setHorizontalHeaderLabels(QStringList() << "Block ID" << "Timestamp" << "Raw NMEA" << "Sentence Type");
    m_rawTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_rawTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_rawTable->setSelectionMode(QAbstractItemView::SingleSelection);

    // Tabella per il dettaglio dei campi
    m_fieldsTable = new QTableWidget(this);
    m_fieldsTable->setColumnCount(3);
    m_fieldsTable->setHorizontalHeaderLabels(QStringList() << "Field #" << "Value" << "Description");
    m_fieldsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    connect(m_rawTable, &QTableWidget::currentCellChanged,
            this, &RawDataDialog::onRawTableRowSelected);

    QSplitter *tablesSplitter = new QSplitter(Qt::Horizontal, this);
    tablesSplitter->addWidget(m_rawTable);
    tablesSplitter->addWidget(m_fieldsTable);

    // Area per i dati elaborati (coordinate)
    m_processedTextEdit = new QTextEdit(this);
    m_processedTextEdit->setReadOnly(true);
    m_processedTextEdit->setPlaceholderText("Dati elaborati (coordinate)...");

    // Area per lo stato
    m_statusTextEdit = new QTextEdit(this);
    m_statusTextEdit->setReadOnly(true);
    m_statusTextEdit->setPlaceholderText("Stato...");

    // Pannello di controllo
    m_pauseButton = new QPushButton("Pausa", this);
    m_resumeButton = new QPushButton("Riprendi", this);
    m_restartButton = new QPushButton("Riavvia Porta", this);
    // Imposta dimensioni minime per rendere visibili i testi
    m_pauseButton->setMinimumSize(80, 30);
    m_resumeButton->setMinimumSize(80, 30);
    m_restartButton->setMinimumSize(100, 30);
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(m_pauseButton);
    controlLayout->addWidget(m_resumeButton);
    controlLayout->addWidget(m_restartButton);
    QWidget *controlWidget = new QWidget(this);
    controlWidget->setLayout(controlLayout);
    setupControls();

    // Splitter verticale per separare le tabelle dalla parte inferiore
    QSplitter *verticalSplitter = new QSplitter(Qt::Vertical, this);
    QWidget *topWidget = new QWidget(this);
    QVBoxLayout *topLayout = new QVBoxLayout(topWidget);
    topLayout->addWidget(tablesSplitter);
    topWidget->setLayout(topLayout);

    QWidget *bottomWidget = new QWidget(this);
    QVBoxLayout *bottomLayout = new QVBoxLayout(bottomWidget);
    bottomLayout->addWidget(m_processedTextEdit);
    bottomLayout->addWidget(m_statusTextEdit);
    bottomWidget->setLayout(bottomLayout);

    verticalSplitter->addWidget(topWidget);
    verticalSplitter->addWidget(bottomWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_legendLabel);
    mainLayout->addWidget(verticalSplitter);
    mainLayout->addWidget(controlWidget);
    setLayout(mainLayout);

    resize(1000, 700);
    show();
}

void RawDataDialog::setupLegend()
{
    m_legendLabel->setText(
        "<b>Legenda:</b><br>"
        "<b>Dati Grezzi:</b> La tabella a sinistra mostra, per ogni frase NMEA, il Block ID, il Timestamp, la frase completa e il Sentence Type.<br>"
        "<b>Dettaglio Campi:</b> Selezionando una riga, la tabella a destra mostra il dettaglio di ogni campo (Indice, Valore, Descrizione).<br>"
        "<b>Dati Elaborati:</b> L'area a destra mostra le coordinate elaborate (in formato decimale).<br>"
        "<b>Stato:</b> Mostra i messaggi sullo stato corrente del modulo GPS.<br>"
        "<b>Controlli:</b> I pulsanti in basso permettono di mettere in pausa, riprendere o riavviare la porta seriale.<br><br>"
        "<b>Esempi di Sigle NMEA:</b><br>"
        "$GPRMC: Dati minimi (orario, stato, lat, lon, velocità, data, ecc.)<br>"
        "$GPGGA: Dati di fissaggio (orario, lat, lon, qualità, satelliti, altitudine, ecc.)<br>"
        "$GPGSA: Fix e DOP<br>"
        "$GPGSV: Informazioni sui satelliti in vista<br>"
        "$GPVTG: Velocità e direzione a terra<br>"
        "$GPGLL: Coordinate geografiche<br>"
        "$GPTXT: Messaggi testuali"
        );
}

void RawDataDialog::setupControls()
{
    connect(m_pauseButton, &QPushButton::clicked, this, [this]() { emit pauseRequested(); });
    connect(m_resumeButton, &QPushButton::clicked, this, [this]() { emit resumeRequested(); });
    connect(m_restartButton, &QPushButton::clicked, this, [this]() { emit restartRequested(); });
}

void RawDataDialog::addRawLogEntry(int blockId, const QString &timestamp, const QString &rawNMEA, const QString &sentenceType)
{
    int row = m_rawTable->rowCount();
    m_rawTable->insertRow(row);

    m_rawTable->setItem(row, 0, new QTableWidgetItem(QString::number(blockId)));
    m_rawTable->setItem(row, 1, new QTableWidgetItem(timestamp));

    QString displayStr = rawNMEA;
    displayStr.remove('\r');
    displayStr.remove('\n');
    m_rawTable->setItem(row, 2, new QTableWidgetItem(displayStr));
    m_rawTable->setItem(row, 3, new QTableWidgetItem(sentenceType));
}

void RawDataDialog::updateProcessedData(const Coordinate &coord)
{
    QString text = QString("Lat: %1, Lon: %2")
                       .arg(coord.latitude, 0, 'f', 6)
                       .arg(coord.longitude, 0, 'f', 6);
    m_processedTextEdit->append(text);
    qDebug() << Q_FUNC_INFO << "Dati elaborati aggiornati:" << text;
}

void RawDataDialog::updateStatus(const QString &status)
{
    m_statusTextEdit->append("Stato: " + status);
    qDebug() << Q_FUNC_INFO << "Stato aggiornato:" << status;
}

void RawDataDialog::onRawTableRowSelected(int currentRow, int currentColumn,
                                          int previousRow, int previousColumn)
{
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if (currentRow < 0) {
        m_fieldsTable->setRowCount(0);
        return;
    }

    // Leggi la frase NMEA dalla colonna "Raw NMEA" (colonna 2)
    QString rawNMEA = m_rawTable->item(currentRow, 2)->text();
    m_fieldsTable->setRowCount(0);

    QString trimmed = rawNMEA.trimmed();
    int starIndex = trimmed.indexOf('*');
    QString withoutChecksum = (starIndex > 0) ? trimmed.left(starIndex) : trimmed;

    QStringList fields = withoutChecksum.split(',');
    if (fields.isEmpty())
        return;

    QString sentenceType = fields[0].mid(1); // Rimuovi il '$'
    const NmeaSentenceInfo* info = getSentenceInfo(sentenceType);
    int minFields = info ? info->minFields : 0;
    while (minFields > 0 && fields.size() < minFields) {
        fields.append("");
    }

    for (int i = 0; i < fields.size(); ++i) {
        int row = m_fieldsTable->rowCount();
        m_fieldsTable->insertRow(row);
        m_fieldsTable->setItem(row, 0, new QTableWidgetItem(QString::number(i)));
        m_fieldsTable->setItem(row, 1, new QTableWidgetItem(fields[i]));

        QString desc;
        if (info) {
            if (info->descriptions.contains(i))
                desc = info->descriptions.value(i);
            else
                desc = QString("Campo #%1 extra").arg(i);
        } else {
            if (i == 0)
                desc = QString("Sentence ID (%1)").arg(fields[i]);
            else
                desc = QString("Campo #%1 generico").arg(i);
        }
        m_fieldsTable->setItem(row, 2, new QTableWidgetItem(desc));
    }
}
