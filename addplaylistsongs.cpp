#include "addplaylistsongs.h"
#include "songdata.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>
#include <QFileInfo>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QDir>

AddPlaylistSongs::AddPlaylistSongs(const QString &username, const QString &playlistName, QWidget *parent)
    : QDialog(parent), username(username), playlistName(playlistName)
{
    setWindowTitle("Add Songs to Playlist");
    resize(450, 600);
    setStyleSheet("background-color: #191414; color: white;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    QLabel *title = new QLabel(QString("Add songs to: %1").arg(playlistName));
    title->setStyleSheet("font-size: 16pt; font-weight: bold;");
    mainLayout->addWidget(title);

    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    QWidget *container = new QWidget;
    QVBoxLayout *songsLayout = new QVBoxLayout(container);
    songsLayout->setSpacing(10);

    // Directorio global de canciones
    QDir songsDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
    QStringList subdirs = songsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &songFolder : subdirs) {
        QDir songDir(songsDir.absoluteFilePath(songFolder));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);

        for (const QString &datFile : datFiles) {
            QString datPath = songDir.absoluteFilePath(datFile);
            QFile f(datPath);
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                in.setVersion(QDataStream::Qt_5_15);
                SongData song;
                in >> song;
                f.close();

                QWidget *songRow = new QWidget;
                QHBoxLayout *rowLayout = new QHBoxLayout(songRow);
                rowLayout->setContentsMargins(5, 5, 5, 5);
                rowLayout->setSpacing(10);

                // 🎵 Cover
                QLabel *coverLabel = new QLabel;
                coverLabel->setFixedSize(40, 40);
                QPixmap pix(song.getCoverPath());
                if (!pix.isNull()) {
                    coverLabel->setPixmap(pix.scaled(40, 40, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
                } else {
                    coverLabel->setStyleSheet("background:#333; color:#aaa;");
                    coverLabel->setText("No\nImg");
                    coverLabel->setAlignment(Qt::AlignCenter);
                }
                rowLayout->addWidget(coverLabel);

                // 🎵 Texto
                QLabel *songLabel = new QLabel(QString("%1 - %2").arg(song.getTitle(), song.getArtist()));
                rowLayout->addWidget(songLabel);

                // 🎵 Botón +
                QPushButton *addBtn = new QPushButton("+");
                addBtn->setFixedSize(32, 32);
                addBtn->setStyleSheet(
                    "QPushButton { background-color: #1ED760; color: black; border-radius: 16px; font-weight: bold; }"
                    "QPushButton:hover { background-color: #1DB954; }"
                    );
                rowLayout->addWidget(addBtn);

                connect(addBtn, &QPushButton::clicked, this, [=]() {
                    onAddSongClicked(song, datPath);
                });

                songsLayout->addWidget(songRow);
            }
        }
    }

    container->setLayout(songsLayout);
    scroll->setWidget(container);
    mainLayout->addWidget(scroll);
}

void AddPlaylistSongs::onAddSongClicked(const SongData &song, const QString &sourceDatPath) {
    // 📌 CAMBIO: Guardar en la misma carpeta donde PlaylistDisplayUI busca
    QString playlistDirPath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/playlists_%1/%2")
                                  .arg(username, playlistName);

    // Crear carpeta de playlist si no existe
    QDir().mkpath(playlistDirPath);

    // Crear carpeta para esta canción dentro de la playlist
    QString songFolderPath = playlistDirPath + "/" + song.getId();
    QDir().mkpath(songFolderPath);

    // Ruta destino para el .dat
    QString destDatPath = songFolderPath + "/" + QFileInfo(sourceDatPath).fileName();

    if (QFile::exists(destDatPath)) {
        QMessageBox::information(this, "Already Added", "This song is already in the playlist.");
        return;
    }

    // Copiar archivo .dat
    if (!QFile::copy(sourceDatPath, destDatPath)) {
        QMessageBox::warning(this, "Error", "Failed to add song.");
        return;
    }

    // Copiar cover
    if (QFile::exists(song.getCoverPath())) {
        QFile::copy(song.getCoverPath(), songFolderPath + "/" + QFileInfo(song.getCoverPath()).fileName());
    }

    // Copiar archivo de audio
    if (QFile::exists(song.getAudioPath())) {
        QFile::copy(song.getAudioPath(), songFolderPath + "/" + QFileInfo(song.getAudioPath()).fileName());
    }

    QMessageBox::information(this, "Added", "Song added to playlist!");
}
