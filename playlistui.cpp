#include "playlistui.h"
#include "addplaylistsongs.h"
#include <QFont>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QPixmap>
#include <QFileInfo>
#include <QMouseEvent>
#include "SongData.h"

PlaylistUI::PlaylistUI(const QString &playlistName, const QString &username, QWidget *parent)
    : QWidget(parent), playlistName(playlistName), username(username)
{
    setStyleSheet("background-color: #191414; color: white;");

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // Cabecera
    QHBoxLayout *headerLayout = new QHBoxLayout;

    coverLabel = new QLabel;
    coverLabel->setFixedSize(150, 150);
    coverLabel->setStyleSheet("background-color: #333; border-radius: 8px;");
    coverLabel->setAlignment(Qt::AlignCenter);
    coverLabel->setText("Cover");
    headerLayout->addWidget(coverLabel);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    QLabel *playlistType = new QLabel("Public Playlist");
    playlistType->setStyleSheet("color: #bbb; font-size: 10pt;");
    infoLayout->addWidget(playlistType);

    titleLabel = new QLabel(playlistName);
    QFont titleFont;
    titleFont.setPointSize(32);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    infoLayout->addWidget(titleLabel);

    infoLayout->addSpacing(10);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    playButton = new QPushButton("▶");
    playButton->setFixedSize(50, 50);
    playButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: black; border-radius: 25px; font-size: 20pt; }"
        "QPushButton:hover { background-color: #1DB954; }"
        );
    buttonLayout->addWidget(playButton);

    addSongButton = new QPushButton("+");
    addSongButton->setFixedSize(40, 40);
    addSongButton->setStyleSheet(
        "QPushButton { background-color: #282828; color: white; border-radius: 20px; font-size: 18pt; }"
        "QPushButton:hover { background-color: #333; }"
        );
    buttonLayout->addWidget(addSongButton);

    connect(addSongButton, &QPushButton::clicked, this, [=]() {
        AddPlaylistSongs *addWindow = new AddPlaylistSongs(username, playlistName, this);
        addWindow->setAttribute(Qt::WA_DeleteOnClose);
        addWindow->show();
    });

    infoLayout->addLayout(buttonLayout);
    headerLayout->addLayout(infoLayout);
    headerLayout->addStretch();

    mainLayout->addLayout(headerLayout);

    // Scroll para canciones
    QScrollArea *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    QWidget *songsWidget = new QWidget;
    QVBoxLayout *songsLayout = new QVBoxLayout(songsWidget);
    songsLayout->setContentsMargins(0, 0, 0, 0);
    songsLayout->setSpacing(0);

    QString playlistPath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/%2")
                               .arg(username, playlistName);
    QDir playlistDir(playlistPath);

    if (playlistDir.exists()) {
        QStringList songFolders = playlistDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        if (!songFolders.isEmpty()) {
            int index = 1;
            for (const QString &songId : songFolders) {
                QDir songDir(playlistDir.filePath(songId));
                QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);
                if (!datFiles.isEmpty()) {
                    QFile f(songDir.filePath(datFiles.first()));
                    if (f.open(QIODevice::ReadOnly)) {
                        QDataStream in(&f);
                        in.setVersion(QDataStream::Qt_5_15);
                        SongData song;
                        in >> song;
                        f.close();

                        QWidget *songRow = new QWidget;
                        QHBoxLayout *rowLayout = new QHBoxLayout(songRow);
                        rowLayout->setContentsMargins(10, 5, 10, 5);
                        rowLayout->setSpacing(10);

                        QLabel *indexLabel = new QLabel(QString::number(index++));
                        indexLabel->setStyleSheet("color: #aaa; font-size: 10pt;");
                        indexLabel->setFixedWidth(20);
                        rowLayout->addWidget(indexLabel);

                        QLabel *cover = new QLabel;
                        cover->setFixedSize(40, 40);
                        QPixmap pix(song.getCoverPath());
                        if (!pix.isNull()) {
                            cover->setPixmap(pix.scaled(40, 40, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
                        } else {
                            cover->setStyleSheet("background-color: #333;");
                        }
                        rowLayout->addWidget(cover);

                        QVBoxLayout *textLayout = new QVBoxLayout;
                        QLabel *titleLbl = new QLabel(song.getTitle());
                        titleLbl->setStyleSheet("font-weight: bold; font-size: 10pt; color: white;");
                        QLabel *artistLbl = new QLabel(song.getArtist());
                        artistLbl->setStyleSheet("color: #bbb; font-size: 9pt;");
                        textLayout->addWidget(titleLbl);
                        textLayout->addWidget(artistLbl);
                        rowLayout->addLayout(textLayout);

                        songRow->setLayout(rowLayout);

                        // 🎵 Reproducir al doble click
                        songRow->installEventFilter(this);
                        songRow->setProperty("audioPath", song.getAudioPath());

                        songsLayout->addWidget(songRow);

                        // Separador visual
                        QFrame *separator = new QFrame;
                        separator->setFrameShape(QFrame::HLine);
                        separator->setFrameShadow(QFrame::Sunken);
                        separator->setStyleSheet("color: #333;");
                        songsLayout->addWidget(separator);
                    }
                }
            }
        } else {
            QLabel *placeholder = new QLabel("No songs yet");
            placeholder->setStyleSheet("color: #aaa; font-size: 12pt;");
            songsLayout->addWidget(placeholder);
        }
    }

    scroll->setWidget(songsWidget);
    mainLayout->addWidget(scroll, 1);
}

// Captura doble click
bool PlaylistUI::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonDblClick) {
        QString path = obj->property("audioPath").toString();
        if (!path.isEmpty()) {
            emit playSong(path); // 🔹 Señal que debe estar conectada al PlayBarUI
        }
        return true;
    }
    return QWidget::eventFilter(obj, event);
}
