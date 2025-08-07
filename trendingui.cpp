#include "trendingui.h"
#include <QVBoxLayout>
#include <QScrollArea>
#include <QFont>
#include <QPushButton>
#include <QStyle>

TrendingUI::TrendingUI(QWidget *parent) : QDialog(parent)
{
    setStyleSheet("background-color: #191414; color: white;");
    setMinimumSize(900, 700);
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    setWindowTitle("Trending - Musicfy");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(40, 30, 30, 22);
    mainLayout->setSpacing(50);

    // --------- Top Songs Title ----------
    topSongsLabel = new QLabel("Trending Songs");
    QFont topFont = topSongsLabel->font();
    topFont.setPointSize(32);
    topFont.setBold(true);
    topSongsLabel->setFont(topFont);
    topSongsLabel->setStyleSheet("color: white; margin-bottom: 18px;");
    mainLayout->addWidget(topSongsLabel, 0, Qt::AlignLeft);

    // --------- Top Songs List ----------
    topSongsList = new QListWidget;
    topSongsList->setStyleSheet(R"(
        QListWidget {
            background: #191414;
            color: white;
            border: none;
            font-size: 20px;
        }
        QListWidget::item {
            border-bottom: 1px solid #282828;
            min-height: 48px;
            padding-left: 8px;
        }
        QListWidget::item:selected {
            background: #232323;
            color: #1ED760;
        }
    )");
    for (int i = 0; i < 4; ++i)
        topSongsList->addItem(QString("Es ejemplo Ingee %1").arg(i + 1));
    mainLayout->addWidget(topSongsList);

    // --------- Albums Title ----------
    albumsLabel = new QLabel("Trending Artists");
    QFont albumsFont = albumsLabel->font();
    albumsFont.setPointSize(32);
    albumsFont.setBold(true);
    albumsLabel->setFont(albumsFont);
    albumsLabel->setStyleSheet("color: white; margin-top: 38px; margin-bottom: 16px;");
    mainLayout->addWidget(albumsLabel, 0, Qt::AlignLeft);

    // --------- Albums/Artists Cards Row ----------
    QWidget *albumsWidget = new QWidget;
    QHBoxLayout *albumsLayout = new QHBoxLayout(albumsWidget);
    albumsLayout->setSpacing(22);
    albumsLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < 6; ++i) {
        QLabel *albumCard = new QLabel;
        albumCard->setFixedSize(165, 165);
        albumCard->setStyleSheet("background: #222; border-radius: 18px;");
        albumsLayout->addWidget(albumCard);
    }

    QScrollArea *albumsScroll = new QScrollArea;
    albumsScroll->setWidget(albumsWidget);
    albumsScroll->setWidgetResizable(true);
    albumsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    albumsScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    albumsScroll->setFixedHeight(185);
    albumsScroll->setStyleSheet("background: transparent; border: none;");

    mainLayout->addWidget(albumsScroll);
    mainLayout->addStretch();

    // Opcional: centrar en pantalla y remover borde si lo deseas
    setModal(true);
    setWindowModality(Qt::ApplicationModal);
}
