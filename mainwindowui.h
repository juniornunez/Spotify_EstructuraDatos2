#ifndef MAINWINDOWUI_H
#define MAINWINDOWUI_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>

class MainWindowUI : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindowUI(const QString &profilePicPath, bool isAdmin, QWidget *parent = nullptr);
    ~MainWindowUI();

private slots:
    void onAddPlaylistClicked();
    void onUploadSongClicked();
    void onUploadAlbumClicked();

private:
    // Barra lateral
    QVBoxLayout *sidebarLayout;
    QPushButton *addPlaylistButton;
    QListWidget *playlistList;

    // Barra superior
    QLabel *homeIconLabel;
    QLineEdit *searchBar;
    QLabel *profilePicLabel;

    // Layouts
    QHBoxLayout *mainLayout;
    QVBoxLayout *mainPanelLayout;
    QHBoxLayout *topBarLayout;

    // Botones extra de admin
    QPushButton *uploadSongButton;
    QPushButton *uploadAlbumButton;
};

#endif // MAINWINDOWUI_H
