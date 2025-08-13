#include "addplaylistsongs.h"
#include <QHBoxLayout>
#include <QFile>
#include <QDataStream>
#include <QMessageBox>

AddPlaylistSongs::AddPlaylistSongs(const QString &username, const QString &playlistName, QWidget *parent)
    : QDialog(parent), username(username), playlistName(playlistName)
{
    setWindowTitle("Add Songs to Playlist");
    resize(400, 500);
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
    songsLayout->setSpacing(8);

    QDir songsDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
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

                QWidget *songRow = new QWidget;
                QHBoxLayout *rowLayout = new QHBoxLayout(songRow);
                rowLayout->setContentsMargins(0, 0, 0, 0);

                QLabel *songLabel = new QLabel(QString("%1 - %2").arg(song.getTitle(), song.getArtist()));
                rowLayout->addWidget(songLabel);

                QPushButton *addBtn = new QPushButton("+");
                addBtn->setFixedSize(32, 32);
                addBtn->setStyleSheet(
                    "QPushButton { background-color: #1ED760; color: black; border-radius: 16px; font-weight: bold; }"
                    "QPushButton:hover { background-color: #1DB954; }"
                    );
                rowLayout->addWidget(addBtn);

                connect(addBtn, &QPushButton::clicked, this, [=]() {
                    onAddSongClicked(song);
                });

                songsLayout->addWidget(songRow);
            }
        }
    }

    container->setLayout(songsLayout);
    scroll->setWidget(container);
    mainLayout->addWidget(scroll);
}

void AddPlaylistSongs::onAddSongClicked(const SongData &song) {
    QDir playlistDir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/playlists_%1/%2")
                         .arg(username, playlistName));
    if (!playlistDir.exists()) {
        QDir().mkpath(playlistDir.absolutePath());
    }

    QString destPath = playlistDir.filePath(song.getId() + ".dat");
    if (QFile::exists(destPath)) {
        QMessageBox::information(this, "Already Added", "This song is already in the playlist.");
        return;
    }

    if (QFile::copy(song.getFilePath(), destPath)) {
        QMessageBox::information(this, "Added", "Song added to playlist!");
    } else {
        QMessageBox::warning(this, "Error", "Failed to add song.");
    }
}
