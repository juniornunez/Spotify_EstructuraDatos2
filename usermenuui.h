#ifndef USERMENUUI_H
#define USERMENUUI_H

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVector>
#include "songcardwidget.h"

class UserMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit UserMenuUI(const QString &profilePicPath, QWidget *parent = nullptr);
    ~UserMenuUI();

private slots:
    void onAddPlaylistClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);

private:
    QVBoxLayout *sidebarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *mainLayout;
    QHBoxLayout *topBarLayout;
    QHBoxLayout *cardsLayout;
    QVector<SongCardWidget*> songCards;
    QLabel *homeIconLabel;
    QLineEdit *searchBar;
    QPushButton *profilePicButton;
    QPushButton *addPlaylistButton;
    QListWidget *playlistList;
};

#endif // USERMENUUI_H
