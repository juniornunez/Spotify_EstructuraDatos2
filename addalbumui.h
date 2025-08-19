#ifndef ADDALBUMUI_H
#define ADDALBUMUI_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include "songdata.h"

class AddAlbumUI : public QWidget {
    Q_OBJECT
public:
    explicit AddAlbumUI(const QString& adminUsername, QWidget *parent = nullptr);

signals:
    void albumAdded(const QList<SongData>& songs);
    void songAdded(const SongData& song);
    void albumUploaded();

private slots:
    void onSelectAlbumCoverClicked();
    void onSelectSongsClicked();
    void onCreateAlbumClicked();

private:
    QString adminUsername;
    QString albumName;
    QString albumCoverPath;

    QLineEdit *albumNameEdit;
    QLineEdit *albumCoverPathEdit;
    QPushButton *selectAlbumCoverButton;
    QPushButton *selectSongsButton;
    QPushButton *uploadAlbumButton;
    QTableWidget *songsTable;

    QString copyFileTo(const QString& sourcePath, const QString& destDir);
    void saveSongData(const QString& dir, const SongData& data);
};

#endif // ADDALBUMUI_H
