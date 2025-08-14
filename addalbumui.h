#ifndef ADDALBUMUI_H
#define ADDALBUMUI_H

#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QLabel>
#include "SongData.h"

class AddAlbumUI : public QWidget
{
    Q_OBJECT
public:
    explicit AddAlbumUI(const QString& adminUsername, QWidget *parent = nullptr);

signals:
    void albumAdded(const QList<SongData>& songs);
    void songAdded(const SongData &data);
private slots:
    void onContinueClicked();
    void onSelectCoverClicked(QWidget *tab);
    void onSelectAudioClicked(QWidget *tab);
    void onCreateAlbumClicked();

private:
    QWidget* createSongTab(int trackNumber);
    bool validateSongTab(QWidget *tab);
    QString copyFileTo(const QString& sourcePath, const QString& destDir);
    void saveSongData(const QString& dir, const SongData& data);

    QString adminUsername;
    QString albumName;
    int songCount;

    // Paso inicial
    QLineEdit *albumNameEdit;
    QLineEdit *songCountEdit;
    QPushButton *continueButton;

    // Pestañas
    QTabWidget *tabWidget;
    QPushButton *uploadAlbumButton;
};

#endif // ADDALBUMUI_H
