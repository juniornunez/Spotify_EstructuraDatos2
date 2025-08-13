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
    explicit PlaylistUI(const QString &playlistName, QWidget *parent = nullptr);

private:
    QString playlistName;
    QLabel *coverLabel;
    QLabel *titleLabel;
    QPushButton *playButton;
    QPushButton *addSongButton;
    QVBoxLayout *mainLayout;
};

#endif // PLAYLISTUI_H
