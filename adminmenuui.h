#ifndef ADMINMENUUI_H
#define ADMINMENUUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include "songcardwidget.h"
#include "artistcardwidget.h"
#include "playbarui.h"

class AdminMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMenuUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent = nullptr);
    ~AdminMenuUI();

private slots:
    void onAddPlaylistClicked();
    void onArtistSettingsClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
    void handlePlayButtonPressed(SongCardWidget* card);

private:
    QString adminUsername;
    QString profilePicPath;

    QVBoxLayout *sidebarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *mainLayout;
    QHBoxLayout *topBarLayout;
    QHBoxLayout *cardsLayout; // para canciones

    QListWidget *playlistList;
    QPushButton *addPlaylistButton;
    QPushButton *trendingButton;
    QPushButton *artistSettingsButton;
    QPushButton *profilePicButton;
    QLineEdit *searchBar;
    QLabel *homeIconLabel;

    QList<SongCardWidget*> songCards;
    SongCardWidget* currentSelectedCard = nullptr;
    QList<ArtistCardWidget*> artistCards;
    PlayBarUI *playBar;
};

#endif // ADMINMENUUI_H
