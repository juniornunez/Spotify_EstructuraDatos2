#ifndef PLAYLISTUI_H
#define PLAYLISTUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

class PlaylistUI : public QWidget
{
    Q_OBJECT
public:
    explicit PlaylistUI(const QString &playlistName, const QString &username, QWidget *parent = nullptr);

private:
    QString playlistName;
    QString username;

    QVBoxLayout *mainLayout;
    QLabel *coverLabel;
    QLabel *titleLabel;
    QPushButton *playButton;
    QPushButton *addSongButton;
};

#endif // PLAYLISTUI_H
