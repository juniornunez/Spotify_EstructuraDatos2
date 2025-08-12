#ifndef ADMINMENUUI_H
#define ADMINMENUUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QMenu>
#include <QHash>
#include "songdata.h"
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
    QHBoxLayout *topBarLayout;
    QHBoxLayout *cardsLayout;
    QHBoxLayout *mainLayout;

    QPushButton *addPlaylistButton;
    QListWidget *playlistList;
    QPushButton *trendingButton;
    QPushButton *artistSettingsButton;
    QPushButton *profilePicButton;
    QLabel *homeIconLabel;
    QLineEdit *searchBar;

    PlayBarUI *playBar;
    SongCardWidget *currentSelectedCard = nullptr;

    QList<SongCardWidget*> songCards;
    QList<ArtistCardWidget*> artistCards;

    // 🔹 Hash para almacenar canciones con su ID único
    QHash<QString, SongData> songHash;
};

#endif // ADMINMENUUI_H
