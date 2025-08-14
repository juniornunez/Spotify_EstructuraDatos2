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

class AdminMenuUI : public QWidget
{
    Q_OBJECT
public:
    explicit AdminMenuUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent = nullptr);


private slots:
    void onArtistSettingsClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
    void handlePlayButtonPressed(SongCardWidget* card);
    void loadPlaylists();
    void onAddPlaylistClicked();
    void showPlaylistUI(const QString &playlistName);
    void restoreMainView();
    void showAdminProfileUI();
private:
    QString adminUsername;
    QString profilePicPath;

    QHBoxLayout *mainLayout;
    QVBoxLayout *sidebarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *topBarLayout;
    QHBoxLayout *cardsLayout;

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


