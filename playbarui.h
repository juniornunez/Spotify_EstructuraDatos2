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

struct SongNode {
    QString cover;
    QString title;
    QString artist;
    QString audioPath;
    SongNode* next;

    SongNode(const QString &c, const QString &t, const QString &a, const QString &p)
        : cover(c), title(t), artist(a), audioPath(p), next(nullptr) {}
};

class PlayBarUI : public QWidget
{
    Q_OBJECT
public:
    enum RepeatMode { RepeatOne, PlayOnce, PlayNext };
      void nextSong();
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
    SongNode* head = nullptr;       // inicio de la lista
    SongNode* currentNode = nullptr; // nodo actual
    void incrementPlayCount();
    QString formatTime(qint64 ms);
    bool isAdmin;

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
