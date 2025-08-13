#include "playlistui.h"
#include "addplaylistsongs.h"
#include <QFont>

PlaylistUI::PlaylistUI(const QString &playlistName, const QString &username, QWidget *parent)
    : QWidget(parent), playlistName(playlistName), username(username)
{
    setStyleSheet("background-color: #191414; color: white;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // Cabecera
    QHBoxLayout *headerLayout = new QHBoxLayout;

    coverLabel = new QLabel;
    coverLabel->setFixedSize(150, 150);
    coverLabel->setStyleSheet("background-color: #333; border-radius: 8px;");
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setText("Cover");
    headerLayout->addWidget(coverLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    QLabel *playlistType = new QLabel("Public Playlist");
    playlistType->setStyleSheet("color: #bbb; font-size: 10pt;");
    infoLayout->addWidget(playlistType);

    titleLabel = new QLabel(playlistName);
    QFont titleFont;
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    infoLayout->addWidget(titleLabel);

    infoLayout->addSpacing(10);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    playButton = new QPushButton("▶");
    playButton->setFixedSize(50, 50);
    playButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: black; border-radius: 25px; font-size: 20pt; }"
        "QPushButton:hover { background-color: #1DB954; }"
        );
    buttonLayout->addWidget(playButton);

    addSongButton = new QPushButton("+");
    addSongButton->setFixedSize(40, 40);
    addSongButton->setStyleSheet(
        "QPushButton { background-color: #282828; color: white; border-radius: 20px; font-size: 18pt; }"
        "QPushButton:hover { background-color: #333; }"
        );
    buttonLayout->addWidget(addSongButton);

    // 🔹 Conectar botón "+" a ventana de agregar canciones
    connect(addSongButton, &QPushButton::clicked, this, [=]() {
        AddPlaylistSongs *addWindow = new AddPlaylistSongs(username, playlistName, this);
        addWindow->setAttribute(Qt::WA_DeleteOnClose);
        addWindow->show();
    });

    infoLayout->addLayout(buttonLayout);
    headerLayout->addLayout(infoLayout);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    // Scroll para canciones
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    QWidget *songsWidget = new QWidget;
    QVBoxLayout *songsLayout = new QVBoxLayout(songsWidget);
    songsLayout->setContentsMargins(0, 0, 0, 0);
    songsLayout->setSpacing(8);

    QLabel *placeholder = new QLabel("No songs yet");
    placeholder->setStyleSheet("color: #aaa; font-size: 12pt;");
    songsLayout->addWidget(placeholder);

    scroll->setWidget(songsWidget);
    mainLayout->addWidget(scroll, 1);
}


