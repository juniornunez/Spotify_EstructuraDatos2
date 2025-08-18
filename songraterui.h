#ifndef SONGRATERUI_H
#define SONGRATERUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QDir>
#include <QFile>
#include <QPushButton>
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

    void loadSongs();
    void saveSongRating(const QString &songId, int rating);
    int loadSongRating(const QString &songId);
    void deleteSongRating(const QString &songId);
};

#endif // SONGRATERUI_H
