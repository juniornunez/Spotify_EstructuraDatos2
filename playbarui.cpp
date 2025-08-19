#include "playbarui.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QStyle>
#include <QFileInfo>
#include <QAudioOutput>
#include <QMouseEvent>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QDataStream>

PlayBarUI::PlayBarUI(const QString &username, QWidget *parent)
    : QWidget(parent), currentUser(username)
{
    setFixedHeight(88);
    setStyleSheet("background: #181818; border-top: 1px solid #111;");

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

    prevButton = new QPushButton(this);
    prevButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));
    prevButton->setIconSize(QSize(30, 30));
    prevButton->setStyleSheet("background: none; border: none; color: #b3b3b3;");
    prevButton->setEnabled(false);

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
    nextButton->setEnabled(false);

    // 🎵 ComboBox para repetición
    repeatModeBox = new QComboBox(this);
    repeatModeBox->addItem("Repetir 1 canción", RepeatOne);
    repeatModeBox->addItem("Reproducir una vez", PlayOnce);
    repeatModeBox->addItem("Siguiente canción", PlayNext);
    repeatModeBox->setCurrentIndex(2); // Default
    repeatModeBox->setStyleSheet(
        "QComboBox { background-color: #222; color: white; border-radius: 8px; padding: 4px 8px; }"
        "QComboBox::drop-down { border: none; }"
        "QComboBox QAbstractItemView { background-color: #333; color: white; selection-background-color: #1ED760; }"
        );

    connect(repeatModeBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int index){
                repeatMode = static_cast<RepeatMode>(repeatModeBox->itemData(index).toInt());
                qDebug() << "Modo repetición cambiado a:" << repeatMode;
            });

    // --- Controls layout
    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->setSpacing(20);
    controlsLayout->addWidget(prevButton);
    controlsLayout->addWidget(playPauseButton);
    controlsLayout->addWidget(nextButton);
    controlsLayout->addWidget(repeatModeBox);

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
    progressBar->setRange(0, 0);
    progressBar->setValue(0);

    connect(progressBar, &QSlider::sliderMoved, this, &PlayBarUI::onSliderMoved);
    connect(progressBar, &QSlider::sliderReleased, this, &PlayBarUI::onSliderReleased);

    timeLabelRight = new QLabel("0:00", this);
    timeLabelRight->setStyleSheet("color: #ccc; font-size: 13px;");

    QHBoxLayout *progressLayout = new QHBoxLayout;
    progressLayout->addWidget(timeLabelLeft);
    progressLayout->addWidget(progressBar, 1);
    progressLayout->addWidget(timeLabelRight);

    // --- Main layout vertical
    QVBoxLayout *centerLayout = new QVBoxLayout;
    centerLayout->addLayout(controlsLayout);
    centerLayout->addLayout(progressLayout);

    // --- Playbar completo
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(18, 8, 18, 8);
    mainLayout->setSpacing(32);
    mainLayout->addLayout(leftLayout, 2);
    mainLayout->addLayout(centerLayout, 3);

    setLayout(mainLayout);

    // --- Player + AudioOutput
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(1.0);
    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::positionChanged, this, &PlayBarUI::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &PlayBarUI::onDurationChanged);

    // 📌 Manejo del fin de canción
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            alreadyCounted = false;
            if (repeatMode == RepeatOne) {
                player->setPosition(0);
                player->play();
            } else if (repeatMode == PlayOnce) {
                isPlaying = false;
                playPauseButton->setIcon(QIcon("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/playimage.png"));
            } else if (repeatMode == PlayNext) {
                emit requestNextSong();
            }
        }
    });
}

void PlayBarUI::setSongInfo(const QString &coverPath,
                            const QString &title,
                            const QString &artist,
                            const QString &audioPath)
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
    currentSongTitle = title;
    currentArtist = artist;
    alreadyCounted = false;

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
    if (player->duration() > 0 && !progressBar->isSliderDown()) {
        progressBar->setValue(static_cast<int>(position / 1000));
    }

    if (position > 5000 && !alreadyCounted) {
        incrementPlayCount();
        alreadyCounted = true;
    }
}

void PlayBarUI::incrementPlayCount()
{
    QString userDir = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1").arg(currentUser);
    QDir().mkpath(userDir + "/songsplays");

    // 🔹 Guardar plays individuales de la canción (por título)
    QString songFile = userDir + "/songsplays/" + currentSongTitle + ".dat";
    QFile f(songFile);
    int plays = 0;
    if (f.open(QIODevice::ReadOnly)) { QDataStream in(&f); in >> plays; f.close(); }
    plays++;
    if (f.open(QIODevice::WriteOnly)) { QDataStream out(&f); out << plays; f.close(); }

    // 🔹 Guardar total de canciones escuchadas
    QString totalFile = userDir + "/totalsongs.dat";
    int totalSongs = 0;
    QFile f2(totalFile);
    if (f2.open(QIODevice::ReadOnly)) { QDataStream in(&f2); in >> totalSongs; f2.close(); }
    totalSongs++;
    if (f2.open(QIODevice::WriteOnly)) { QDataStream out(&f2); out << totalSongs; f2.close(); }

    // 🔹 Guardar tiempo total escuchado
    QString timeFile = userDir + "/totaltime.dat";
    qint64 totalTime = 0;
    QFile f3(timeFile);
    if (f3.open(QIODevice::ReadOnly)) { QDataStream in(&f3); in >> totalTime; f3.close(); }
    totalTime += player->duration() / 1000; // segundos
    if (f3.open(QIODevice::WriteOnly)) { QDataStream out(&f3); out << totalTime; f3.close(); }

    qDebug() << "Play actualizado:" << currentSongTitle << "->" << plays
             << " | Tiempo total:" << totalTime;
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
    progressBar->setRange(0, static_cast<int>(duration / 1000));
    timeLabelRight->setText(formatTime(duration));
}

QString PlayBarUI::formatTime(qint64 ms)
{
    int seconds = static_cast<int>(ms / 1000);
    int min = seconds / 60;
    int sec = seconds % 60;
    return QString::number(min) + ":" + QString("%1").arg(sec, 2, 10, QLatin1Char('0'));
}

void PlayBarUI::onSliderMoved(int value)
{
    timeLabelLeft->setText(formatTime(value * 1000));
}

void PlayBarUI::onSliderReleased()
{
    player->setPosition(progressBar->value() * 1000);
}

// ================== 📊 ESTADÍSTICAS ==================

int PlayBarUI::getTotalSongsListened(const QString &username) {
    QFile f(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/totalsongs.dat").arg(username));
    if (!f.exists()) return 0;
    if (f.open(QIODevice::ReadOnly)) { QDataStream in(&f); int val; in >> val; f.close(); return val; }
    return 0;
}

qint64 PlayBarUI::getTotalListeningTime(const QString &username) {
    QFile f(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/totaltime.dat").arg(username));
    if (!f.exists()) return 0;
    if (f.open(QIODevice::ReadOnly)) { QDataStream in(&f); qint64 val; in >> val; f.close(); return val; }
    return 0;
}

QMap<QString,int> PlayBarUI::getPersonalSongPlayCounts(const QString &username) {
    QMap<QString,int> result;
    QDir dir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/songsplays").arg(username));
    if (!dir.exists()) return result;

    QStringList files = dir.entryList(QStringList() << "*.dat", QDir::Files);
    for (const QString &file : files) {
        QFile f(dir.absoluteFilePath(file));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            int plays; in >> plays; f.close();
            QString songName = file.section(".",0,0);
            result.insert(songName, plays);
        }
    }
    return result;
}
