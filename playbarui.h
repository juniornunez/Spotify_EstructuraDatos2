#ifndef PLAYBARUI_H
#define PLAYBARUI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QComboBox>

class PlayBarUI : public QWidget
{
    Q_OBJECT

public:
    explicit PlayBarUI(QWidget *parent = nullptr);

    // ✅ Guardamos plays en admindata/<artist>/songsplays/<title>/plays.dat
    void setSongInfo(const QString &coverPath,
                     const QString &title,
                     const QString &artist,
                     const QString &audioPath);

    void play();

signals:
    void requestNextSong();

private slots:
    void onPlayPauseClicked();
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onSliderMoved(int value);
    void onSliderReleased();

private:
    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *artistLabel;
    QPushButton *prevButton;
    QPushButton *playPauseButton;
    QPushButton *nextButton;
    QLabel *timeLabelLeft;
    QLabel *timeLabelRight;
    QSlider *progressBar;
    QAudioOutput *audioOutput;
    QMediaPlayer *player;
    QComboBox *repeatModeBox;

    QString currentAudioPath;
    QString currentSongTitle;   // ✅ título como "ID"
    QString currentArtist;      // ✅ carpeta de artista
    bool isPlaying = false;
    bool alreadyCounted = false; // ✅ controla conteo de reproducciones

    enum RepeatMode { RepeatOne, PlayOnce, PlayNext };
    RepeatMode repeatMode = PlayNext;

    QString formatTime(qint64 ms);
    void incrementPlayCount(); // ✅ cuenta reproducciones en admindata
};

#endif // PLAYBARUI_H
