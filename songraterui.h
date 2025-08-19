#ifndef SONGRATERUI_H
#define SONGRATERUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QHash>
#include "songdata.h"
#include "horizontalsongcard.h"

class SongRaterUI : public QWidget
{
    Q_OBJECT
public:
    explicit SongRaterUI(const QString &username, QWidget *parent = nullptr);

private:
    QString username;
    QVBoxLayout *mainLayout;
    QLineEdit *searchBar;
    QScrollArea *scroll;
    QWidget *container;
    QVBoxLayout *songsLayout;

    // 🔑 Hash para canciones y widgets
    QHash<QString, SongData> songHash;       // songId -> SongData
    QHash<QString, QWidget*> rowHash;        // songId -> fila QWidget

    void loadSongs();
    void saveSongRating(const QString &songId, int rating);
    int loadSongRating(const QString &songId);
    void deleteSongRating(const QString &songId);
};

#endif // SONGRATERUI_H
