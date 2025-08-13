#ifndef ADDPLAYLISTSONGS_H
#define ADDPLAYLISTSONGS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLabel>
#include <QDir>
#include "SongData.h"

class AddPlaylistSongs : public QDialog {
    Q_OBJECT
public:
    explicit AddPlaylistSongs(const QString &username, const QString &playlistName, QWidget *parent = nullptr);

private slots:
    void onAddSongClicked(const SongData &song, const QString &sourceDatPath);

private:
    QString username;
    QString playlistName;
    QVBoxLayout *mainLayout;
};

#endif // ADDPLAYLISTSONGS_H
