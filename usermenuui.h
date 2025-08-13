#ifndef USERMENUUI_H
#define USERMENUUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QLabel>
#include <QMenu>
#include <QHash>

#include "songdata.h"
#include "songcardwidget.h"
#include "artistcardwidget.h"
#include "playbarui.h"

class UserMenuUI : public QWidget
{
    Q_OBJECT
public:
    explicit UserMenuUI(const QString &profilePicPath, const QString &username, QWidget *parent = nullptr);
    ~UserMenuUI();

private slots:
    void onAddPlaylistClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
    void handlePlayButtonPressed(SongCardWidget* card);

private:
    // Datos del usuario
    QString username;
    QString profilePicPath;

    // Layouts y widgets
    QVBoxLayout *sidebarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *topBarLayout;
    QHBoxLayout *mainLayout;
    QHBoxLayout *cardsLayout;

    QPushButton *addPlaylistButton;
    QPushButton *trendingButton;
    QPushButton *profilePicButton;

    QListWidget *playlistList;
    QLineEdit *searchBar;
    QLabel *homeIconLabel;

    // Canciones y artistas
    QList<SongCardWidget*> songCards;
    QList<ArtistCardWidget*> artistCards;
    QHash<QString, SongData> songHash;

    // Reproducción
    SongCardWidget *currentSelectedCard = nullptr;
    PlayBarUI *playBar;
};

#endif // USERMENUUI_H
