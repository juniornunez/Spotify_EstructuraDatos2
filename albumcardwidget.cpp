#include "albumcardwidget.h"
#include <QPixmap>
#include <QMouseEvent>

AlbumCardWidget::AlbumCardWidget(const QString &albumName,
                                 const QString &coverPath,
                                 const QString &artistName,
                                 QWidget *parent)
    : QWidget(parent),
    m_albumName(albumName),
    m_coverPath(coverPath),
    m_artistName(artistName)
{
    setStyleSheet("background-color: transparent; color: white;");

    coverLabel = new QLabel;
    coverLabel->setFixedSize(140, 140);
    coverLabel->setScaledContents(true);
    QPixmap pix(coverPath);
    if (!pix.isNull()) {
        coverLabel->setPixmap(pix.scaled(140, 140, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    }

    titleLabel = new QLabel(albumName);
    titleLabel->setStyleSheet("font-weight: bold;");
    titleLabel->setWordWrap(true);

    artistLabel = new QLabel(artistName);
    artistLabel->setStyleSheet("color: gray; font-size: 12px;");
    artistLabel->setWordWrap(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(4);
    layout->addWidget(coverLabel, 0, Qt::AlignCenter);
    layout->addWidget(titleLabel);
    layout->addWidget(artistLabel);

    setLayout(layout);
    setFixedWidth(150);
}


// --- Doble click ---
void AlbumCardWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Album doble clic:" << m_albumName;
        emit albumDoubleClicked(m_albumName, m_coverPath, m_artistName);
    }
    QWidget::mouseDoubleClickEvent(event);
}
