#ifndef ADMINMENUUI_H
#define ADMINMENUUI_H

#include "usermenuui.h"
#include <QPushButton>

class AdminMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit AdminMenuUI(const QString &profilePicPath, QWidget *parent = nullptr);
    ~AdminMenuUI();

private:
    QVBoxLayout *sidebarLayout;
    QHBoxLayout *topBarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *mainLayout;
    QListWidget *playlistList;
    QPushButton *addPlaylistButton;
    QPushButton *profilePicButton;    // <-- Cambiado!
    QLineEdit *searchBar;
    QLabel *homeIconLabel;

    QPushButton *artistSettingsButton;

private slots:
    void onAddPlaylistClicked();
    void onArtistSettingsClicked();
    void onProfilePicClicked(); // por si lo quieres usar después
};

#endif // ADMINMENUUI_H
