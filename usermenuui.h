#ifndef USERMENUUI_H
#define USERMENUUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>

class UserMenuUI : public QWidget
{
    Q_OBJECT

public:
    explicit UserMenuUI(const QString &profilePicPath, QWidget *parent = nullptr);
    ~UserMenuUI();

protected:
    QVBoxLayout *sidebarLayout;
    QHBoxLayout *topBarLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *mainLayout;
    QListWidget *playlistList;
    QPushButton *addPlaylistButton;
    QPushButton *profilePicButton; // Cambiado!
    QLineEdit *searchBar;
    QLabel *homeIconLabel;

protected slots:
    void onAddPlaylistClicked();
    void onProfilePicClicked();
};

#endif // USERMENUUI_H

