#include "playlistdisplayui.h"
#include "horizontalsongcard.h"
#include "songdata.h"

#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QPixmap>
#include <QFont>
#include <QScrollArea>
#include <QPushButton>
#include <QDebug>

PlaylistDisplayUI::PlaylistDisplayUI(const QString &albumName,
                                     const QString &coverPath,
                                     const QString &artistName,
                                     const QString &adminUsername,
                                     QWidget *parent)
    : QWidget(parent),
    albumName(albumName),
    coverPath(coverPath),
    artistName(artistName),
    adminUsername(adminUsername),
    currentIndex(-1)   // Ninguna canción seleccionada al inicio
{
    setStyleSheet("background-color: #191414; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(25);

    // === HEADER ===
    QHBoxLayout *headerLayout = new QHBoxLayout;

    // Cover
    coverLabel = new QLabel;
    coverLabel->setFixedSize(200, 200);
    coverLabel->setScaledContents(true);
    QPixmap pix(coverPath);
    if (!pix.isNull()) {
        coverLabel->setPixmap(pix.scaled(200, 200, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        coverLabel->setStyleSheet("background:#333; color:#bbb; font-size:12px;");
        coverLabel->setText("Cover");
        coverLabel->setAlignment(Qt::AlignCenter);
    }
    headerLayout->addWidget(coverLabel);

    // Info del álbum
    QVBoxLayout *infoLayout = new QVBoxLayout;
    QLabel *subtitle = new QLabel("Public Playlist");
    subtitle->setStyleSheet("color: #bbb; font-size: 12px;");

    albumTitleLabel = new QLabel(albumName);
    QFont albumFont;
    albumFont.setPointSize(28);
    albumFont.setBold(true);
    albumTitleLabel->setFont(albumFont);

    artistLabel = new QLabel("by " + artistName);
    artistLabel->setStyleSheet("color: #bbb; font-size: 14px;");

    infoLayout->addWidget(subtitle);
    infoLayout->addWidget(albumTitleLabel);
    infoLayout->addWidget(artistLabel);
    infoLayout->addStretch();

    headerLayout->addLayout(infoLayout);
    mainLayout->addLayout(headerLayout);

    // === Botones (play y añadir) ===
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    QPushButton *playButton = new QPushButton;
    playButton->setFixedSize(60, 60);
    playButton->setStyleSheet("QPushButton { background-color: #1ED760; border-radius: 30px; }");
    playButton->setIcon(QIcon(":/icons/play.png")); // pon tu icono aquí
    playButton->setIconSize(QSize(40,40));

    QPushButton *addButton = new QPushButton("+");
    addButton->setFixedSize(45, 45);
    addButton->setStyleSheet("QPushButton { background-color: #333; color:white; border-radius: 22px; font-size:22px; }");

    buttonsLayout->addWidget(playButton);
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addStretch();

    mainLayout->addLayout(buttonsLayout);

    // === Lista de canciones ===
    QWidget *songsContainer = new QWidget;
    songsLayout = new QVBoxLayout(songsContainer);
    songsLayout->setContentsMargins(0, 0, 0, 0);
    songsLayout->setSpacing(6);

    QScrollArea *songsScroll = new QScrollArea;
    songsScroll->setWidget(songsContainer);
    songsScroll->setWidgetResizable(true);
    songsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    songsScroll->setStyleSheet("QScrollArea { border: none; }");

    mainLayout->addWidget(songsScroll);

    // Cargar canciones
    loadAlbumSongs();
}

void PlaylistDisplayUI::loadAlbumSongs() {
    QString baseDir = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs";
    QDir songsDir(baseDir);

    int index = 0;

    QStringList subdirs = songsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &songFolder : subdirs) {
        QDir songDir(songsDir.absoluteFilePath(songFolder));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);

        for (const QString &datFile : datFiles) {
            QFile f(songDir.absoluteFilePath(datFile));
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                in.setVersion(QDataStream::Qt_5_15);
                SongData song;
                in >> song;
                f.close();

                if (song.getAlbumName() == albumName) {
                    HorizontalSongCard *card = new HorizontalSongCard(
                        index + 1,   // Mostrar posición desde 1
                        song.getCoverPath(),
                        song.getTitle(),
                        song.getArtist(),
                        song.getAudioPath()
                        );

                    // Guardar canción en la lista
                    songs.append(song);

                    // 🔗 Conectar la señal de la card hacia la UI
                    connect(card, &HorizontalSongCard::songDoubleClicked,
                            this, [this, index](const QString &coverPath,
                                          const QString &title,
                                          const QString &artist,
                                          const QString &audioPath) {
                                currentIndex = index; // Guardamos qué canción está sonando
                                emit songSelected(coverPath, title, artist, audioPath);
                            });

                    songsLayout->addWidget(card);
                    index++;
                }
            }
        }
    }

    songsLayout->addStretch();
}

// 🚀 Función para reproducir la siguiente canción
void PlaylistDisplayUI::playNextSong() {
    if (currentIndex >= 0 && currentIndex + 1 < songs.size()) {
        currentIndex++;
        SongData song = songs[currentIndex];
        emit songSelected(song.getCoverPath(),
                          song.getTitle(),
                          song.getArtist(),
                          song.getAudioPath());
    }
}
