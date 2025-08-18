#ifndef PLAYLISTDISPLAYUI_H
#define PLAYLISTDISPLAYUI_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "songdata.h"
#include <QPushButton>
class HorizontalSongCard;

class PlaylistDisplayUI : public QWidget
{
    Q_OBJECT
public:
    // Constructor para álbumes
    explicit PlaylistDisplayUI(const QString &albumName,
                               const QString &coverPath,
                               const QString &artistName,
                               const QString &adminUsername,
                               QWidget *parent = nullptr);

    // Constructor para playlists de usuario
    explicit PlaylistDisplayUI(const QString &playlistName,
                               const QString &username,
                               QWidget *parent = nullptr);

signals:
    void songSelected(const QString &cover,
                      const QString &title,
                      const QString &artist,
                      const QString &audio);

public slots:
    void playNextSong();

private slots:
    void onAddSongButtonClicked();

private:
    // Datos álbum
    QString albumName;
    QString coverPath;
    QString artistName;
    QString adminUsername;
    QPushButton *addSongButton;
    // Datos playlist usuario
    QString playlistName;
    QString username;

    // Estado
    bool isUserPlaylist;
    int currentIndex;
    QList<SongData> songs;

    // UI
    QLabel *coverLabel;
    QLabel *albumTitleLabel;
    QLabel *artistLabel;
    QVBoxLayout *songsLayout;

    // Helpers
    void loadAlbumSongs();
    void loadUserPlaylistSongs();
};

#endif // PLAYLISTDISPLAYUI_H
