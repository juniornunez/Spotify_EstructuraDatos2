#ifndef ADMINMENUUI_H
#define ADMINMENUUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QMenu>
#include <QAction>
#include <QMap>
#include "songdata.h"
#include "songcardwidget.h"
#include "artistcardwidget.h"
#include "playbarui.h"
#include "songraterui.h"

class AdminMenuUI : public QWidget
{
    Q_OBJECT

public slots:
    void showAlbumUI(const QString &albumName,
                     const QString &coverPath,
                     const QString &artistName);


public:
    explicit AdminMenuUI(const QString &profilePicPath, const QString &adminUsername, bool isAdmin, QWidget *parent = nullptr);
    void restoreMainView();

private slots:
    void onArtistSettingsClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
    void handlePlayButtonPressed(SongCardWidget* card);
    void loadPlaylists();
    void onAddPlaylistClicked();
    void showPlaylistUI(const QString &playlistName);

    void showAdminProfileUI();

private:
    QString adminUsername;
    QString profilePicPath;
    bool isAdmin;
    QHBoxLayout *mainLayout;
    QVBoxLayout *sidebarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *topBarLayout;
    QHBoxLayout *cardsLayout;
    QPushButton *rateSongsButton;
    QPushButton *homeButton;
    QPushButton *addPlaylistButton;
    QPushButton *trendingButton;
    QPushButton *artistSettingsButton;
    QPushButton *profilePicButton;
    QListWidget *playlistList;
    QLabel *homeIconLabel;
    QLineEdit *searchBar;

    QList<SongCardWidget*> songCards;
    QList<ArtistCardWidget*> artistCards;
    QMap<QString, SongData> songHash;

    SongCardWidget *currentSelectedCard;
    PlayBarUI *playBar;

    QWidget *originalContentWidget; // Guarda la vista original
    QWidget *currentViewWidget;     // Guarda la vista actualmente mostrada
};

#endif // ADMINMENUUI_H


