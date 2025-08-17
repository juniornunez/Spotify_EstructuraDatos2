#include "HorizontalSongCard.h"
#include <QPixmap>
#include <QFont>
#include <QMouseEvent>
#include <QDebug>

HorizontalSongCard::HorizontalSongCard(int position,
                                       const QString &coverPath,
                                       const QString &title,
                                       const QString &artist,
                                       const QString &audioPath,
                                       QWidget *parent)
    : QWidget(parent),
    coverPath(coverPath),
    audioPath(audioPath)
{
    setFixedHeight(50);
    setStyleSheet("background-color: transparent; color: white;");

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 0, 8, 0);
    layout->setSpacing(15);

    // Posición
    posLabel = new QLabel(QString::number(position));
    posLabel->setFixedWidth(25);
    posLabel->setStyleSheet("color: #aaa; font-size: 14px;");
    layout->addWidget(posLabel, 0, Qt::AlignVCenter);

    // Cover
    coverLabel = new QLabel;
    coverLabel->setFixedSize(40, 40);
    QPixmap pix(coverPath);
    if (!pix.isNull()) {
        coverLabel->setPixmap(pix.scaled(40, 40, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }
    layout->addWidget(coverLabel);

    // Titulo + artista
    QVBoxLayout *textLayout = new QVBoxLayout;
    titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    artistLabel = new QLabel(artist);
    artistLabel->setStyleSheet("color: #bbb; font-size: 12px;");
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(artistLabel);
    layout->addLayout(textLayout, 1);
}

// === Doble clic sobre la card ===
void HorizontalSongCard::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Doble clic en canción:" << titleLabel->text();
        emit songDoubleClicked(coverPath,
                               titleLabel->text(),
                               artistLabel->text(),
                               audioPath);
    }
    QWidget::mouseDoubleClickEvent(event);
}



