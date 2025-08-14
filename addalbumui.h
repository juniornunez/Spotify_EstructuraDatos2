#ifndef ADDALBUMUI_H
#define ADDALBUMUI_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>
#include <QComboBox>
#include <QTextEdit>
#include "songdata.h"

class AddAlbumUI : public QWidget
{
    Q_OBJECT

public:
    explicit AddAlbumUI(const QString& adminUsername, QWidget *parent = nullptr);

signals:
    void songAdded(const SongData& song);
    void albumAdded(const QList<SongData>& songs);

private slots:
    void onContinueClicked();
    void onSelectAlbumCoverClicked();
    void onSelectAudioClicked(QWidget *tab);
    void onCreateAlbumClicked();

private:
    QWidget* createSongTab(int trackNumber);
    bool validateSongTab(QWidget *tab);
    QString copyFileTo(const QString& sourcePath, const QString& destDir);
    void saveSongData(const QString& dir, const SongData& data);

    QString adminUsername;
    QString albumName;
    QString albumCoverPath;  // Carátula única del álbum
    int songCount;

    QLineEdit *albumNameEdit;
    QLineEdit *songCountEdit;
    QLineEdit *albumCoverPathEdit;
    QPushButton *continueButton;
    QPushButton *uploadAlbumButton;
    QTabWidget *tabWidget;
};

#endif // ADDALBUMUI_H
