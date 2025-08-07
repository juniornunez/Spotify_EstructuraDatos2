#ifndef USERPROFILEUI_H
#define USERPROFILEUI_H

#include <QWidget>
#include <QLabel>

class UserProfileUI : public QWidget
{
    Q_OBJECT
public:
    explicit UserProfileUI(const QString &username, QWidget *parent = nullptr);
    ~UserProfileUI();

private:
    void loadUserData(const QString &username);
    QString findProfilePic(const QString &username);

    QLabel *profilePicLabel;
    QLabel *usernameLabel;
    QLabel *ageLabel;

    QString m_username;
    QString m_profilePicPath;
    int m_age = -1;
};

#endif // USERPROFILEUI_H
