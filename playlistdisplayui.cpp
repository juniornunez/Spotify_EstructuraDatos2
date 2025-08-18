#include "playlistdisplayui.h"
#include "horizontalsongcard.h"
#include "songdata.h"
#include "addplaylistsongs.h"
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QPixmap>
#include <QFont>
#include <QScrollArea>
#include <QPushButton>
#include <QDebug>

// === CONSTRUCTOR PARA ÁLBUMES ===
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
    currentIndex(-1),
    isUserPlaylist(false),
    addSongButton(nullptr)
{
    setStyleSheet("background-color: #191414; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(25);

    // === HEADER ===
    QHBoxLayout *headerLayout = new QHBoxLayout;

    coverLabel = new QLabel;
    coverLabel->setFixedSize(200, 200);
    QPixmap pix(coverPath);
    if (!pix.isNull()) {
        coverLabel->setPixmap(pix.scaled(200, 200, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        coverLabel->setStyleSheet("background:#333; color:#bbb;");
        coverLabel->setText("Cover");
        coverLabel->setAlignment(Qt::AlignCenter);
    }
    headerLayout->addWidget(coverLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    QLabel *subtitle = new QLabel("Album");
    subtitle->setStyleSheet("color: #bbb; font-size: 12px;");
    albumTitleLabel = new QLabel(albumName);
    QFont albumFont; albumFont.setPointSize(28); albumFont.setBold(true);
    albumTitleLabel->setFont(albumFont);
    artistLabel = new QLabel("by " + artistName);
    artistLabel->setStyleSheet("color: #bbb; font-size: 14px;");
    infoLayout->addWidget(subtitle);
    infoLayout->addWidget(albumTitleLabel);
    infoLayout->addWidget(artistLabel);
    infoLayout->addStretch();
    headerLayout->addLayout(infoLayout);

    mainLayout->addLayout(headerLayout);

    // Lista de canciones
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

    loadAlbumSongs();
}

// === CONSTRUCTOR PARA PLAYLISTS DE USUARIO ===
PlaylistDisplayUI::PlaylistDisplayUI(const QString &playlistName,
                                     const QString &username,
                                     QWidget *parent)
    : QWidget(parent),
    playlistName(playlistName),
    username(username),
    currentIndex(-1),
    isUserPlaylist(true)
{
    setStyleSheet("background-color: #191414; color: white;");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(25);

    // === HEADER ===
    QHBoxLayout *headerLayout = new QHBoxLayout;

    coverLabel = new QLabel;
    coverLabel->setFixedSize(200, 200);

    // Intentar cargar portada de playlist
    QString coverPath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/playlists_%1/%2/cover.jpg")
                            .arg(username, playlistName);
    QPixmap pix(coverPath);
    if (!pix.isNull()) {
        coverLabel->setPixmap(pix.scaled(200, 200, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        coverLabel->setStyleSheet("background:#333; color:#bbb;");
        coverLabel->setText("Playlist Cover");
        coverLabel->setAlignment(Qt::AlignCenter);
    }
    headerLayout->addWidget(coverLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    QLabel *subtitle = new QLabel("User Playlist");
    subtitle->setStyleSheet("color: #bbb; font-size: 12px;");
    albumTitleLabel = new QLabel(playlistName);
    QFont albumFont; albumFont.setPointSize(28); albumFont.setBold(true);
    albumTitleLabel->setFont(albumFont);
    artistLabel = new QLabel("by " + username);
    artistLabel->setStyleSheet("color: #bbb; font-size: 14px;");
    infoLayout->addWidget(subtitle);
    infoLayout->addWidget(albumTitleLabel);
    infoLayout->addWidget(artistLabel);

    // 👇 Botón para agregar canciones
    addSongButton = new QPushButton("+");
    addSongButton->setFixedSize(36, 36);
    addSongButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: black; border-radius: 18px; font-size: 18pt; font-weight: bold; }"
        "QPushButton:hover { background-color: #1DB954; }"
        );
    connect(addSongButton, &QPushButton::clicked, this, &PlaylistDisplayUI::onAddSongButtonClicked);
    infoLayout->addWidget(addSongButton, 0, Qt::AlignLeft);

    infoLayout->addStretch();
    headerLayout->addLayout(infoLayout);

    mainLayout->addLayout(headerLayout);

    // Lista de canciones
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

    loadUserPlaylistSongs();
}

// === Cargar canciones de un álbum ===
void PlaylistDisplayUI::loadAlbumSongs() {
    songs.clear();
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
                SongData song; in >> song; f.close();
                if (song.getAlbumName() == albumName) {
                    HorizontalSongCard *card = new HorizontalSongCard(
                        index + 1, song.getCoverPath(), song.getTitle(),
                        song.getArtist(), song.getAudioPath());
                    songs.append(song);
                    connect(card, &HorizontalSongCard::songDoubleClicked,
                            this, [this, index](const QString &cover, const QString &title,
                                          const QString &artist, const QString &audio) {
                                currentIndex = index;
                                emit songSelected(cover, title, artist, audio);
                            });
                    songsLayout->addWidget(card);
                    index++;
                }
            }
        }
    }
    songsLayout->addStretch();
}

// === Cargar canciones de playlist de usuario ===
// === Cargar canciones de playlist de usuario ===
void PlaylistDisplayUI::loadUserPlaylistSongs() {
    songs.clear();
    QString playlistPath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/playlists_%1/%2")
                               .arg(username, playlistName);
    QDir playlistDir(playlistPath);

    int index = 0;
    QStringList songFolders = playlistDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &songId : songFolders) {
        QDir songDir(playlistDir.filePath(songId));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);
        if (!datFiles.isEmpty()) {
            QFile f(songDir.filePath(datFiles.first()));
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                in.setVersion(QDataStream::Qt_5_15);
                SongData song; in >> song; f.close();

                // 🔹 Widget fila
                QWidget *rowWidget = new QWidget;
                QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
                rowLayout->setContentsMargins(0, 0, 0, 0);

                // 🔹 Tarjeta de canción
                HorizontalSongCard *card = new HorizontalSongCard(
                    index + 1, song.getCoverPath(), song.getTitle(),
                    song.getArtist(), song.getAudioPath());

                connect(card, &HorizontalSongCard::songDoubleClicked,
                        this, [this, index](const QString &cover, const QString &title,
                                      const QString &artist, const QString &audio) {
                            currentIndex = index;
                            emit songSelected(cover, title, artist, audio);
                        });

                rowLayout->addWidget(card, 1);

                // 🔹 Botón rojo de eliminar (" - ")
                QPushButton *removeBtn = new QPushButton("-");
                removeBtn->setFixedSize(32, 32);
                removeBtn->setStyleSheet(
                    "QPushButton { background-color: #d32f2f; color: white; font-size: 16pt; font-weight: bold; border-radius: 16px; }"
                    "QPushButton:hover { background-color: #b71c1c; }"
                    );
                rowLayout->addWidget(removeBtn);

                connect(removeBtn, &QPushButton::clicked, this, [=]() {
                    if (QMessageBox::question(this, "Eliminar canción",
                                              QString("¿Quieres eliminar \"%1\" de esta playlist?").arg(song.getTitle()))
                        == QMessageBox::Yes)
                    {
                        QDir dirToRemove(songDir.absolutePath());
                        dirToRemove.removeRecursively(); // ✅ Ahora sí borra bien la carpeta

                        // Recargar canciones
                        QLayoutItem *child;
                        while ((child = songsLayout->takeAt(0)) != nullptr) {
                            if (child->widget()) child->widget()->deleteLater();
                            delete child;
                        }
                        loadUserPlaylistSongs();
                    }
                });


                songs.append(song);
                songsLayout->addWidget(rowWidget);
                index++;
            }
        }
    }
    songsLayout->addStretch();
}


// === Reproducir siguiente canción ===
void PlaylistDisplayUI::playNextSong() {
    if (currentIndex >= 0 && currentIndex + 1 < songs.size()) {
        currentIndex++;
        SongData song = songs[currentIndex];
        emit songSelected(song.getCoverPath(), song.getTitle(),
                          song.getArtist(), song.getAudioPath());
    }
}

// === Botón para agregar canciones ===
void PlaylistDisplayUI::onAddSongButtonClicked() {
    AddPlaylistSongs dialog(username, playlistName, this);
    dialog.exec();

    // Recargar canciones después de cerrar el diálogo
    QLayoutItem *child;
    while ((child = songsLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    loadUserPlaylistSongs();
}
