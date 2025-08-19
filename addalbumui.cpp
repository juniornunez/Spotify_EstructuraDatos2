#include "addalbumui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>
#include <QUuid>
#include <QFileInfo>
#include <QTableWidgetItem>
#include "adminmenuui.h"
#include <QHeaderView>

AddAlbumUI::AddAlbumUI(const QString& adminUsername, QWidget *parent)
    : QWidget(parent), adminUsername(adminUsername)
{
    setStyleSheet("background-color: #191414; color: white;");

    QLabel *albumNameLabel = new QLabel("Nombre del álbum:");
    albumNameEdit = new QLineEdit;
    albumNameEdit->setStyleSheet("background: #121212; color: white; border-radius: 5px; padding: 6px;");

    QLabel *albumCoverLabel = new QLabel("Carátula del álbum:");
    albumCoverPathEdit = new QLineEdit;
    albumCoverPathEdit->setReadOnly(true);
    selectAlbumCoverButton = new QPushButton("Seleccionar imagen");
    connect(selectAlbumCoverButton, &QPushButton::clicked, this, &AddAlbumUI::onSelectAlbumCoverClicked);

    QHBoxLayout *coverLayout = new QHBoxLayout;
    coverLayout->addWidget(albumCoverPathEdit);
    coverLayout->addWidget(selectAlbumCoverButton);

    selectSongsButton = new QPushButton("Seleccionar canciones");
    connect(selectSongsButton, &QPushButton::clicked, this, &AddAlbumUI::onSelectSongsClicked);

    songsTable = new QTableWidget;
    songsTable->setColumnCount(2);
    songsTable->setHorizontalHeaderLabels({"Incluir", "Archivo"});
    songsTable->horizontalHeader()->setStretchLastSection(true);

    uploadAlbumButton = new QPushButton("Subir Álbum");
    uploadAlbumButton->setStyleSheet("background-color: #1ED760; color: black; font-weight: bold; border-radius: 20px; padding: 8px 16px;");
    connect(uploadAlbumButton, &QPushButton::clicked, this, &AddAlbumUI::onCreateAlbumClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(albumNameLabel);
    mainLayout->addWidget(albumNameEdit);
    mainLayout->addWidget(albumCoverLabel);
    mainLayout->addLayout(coverLayout);
    mainLayout->addWidget(selectSongsButton);
    mainLayout->addWidget(songsTable);
    mainLayout->addWidget(uploadAlbumButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
    setFixedSize(600, 500);
    setWindowTitle("Crear Álbum");
}

void AddAlbumUI::onSelectAlbumCoverClicked() {
    QString file = QFileDialog::getOpenFileName(this, "Seleccionar imagen del álbum", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!file.isEmpty()) albumCoverPathEdit->setText(file);
}

void AddAlbumUI::onSelectSongsClicked() {
    QStringList files = QFileDialog::getOpenFileNames(this, "Seleccionar canciones", "", "Audio Files (*.mp3 *.wav *.flac)");
    if (files.isEmpty()) return;

    songsTable->setRowCount(0);
    for (const QString &file : files) {
        int row = songsTable->rowCount();
        songsTable->insertRow(row);

        QTableWidgetItem *checkItem = new QTableWidgetItem;
        checkItem->setCheckState(Qt::Checked);
        songsTable->setItem(row, 0, checkItem);

        QFileInfo info(file);
        songsTable->setItem(row, 1, new QTableWidgetItem(info.fileName()));
        songsTable->item(row, 1)->setData(Qt::UserRole, file);
    }
}

QString AddAlbumUI::copyFileTo(const QString& sourcePath, const QString& destDir) {
    QDir().mkpath(destDir);
    QFileInfo fileInfo(sourcePath);
    QString destPath = destDir + "/" + fileInfo.fileName();
    QFile::remove(destPath);
    QFile::copy(sourcePath, destPath);
    return destPath;
}

void AddAlbumUI::saveSongData(const QString& dir, const SongData& data) {
    QFile file(dir + "/song.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15);
        out << data;
        file.close();
    }
}

void AddAlbumUI::onCreateAlbumClicked() {
    albumName = albumNameEdit->text().trimmed();
    albumCoverPath = albumCoverPathEdit->text();

    if (albumName.isEmpty() || albumCoverPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Completa el nombre y la carátula.");
        return;
    }

    QList<SongData> songs;
    for (int row = 0; row < songsTable->rowCount(); row++) {
        if (songsTable->item(row, 0)->checkState() == Qt::Unchecked) continue;

        QString audioPath = songsTable->item(row, 1)->data(Qt::UserRole).toString();
        QFileInfo info(audioPath);

        QString songID = QUuid::createUuid().toString(QUuid::WithoutBraces);
        QString title = info.baseName(); // nombre de archivo como título
        QString genre = "Pop";           // valor default
        QString duration = "--:--";      // se podría leer con QMediaPlayer
        QString description = "Canción del álbum";

        QString baseDir = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/";
        QString globalSongDir = baseDir + "globalsongs/" + songID;
        QString adminSongDir = baseDir + "admindata/" + adminUsername + "/artistsongs/" + songID;

        QString coverGlobal = copyFileTo(albumCoverPath, globalSongDir);
        QString audioGlobal = copyFileTo(audioPath, globalSongDir);

        copyFileTo(albumCoverPath, adminSongDir);
        copyFileTo(audioPath, adminSongDir);

        SongData data(songID, title, genre, duration, description,
                      coverGlobal, audioGlobal, adminUsername,
                      QDateTime::currentDateTime(), albumName, row+1);

        saveSongData(globalSongDir, data);
        saveSongData(adminSongDir, data);

        songs.append(data);
        emit songAdded(data);
    }

    if (songs.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes seleccionar al menos una canción.");
        return;
    }

    emit albumAdded(songs);
    QMessageBox::information(this, "Éxito", "Álbum subido correctamente.");
    emit albumUploaded();
    close();
}

