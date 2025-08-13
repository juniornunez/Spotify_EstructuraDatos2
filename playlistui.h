#ifndef PLAYLISTUI_H
#define PLAYLISTUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QEvent>

class PlaylistUI : public QWidget {
    Q_OBJECT
public:
    explicit PlaylistUI(const QString &playlistName, const QString &username, QWidget *parent = nullptr);

signals:
    void playSong(const QString &audioPath); // 🔹 Señal para reproducir en PlayBarUI

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

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
