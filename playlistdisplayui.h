#ifndef PLAYLISTDISPLAYUI_H
#define PLAYLISTDISPLAYUI_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "songdata.h"

class PlaylistDisplayUI : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistDisplayUI(const QString &albumName,
                               const QString &coverPath,
                               const QString &artistName,
                               const QString &adminUsername,
                               QWidget *parent = nullptr);

public slots:
    void playNextSong(); // 🚀 Reproduce la siguiente canción

signals:
    void songSelected(const QString &cover,
                      const QString &title,
                      const QString &artist,
                      const QString &audioPath);

private:
    void loadAlbumSongs();

    QLabel *coverLabel;
    QLabel *albumTitleLabel;
    QLabel *artistLabel;
    QVBoxLayout *songsLayout;

    QString albumName;
    QString coverPath;
    QString artistName;
    QString adminUsername;

    QList<SongData> songs;  // 🔑 Lista de canciones del álbum
    int currentIndex;       // 🔑 Canción actual
};

#endif // PLAYLISTDISPLAYUI_H
