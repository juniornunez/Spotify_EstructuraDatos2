#ifndef MANAGESONGSUI_H
#define MANAGESONGSUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include "songdata.h"
#include "horizontalsongcard.h"

class ManageSongsUI : public QWidget
{
    Q_OBJECT
public:
    explicit ManageSongsUI(const QString &adminUsername, QWidget *parent = nullptr)
        : QWidget(parent), adminUsername(adminUsername)
    {
        setStyleSheet("background-color: #191414; color: white;");

        mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(10);

        QLabel *title = new QLabel("Gestionar mi música");
        QFont titleFont = title->font();
        titleFont.setPointSize(20);
        titleFont.setBold(true);
        title->setFont(titleFont);
        mainLayout->addWidget(title);

        loadSongs();
    }

private:
    QString adminUsername;
    QVBoxLayout *mainLayout;

    void loadSongs()
    {
        // Limpiar lo que haya
        QLayoutItem *child;
        while ((child = mainLayout->takeAt(1)) != nullptr) {
            if (child->widget()) child->widget()->deleteLater();
            delete child;
        }

        QDir artistSongsDir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/artistsongs").arg(adminUsername));
        if (!artistSongsDir.exists()) {
            QLabel *noSongs = new QLabel("No tienes canciones subidas.");
            noSongs->setStyleSheet("color: #bbb; font-size: 14pt;");
            mainLayout->addWidget(noSongs);
            return;
        }

        QStringList songFolders = artistSongsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &songFolder : songFolders) {
            QDir songDir(artistSongsDir.absoluteFilePath(songFolder));
            QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);
            if (datFiles.isEmpty()) continue;

            QString datPath = songDir.absoluteFilePath(datFiles.first());
            QFile f(datPath);
            if (!f.open(QIODevice::ReadOnly)) continue;

            QDataStream in(&f);
            in.setVersion(QDataStream::Qt_5_15);
            SongData song;
            in >> song;
            f.close();

            QWidget *rowWidget = new QWidget;
            QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(0, 0, 0, 0);

            // Card
            HorizontalSongCard *card = new HorizontalSongCard(0, song.getCoverPath(), song.getTitle(), song.getArtist());
            rowLayout->addWidget(card, 1);

            // Botón eliminar
            QPushButton *deleteButton = new QPushButton("Eliminar");
            deleteButton->setStyleSheet(
                "QPushButton { background-color: #d32f2f; color: white; font-weight: bold; padding: 6px 14px; border-radius: 6px; }"
                "QPushButton:hover { background-color: #b71c1c; }"
                );
            rowLayout->addWidget(deleteButton, 0, Qt::AlignRight);

            connect(deleteButton, &QPushButton::clicked, this, [=]() {
                if (QMessageBox::question(this, "Confirmar eliminación",
                                          QString("¿Eliminar la canción \"%1\"?").arg(song.getTitle())) == QMessageBox::Yes)
                {
                    deleteSong(song.getId());
                    loadSongs(); // refrescar
                }
            });

            mainLayout->addWidget(rowWidget);
        }
    }

    void deleteSong(const QString &songId)
    {
        // Ruta en globalsongs
        QDir globalsDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
        QStringList globalSubdirs = globalsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &folder : globalSubdirs) {
            if (folder == songId) {
                QDir dirToRemove(globalsDir.absoluteFilePath(folder));
                dirToRemove.removeRecursively();
                break;
            }
        }

        // Ruta en artistsongs
        QDir artistDir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/artistsongs").arg(adminUsername));
        QStringList artistSubdirs = artistDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &folder : artistSubdirs) {
            if (folder == songId) {
                QDir dirToRemove(artistDir.absoluteFilePath(folder));
                dirToRemove.removeRecursively();
                break;
            }
        }
    }
};

#endif // MANAGESONGSUI_H
