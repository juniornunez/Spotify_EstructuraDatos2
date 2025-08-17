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
    void setSongInfo(const QString &coverPath,
                     const QString &title,
                     const QString &artist,
                     const QString &audioPath);
    void play();

signals:
    void requestNextSong();  // 🚀 Para que AdminMenuUI/PlaylistDisplayUI puedan reaccionar al "Siguiente canción"

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
    QString currentAudioPath;
    bool isPlaying = false;

    // 🔁 Modos de repetición
    enum RepeatMode { RepeatOne, PlayOnce, PlayNext };
    RepeatMode repeatMode = PlayNext;  // Default: siguiente canción
    QComboBox *repeatModeBox;

    QString formatTime(qint64 ms);
};

#endif // PLAYBARUI_H
