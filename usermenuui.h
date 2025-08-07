#ifndef USERMENUUI_H
#define USERMENUUI_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "songcardwidget.h"
#include "playbarui.h"

class UserMenuUI : public QWidget
{
    Q_OBJECT

public:
    UserMenuUI(const QString &profilePicPath, const QString &username, QWidget *parent = nullptr);
    ~UserMenuUI();

private slots:
    void onAddPlaylistClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
    void handlePlayButtonPressed(const QString &audioPath, const QString &cover, const QString &title, const QString &artist);

private:
    QString m_username;
    QString m_profilePicPath;

    QVBoxLayout *sidebarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *mainLayout;
    QHBoxLayout *topBarLayout;
    QHBoxLayout *cardsLayout;

    QListWidget *playlistList;
    QPushButton *addPlaylistButton;
    QPushButton *profilePicButton;
    QLabel *homeIconLabel;
    QLineEdit *searchBar;
    QPushButton *trendingButton;
    QList<SongCardWidget*> songCards;
    PlayBarUI *playBar;
};

#endif // USERMENUUI_H
