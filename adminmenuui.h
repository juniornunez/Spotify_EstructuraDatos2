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
class PlayBarUI;

class AdminMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMenuUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent = nullptr);
    ~AdminMenuUI();
    QString profilePicPath;
private slots:
    void onAddPlaylistClicked();
    void onArtistSettingsClicked();
    void onProfilePicClicked();
    void handleCardToggled(SongCardWidget* card, bool nowSelected);
    void handlePlayButtonPressed(SongCardWidget* card);

private:

    QString adminUsername;
    QVector<SongCardWidget*> songCards;
    QVBoxLayout *sidebarLayout;
    QHBoxLayout *topBarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *mainLayout;
    QHBoxLayout *cardsLayout;
    QListWidget *playlistList;
    QPushButton *addPlaylistButton;
    QPushButton *profilePicButton;
    QLineEdit *searchBar;
    QLabel *homeIconLabel;
    QPushButton *artistSettingsButton;

    SongCardWidget* currentSelectedCard = nullptr;
    PlayBarUI* playBar = nullptr;
};

#endif // ADMINMENUUI_H
