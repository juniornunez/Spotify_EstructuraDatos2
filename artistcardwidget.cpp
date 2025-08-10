#include "artistcardwidget.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

ArtistCardWidget::ArtistCardWidget(const QString &artistName, const QString &profilePicPath, QWidget *parent)
    : QWidget(parent), artistName(artistName)
{
    setFixedSize(160, 200);
    setStyleSheet("background-color: #181818; border-radius: 10px;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->setSpacing(6);

    // Imagen del artista
    coverLabel = new QLabel;
    coverLabel->setFixedSize(144, 144);

    QPixmap pix(profilePicPath);
    if (!pix.isNull()) {
        QPixmap rounded(144, 144);
        rounded.fill(Qt::transparent);

        QPainter painter(&rounded);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 144, 144);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pix.scaled(144, 144, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        painter.end();

        coverLabel->setPixmap(rounded);
    } else {
        coverLabel->setStyleSheet("background-color: #333; border-radius: 72px;");
    }

    layout->addWidget(coverLabel, 0, Qt::AlignHCenter);

    // Nombre del artista
    nameLabel = new QLabel(artistName);
    nameLabel->setStyleSheet("color: white; font-weight: bold; font-size: 12pt;");
    nameLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(nameLabel);

    setLayout(layout);
}

void ArtistCardWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit doubleClicked(artistName);
    }
}
