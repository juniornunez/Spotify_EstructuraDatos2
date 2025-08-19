#ifndef USERMENUUI_H
#define USERMENUUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QMap>
#include "songdata.h"
#include "songcardwidget.h"
#include "artistcardwidget.h"
#include "playbarui.h"
#include "playlistdisplayui.h"

class UserMenuUI : public QWidget {
    Q_OBJECT
public:
    explicit UserMenuUI(const QString &profilePicPath, const QString &username, bool isAdmin, QWidget *parent = nullptr);

private slots:
    void onAddPlaylistClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
    void handlePlayButtonPressed(SongCardWidget* card);
    void restoreMainView();
    void showPlaylistUI(const QString &playlistName);
    void showUserProfileUI();
    void showAlbumUI(const QString &albumName,
                     const QString &coverPath,
                     const QString &artistName);

private:
    void loadPlaylists();
    bool isAdmin;
    QString username;
    QString profilePicPath;

    QHBoxLayout *mainLayout;
    QVBoxLayout *sidebarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *topBarLayout;

    QListWidget *playlistList;
    QPushButton *addPlaylistButton;
    QPushButton *trendingButton;
    QPushButton *rateSongsButton;
    QPushButton *homeButton;
    QPushButton *profilePicButton;
    QLineEdit   *searchBar;

    QWidget *originalContentWidget;
    QWidget *currentViewWidget;

    PlayBarUI *playBar;

    QHBoxLayout *cardsLayout;
    QList<SongCardWidget*> songCards;
    QList<ArtistCardWidget*> artistCards;

    SongCardWidget* currentSelectedCard = nullptr;

    QMap<QString, SongData> songHash;
};

#endif // USERMENUUI_H
