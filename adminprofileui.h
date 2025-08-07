#ifndef ADMINPROFILEUI_H
#define ADMINPROFILEUI_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QScrollArea>
#include <QFrame>

class AdminProfileUI : public QWidget
{
    Q_OBJECT

public:
    explicit AdminProfileUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent = nullptr);

private:
    void setupTopBar();
    void setupMusicAndAlbumsTab();
    void setupArtistProfileTab();
    void loadAdminData(const QString &adminUsername);

    QLabel *profilePicLabel;
    QLabel *adminNameLabel;

    QPushButton *tabMusicButton;
    QPushButton *tabProfileButton;
    QStackedWidget *mainStack;

    // Music & Albums tab
    QWidget *musicAndAlbumsWidget;
    QVBoxLayout *musicAndAlbumsLayout;
    QListWidget *songsList;

    // Albums list, add later if you want custom widgets
    QWidget *albumsWidget;
    QHBoxLayout *albumsLayout;

    // Artist profile tab
    QWidget *artistProfileWidget;
    QVBoxLayout *artistProfileLayout;
    QLabel *profilePicBig;
    QLabel *realNameValue;
    QLabel *stageNameValue;
    QLabel *countryValue;
    QLabel *bioValue;

    // Data
    QString m_profilePicPath;
    QString m_adminUsername;
    QString m_realName;
    QString m_stageName;
    QString m_country;
    QString m_bio;

private slots:
    void onTabMusicClicked();
    void onTabProfileClicked();
};

#endif // ADMINPROFILEUI_H
