#include "playbarui.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStyle>
#include <QFileInfo>
#include <QAudioOutput>

PlayBarUI::PlayBarUI(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(88);
    setStyleSheet("background: #181818; border-top: 1px solid #111;");

    // --- Left: cover, title, artist
    coverLabel = new QLabel(this);
    coverLabel->setFixedSize(60, 60);
    coverLabel->setStyleSheet("border-radius: 8px; background: #222;");

    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("color: white; font-weight: bold; font-size: 15px;");
    artistLabel = new QLabel(this);
    artistLabel->setStyleSheet("color: #b3b3b3; font-size: 13px;");

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setSpacing(0);
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->addWidget(titleLabel);
    textLayout->addWidget(artistLabel);

    QHBoxLayout *leftLayout = new QHBoxLayout;
    leftLayout->setSpacing(14);
    leftLayout->addWidget(coverLabel);
    leftLayout->addLayout(textLayout);

    // --- Center: controls
    prevButton = new QPushButton(this);
    prevButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    prevButton->setIconSize(QSize(30, 30));
    prevButton->setStyleSheet("background: none; border: none; color: #b3b3b3;");
    prevButton->setEnabled(false); // Disabled por ahora

    playPauseButton = new QPushButton(this);
    playPauseButton->setFixedSize(46, 46);
    playPauseButton->setStyleSheet("background: #1ED760; border: none; border-radius: 23px;");
    playPauseButton->setIcon(QIcon("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/playimage.png"));
    playPauseButton->setIconSize(QSize(30, 30));

    connect(playPauseButton, &QPushButton::clicked, this, &PlayBarUI::onPlayPauseClicked);

    nextButton = new QPushButton(this);
    nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    nextButton->setIconSize(QSize(30, 30));
    nextButton->setStyleSheet("background: none; border: none; color: #b3b3b3;");
    nextButton->setEnabled(false); // Disabled por ahora

    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->setSpacing(20);
    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(playPauseButton);
    controlsLayout->addWidget(nextButton);

    // --- Bottom: progress bar
    timeLabelLeft = new QLabel("0:00", this);
    timeLabelLeft->setStyleSheet("color: #ccc; font-size: 13px;");
    progressBar = new QSlider(Qt::Horizontal, this);
    progressBar->setStyleSheet(
        "QSlider::groove:horizontal { background: #555; height: 6px; border-radius: 3px; }"
        "QSlider::handle:horizontal { background: white; width: 0px; }"
        "QSlider::sub-page:horizontal { background: #fff; border-radius: 3px; }"
        "QSlider { height: 6px; }"
        );
    progressBar->setRange(0, 1000);
    progressBar->setValue(0);
    progressBar->setEnabled(false); // Solo visual

    timeLabelRight = new QLabel("0:00", this);
    timeLabelRight->setStyleSheet("color: #ccc; font-size: 13px;");

    QHBoxLayout *progressLayout = new QHBoxLayout;
    progressLayout->addWidget(timeLabelLeft);
    progressLayout->addWidget(progressBar, 1);
    progressLayout->addWidget(timeLabelRight);

    // --- Main layout vertical (controls arriba, progress abajo)
    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->addLayout(controlsLayout);
    centerLayout->addLayout(progressLayout);

    // --- Todo el playbar
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(18, 8, 18, 8);
    mainLayout->setSpacing(32);
    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addLayout(centerLayout, 3);

    setLayout(mainLayout);

    // --- Player + AudioOutput
    audioOutput = new QAudioOutput(this);  // <--- ¡NUEVO!
    audioOutput->setVolume(1.0);           // Opcional: volumen al 100%
    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);   // <--- ¡Muy importante!
    connect(player, &QMediaPlayer::positionChanged, this, &PlayBarUI::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &PlayBarUI::onDurationChanged);
}

void PlayBarUI::setSongInfo(const QString &coverPath, const QString &title, const QString &artist, const QString &audioPath)
{
    QPixmap pix(coverPath);
    if (!pix.isNull()) {
        coverLabel->setPixmap(pix.scaled(60, 60, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    } else {
        coverLabel->setText("No\nImage");
        coverLabel->setAlignment(Qt::AlignCenter);
        coverLabel->setStyleSheet("background: #333; color: #888; border-radius: 8px;");
    }
    titleLabel->setText(title);
    artistLabel->setText(artist);
    currentAudioPath = audioPath;
    player->setSource(QUrl::fromLocalFile(audioPath));
    player->stop();
    playPauseButton->setIcon(QIcon("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/playimage.png"));
    isPlaying = false;
    timeLabelLeft->setText("0:00");
    timeLabelRight->setText("0:00");
    progressBar->setValue(0);
}

void PlayBarUI::onPlayPauseClicked()
{
    if (isPlaying) {
        player->pause();
        isPlaying = false;
        playPauseButton->setIcon(QIcon("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/playimage.png"));
    } else {
        player->play();
        isPlaying = true;
        playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void PlayBarUI::onPositionChanged(qint64 position)
{
    timeLabelLeft->setText(formatTime(position));
    if (player->duration() > 0) {
        progressBar->setValue(static_cast<int>(position * 1000 / player->duration()));
    } else {
        progressBar->setValue(0);
    }
}

void PlayBarUI::play()
{
    if (!isPlaying) {
        player->play();
        isPlaying = true;
        playPauseButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
}

void PlayBarUI::onDurationChanged(qint64 duration)
{
    timeLabelRight->setText(formatTime(duration));
}

QString PlayBarUI::formatTime(qint64 ms)
{
    int seconds = static_cast<int>(ms / 1000);
    int min = seconds / 60;
    int sec = seconds % 60;
    return QString::number(min) + ":" + QString("%1").arg(sec, 2, 10, QLatin1Char('0'));
}
