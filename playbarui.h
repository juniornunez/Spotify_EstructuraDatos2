#ifndef PLAYBARUI_H
#define PLAYBARUI_H

#include <QWidget>
#include <QMediaPlayer>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QMap>

class QAudioOutput;

class PlayBarUI : public QWidget
{
    Q_OBJECT
public:
    enum RepeatMode { RepeatOne, PlayOnce, PlayNext };

    explicit PlayBarUI(const QString &username,bool isAdmin, QWidget *parent = nullptr);

    void setSongInfo(const QString &coverPath,
                     const QString &title,
                     const QString &artist,
                     const QString &audioPath);

    void play();

    // 📊 Funciones de estadísticas personales
    static int getTotalSongsListened(const QString &username);
    static qint64 getTotalListeningTime(const QString &username);
    static QMap<QString,int> getPersonalSongPlayCounts(const QString &username);

signals:
    void requestNextSong();

private slots:
    void onPlayPauseClicked();
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onSliderMoved(int value);
    void onSliderReleased();

private:
    void incrementPlayCount();
    QString formatTime(qint64 ms);
    bool isAdmin;
    // 🎵 Datos UI
    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *artistLabel;
    QPushButton *prevButton;
    QPushButton *playPauseButton;
    QPushButton *nextButton;
    QComboBox *repeatModeBox;
    QLabel *timeLabelLeft;
    QLabel *timeLabelRight;
    QSlider *progressBar;

    // 🎶 Player
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    bool isPlaying = false;
    bool alreadyCounted = false;
    RepeatMode repeatMode = PlayNext;

    // 📌 Info canción actual
    QString currentAudioPath;
    QString currentSongTitle;
    QString currentArtist;

    // 📌 Usuario actual
    QString currentUser;
};

#endif // PLAYBARUI_H
