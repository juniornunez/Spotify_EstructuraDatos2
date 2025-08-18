#include "songraterui.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileInfo>

// Constructor
SongRaterUI::SongRaterUI(const QString &username, QWidget *parent)
    : QWidget(parent), username(username)
{
    setStyleSheet("background-color: #191414; color: white;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(10);

    QLabel *title = new QLabel("Rate Songs ⭐");
    QFont titleFont = title->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    title->setFont(titleFont);
    mainLayout->addWidget(title);

    loadSongs();
}

void SongRaterUI::loadSongs()
{
    // Scroll
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    QWidget *container = new QWidget;
    QVBoxLayout *songsLayout = new QVBoxLayout(container);
    songsLayout->setSpacing(10);

    // Cargar todas las canciones de globalsongs
    QDir songsDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
    QStringList subdirs = songsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    for (const QString &songFolder : subdirs) {
        QDir songDir(songsDir.absoluteFilePath(songFolder));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);
        if (datFiles.isEmpty()) continue;

        QFile f(songDir.absoluteFilePath(datFiles.first()));
        if (!f.open(QIODevice::ReadOnly)) continue;

        QDataStream in(&f);
        in.setVersion(QDataStream::Qt_5_15);
        SongData song; in >> song; f.close();

        QWidget *rowWidget = new QWidget;
        QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
        rowLayout->setContentsMargins(0, 0, 0, 0);

        // 🎵 Song Card
        HorizontalSongCard *card = new HorizontalSongCard(
            0, song.getCoverPath(), song.getTitle(), song.getArtist(), song.getAudioPath()
            );
        rowLayout->addWidget(card, 2);

        // ⭐ Stars
        QWidget *starsWidget = new QWidget;
        QHBoxLayout *starsLayout = new QHBoxLayout(starsWidget);
        starsLayout->setContentsMargins(0, 0, 0, 0);
        starsLayout->setSpacing(3);

        auto *stars = new QVector<QPushButton*>(); // 👈 ahora persiste
        int currentRating = loadSongRating(song.getId());

        for (int i = 1; i <= 5; i++) {
            QPushButton *star = new QPushButton("★");
            star->setFixedSize(28, 28);
            star->setStyleSheet(
                QString("QPushButton { font-size:18px; border:none; color:%1; }")
                    .arg(i <= currentRating ? "#FFD700" : "#555")
                );

            connect(star, &QPushButton::clicked, this, [this, song, i, stars]() {
                saveSongRating(song.getId(), i);
                // actualizar colores
                for (int j = 0; j < stars->size(); j++) {
                    (*stars)[j]->setStyleSheet(
                        QString("QPushButton { font-size:18px; border:none; color:%1; }")
                            .arg(j < i ? "#FFD700" : "#555")
                        );
                }
            });

            stars->push_back(star);
            starsLayout->addWidget(star);
        }

        rowLayout->addWidget(starsWidget);

        // ❌ Botón eliminar rating
        QPushButton *deleteBtn = new QPushButton("-");
        deleteBtn->setFixedSize(32, 32);
        deleteBtn->setStyleSheet(
            "QPushButton { background-color:#d32f2f; color:white; font-weight:bold; border-radius:16px; }"
            "QPushButton:hover { background-color:#b71c1c; }"
            );

        connect(deleteBtn, &QPushButton::clicked, this, [this, song, stars]() {
            deleteSongRating(song.getId());
            for (auto *s : *stars) {
                s->setStyleSheet("QPushButton { font-size:18px; border:none; color:#555; }");
            }
        });

        rowLayout->addWidget(deleteBtn);
        songsLayout->addWidget(rowWidget);
    }

    container->setLayout(songsLayout);
    scroll->setWidget(container);
    mainLayout->addWidget(scroll);
}

// 🔹 Guardar rating
void SongRaterUI::saveSongRating(const QString &songId, int rating)
{
    QString dirPath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/ratingsongs/%1").arg(username);
    QDir().mkpath(dirPath);

    QFile f(dirPath + "/" + songId + ".dat");
    if (f.open(QIODevice::WriteOnly)) {
        QDataStream out(&f);
        out.setVersion(QDataStream::Qt_5_15);
        out << rating;
        f.close();
    }
}

// 🔹 Cargar rating
int SongRaterUI::loadSongRating(const QString &songId)
{
    QString filePath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/ratingsongs/%1/%2.dat")
    .arg(username, songId);
    QFile f(filePath);
    if (!f.exists()) return 0;

    if (f.open(QIODevice::ReadOnly)) {
        QDataStream in(&f);
        in.setVersion(QDataStream::Qt_5_15);
        int rating; in >> rating; f.close();
        return rating;
    }
    return 0;
}

// 🔹 Eliminar rating
void SongRaterUI::deleteSongRating(const QString &songId)
{
    QString filePath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/ratingsongs/%1/%2.dat")
    .arg(username, songId);
    QFile::remove(filePath);
}
