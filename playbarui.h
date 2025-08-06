#ifndef PLAYBARUI_H
#define PLAYBARUI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QMediaPlayer>

class PlayBarUI : public QWidget
{
    Q_OBJECT

public:
    explicit PlayBarUI(QWidget *parent = nullptr);
    void setSongInfo(const QString&, const QString&, const QString&, const QString&);
    void play(); // <---- aquí

private slots:
    void onPlayPauseClicked();
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);

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

    QString formatTime(qint64 ms);
};

#endif // PLAYBARUI_H
