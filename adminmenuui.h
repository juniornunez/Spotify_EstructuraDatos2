#ifndef ADMINMENUUI_H
#define ADMINMENUUI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVector>

class SongCardWidget;

class AdminMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMenuUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent = nullptr);
    ~AdminMenuUI();

private:
    QString adminUsername;
    QVector<SongCardWidget*> songCards;
    SongCardWidget* currentSelectedCard = nullptr;

    QVBoxLayout *sidebarLayout;
    QHBoxLayout *topBarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *mainLayout;
    QListWidget *playlistList;
    QPushButton *addPlaylistButton;
    QPushButton *profilePicButton;
    QLineEdit *searchBar;
    QLabel *homeIconLabel;
    QHBoxLayout *cardsLayout;
    QPushButton *artistSettingsButton;

private slots:
    void onAddPlaylistClicked();
    void onArtistSettingsClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
};

#endif // ADMINMENUUI_H
