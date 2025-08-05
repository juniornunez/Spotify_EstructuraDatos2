#include "songcardwidget.h"
#include <QPainter>
#include <QFont>
#include <QMouseEvent>
#include <QIcon>

SongCardWidget::SongCardWidget(const QString& coverPath,
                               const QString& title,
                               const QString& artist,
                               QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(180, 245);
    setStyleSheet("background: #191414; border-radius: 18px; border: none;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Cover image
    coverLabel = new QLabel(this);
    coverLabel->setFixedSize(160, 160);
    coverLabel->setStyleSheet("border-radius: 10px; background: #222;");
    setCover(coverPath);

    // Play button (oculto por defecto)
    playButton = new QPushButton(this);
    playButton->setFixedSize(54, 54);
    playButton->setIcon(QIcon(":/assets/playimage.png"));  // Ícono de play que quieres
    playButton->setIconSize(QSize(32, 32));               // Ajusta el tamaño según tu imagen
    playButton->setStyleSheet(
        "QPushButton {"
        "background: #1ED760;"
        "border: none;"
        "border-radius: 27px;"
        "box-shadow: 0px 4px 24px #000A;"
        "}"
        "QPushButton:hover { background: #1fdf64; }"
        );
    playButton->move((width() - playButton->width())/2, 26 + (160-54)/2);
    playButton->hide();


    connect(playButton, &QPushButton::clicked, this, [=]() {
        // En el futuro: código para reproducir
    });

    mainLayout->addWidget(coverLabel, 0, Qt::AlignHCenter);

    // Title label
    titleLabel = new QLabel(this);
    titleLabel->setWordWrap(true);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(13);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");
    setTitle(title);

    // Artist label
    artistLabel = new QLabel(this);
    QFont artistFont = artistLabel->font();
    artistFont.setPointSize(10);
    artistLabel->setFont(artistFont);
    artistLabel->setStyleSheet("color: #b3b3b3;");
    setArtist(artist);

    mainLayout->addWidget(titleLabel, 0, Qt::AlignLeft);
    mainLayout->addWidget(artistLabel, 0, Qt::AlignLeft);
    mainLayout->addStretch();
    setLayout(mainLayout);
}

void SongCardWidget::setCover(const QString& coverPath) {
    QPixmap pix(coverPath);
    if (!pix.isNull()) {
        coverLabel->setPixmap(pix.scaled(160, 160, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        coverLabel->setText("No image");
        coverLabel->setAlignment(Qt::AlignCenter);
        coverLabel->setStyleSheet("background: #333; color: #888; border-radius: 10px;");
    }
}

void SongCardWidget::setTitle(const QString& title) {
    titleLabel->setText(title);
}

void SongCardWidget::setArtist(const QString& artist) {
    artistLabel->setText(artist);
}

void SongCardWidget::mousePressEvent(QMouseEvent *event) {
    setSelected(!selected); // Toggle
    emit toggled(this, selected);
    QWidget::mousePressEvent(event);
}

void SongCardWidget::setSelected(bool s) {
    if (selected == s) return;
    selected = s;
    playButton->setVisible(selected);
    update();
}

bool SongCardWidget::isSelected() const {
    return selected;
}

void SongCardWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);
    if (selected) {
        QPainter painter(this);
        QPen pen(QColor("#1ED760"));
        pen.setWidth(3);
        painter.setPen(pen);
        painter.drawRoundedRect(rect().adjusted(1, 1, -2, -2), 18, 18);
    }
}
