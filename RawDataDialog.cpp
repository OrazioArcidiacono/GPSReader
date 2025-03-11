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
#include <QDebug>
#include <QTimer>
#include <QScrollBar>

RawDataDialog::RawDataDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("GPS Data Viewer");
    // Setup legend label.
    m_legendLabel = new QLabel(this);
    m_legendLabel->setWordWrap(true);
    setupLegend();
    // Setup raw data table.

    m_rawTable = new QTableWidget(this);
    m_rawTable->setColumnCount(4);
    m_rawTable->setHorizontalHeaderLabels(QStringList() << "Block ID" << "Timestamp" << "Raw NMEA" << "Sentence Type");
    m_rawTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    m_rawTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_rawTable->setSelectionMode(QAbstractItemView::SingleSelection);
    // Setup fields (details) table.

    m_autoScrollEnabled = true;
    m_autoScrollTimer = new QTimer(this);
    m_autoScrollTimer->setSingleShot(true);
    m_autoScrollTimer->setInterval(10000); // 10 secondi
    connect(m_autoScrollTimer, &QTimer::timeout, this, [this]() {
        m_autoScrollEnabled = true;
        m_rawTable->scrollToBottom();
    });

    connect(m_rawTable->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int value) {
        if (value != m_rawTable->verticalScrollBar()->maximum()) {
            // L'utente ha spostato la scrollbar manualmente:
            m_autoScrollEnabled = false;
            m_autoScrollTimer->start();  // Riavvia il timer per 10 secondi.
        } else {
            // Se l'utente arriva in fondo, auto-scroll si riabilita immediatamente.
            m_autoScrollEnabled = true;
        }
    });

    m_fieldsTable = new QTableWidget(this);
    m_fieldsTable->setColumnCount(3);
    m_fieldsTable->setHorizontalHeaderLabels(QStringList() << "Key" << "Value" << "Description");
    m_fieldsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
// Connect row selection to update detailed view.
    connect(m_rawTable, &QTableWidget::currentCellChanged,
            this, &RawDataDialog::onRawTableRowSelected);

    QSplitter *tablesSplitter = new QSplitter(Qt::Horizontal, this);
    tablesSplitter->addWidget(m_rawTable);
    tablesSplitter->addWidget(m_fieldsTable);
    // Setup processed data area.

    m_processedTextEdit = new QTextEdit(this);
    m_processedTextEdit->setReadOnly(true);
    m_processedTextEdit->setPlaceholderText("Dati elaborati (coordinate)...");
    // Setup status area.

    m_statusTextEdit = new QTextEdit(this);
    m_statusTextEdit->setReadOnly(true);
    m_statusTextEdit->setPlaceholderText("Stato...");
    // Setup control buttons.

    m_pauseButton = new QPushButton("Pausa", this);
    m_resumeButton = new QPushButton("Riprendi", this);
    m_restartButton = new QPushButton("Riavvia Porta", this);
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

void RawDataDialog::setupLegend() {
    m_legendLabel->setText(
        "<b>Legenda:</b><br>"
        "<b>Dati Grezzi:</b> La tabella a sinistra mostra, per ogni frase NMEA, il Block ID, il Timestamp, la frase completa e il Sentence Type.<br>"
        "<b>Dettaglio Campi Parsati:</b> Cliccando una riga, la tabella a destra mostrerà i campi (Key/Value) ottenuti dal parsing completo della frase.<br>"
        "<b>Dati Elaborati:</b> L'area a destra mostra le coordinate elaborate (in formato decimale).<br>"
        "<b>Stato:</b> Mostra i messaggi sullo stato corrente del modulo GPS.<br>"
        "<b>Controlli:</b> I pulsanti in basso permettono di mettere in pausa, riprendere o riavviare la porta seriale."
        );
}

void RawDataDialog::setupControls() {
    connect(m_pauseButton, &QPushButton::clicked, this, [this]() { emit pauseRequested(); }); //signal-slot con lambda
    connect(m_resumeButton, &QPushButton::clicked, this, [this]() { emit resumeRequested(); });
    connect(m_restartButton, &QPushButton::clicked, this, [this]() { emit restartRequested(); });
}

void RawDataDialog::addRawLogEntry(int blockId, const QString &timestamp, const QString &rawNMEA, const QString &sentenceType) {
    int row = m_rawTable->rowCount();
    m_rawTable->insertRow(row);

    m_rawTable->setItem(row, 0, new QTableWidgetItem(QString::number(blockId)));
    m_rawTable->setItem(row, 1, new QTableWidgetItem(timestamp));

    QString displayStr = rawNMEA;
    displayStr.remove('\r');
    displayStr.remove('\n');
    m_rawTable->setItem(row, 2, new QTableWidgetItem(displayStr));
    m_rawTable->setItem(row, 3, new QTableWidgetItem(sentenceType));

    // Se l'auto-scroll è abilitato, scorri automaticamente in fondo.
    if (m_autoScrollEnabled)
        m_rawTable->scrollToBottom();
}

void RawDataDialog::updateProcessedData(const Coordinate &coord) {
    QString text = QString("Lat: %1, Lon: %2")
                       .arg(coord.latitude, 0, 'f', 6)
                       .arg(coord.longitude, 0, 'f', 6);
    m_processedTextEdit->append(text);
    qDebug() << Q_FUNC_INFO << "Dati elaborati aggiornati:" << text;
}

void RawDataDialog::updateStatus(const QString &status) {
    m_statusTextEdit->append("Stato: " + status);
    qDebug() << Q_FUNC_INFO << "Stato aggiornato:" << status;
}

void RawDataDialog::updateParsedData(const NMEAParser::ParsedData &parsedData) {
    // Questo slot non viene chiamato automaticamente per non sovrascrivere il dettaglio.
    Q_UNUSED(parsedData)
}

void RawDataDialog::onRawTableRowSelected(int currentRow, int currentColumn, int previousRow, int previousColumn) {
    Q_UNUSED(currentColumn)
    Q_UNUSED(previousRow)
    Q_UNUSED(previousColumn)

    if (currentRow < 0) {
        m_fieldsTable->setRowCount(0);
        return;
    }

    // Recupera la frase NMEA dalla colonna 2 (Raw NMEA)
    QString rawSentence = m_rawTable->item(currentRow, 2)->text();
    // Usa il parser centralizzato per ottenere i dati strutturati completi
    NMEAParser::ParsedData parsed = NMEAParser::parse(rawSentence);

    // Pulisce la tabella dei dettagli e la popola con le coppie chiave/valore complete
    m_fieldsTable->setRowCount(0);
    QMapIterator<QString, QVariant> i(parsed.fields);
    while (i.hasNext()) {
        i.next();
        int row = m_fieldsTable->rowCount();
        m_fieldsTable->insertRow(row);
        m_fieldsTable->setItem(row, 0, new QTableWidgetItem(i.key()));
        m_fieldsTable->setItem(row, 1, new QTableWidgetItem(i.value().toString()));
        m_fieldsTable->setItem(row, 2, new QTableWidgetItem(""));
    }

    // Se ci sono errori nel parsing, li segnala nello status
    if (!parsed.valid) {
        m_statusTextEdit->append("Parsing error: " + parsed.errorMessage);
    }
}
