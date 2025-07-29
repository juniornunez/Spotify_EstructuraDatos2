#ifndef LOGINUI_H
#define LOGINUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "userdata.h"
#include "registerui.h"
#include "questionUI.h"
#include "registerAdminUI.h"

class LoginUI : public QWidget
{
    Q_OBJECT

public:
    explicit LoginUI(QWidget *parent = nullptr);
    ~LoginUI();

    UserData getCurrentUser() const;
    int getUserType() const;

signals:
    void loginSuccess(int userType);
    void signUpClicked();
    void openRegister();

private slots:
    void onLoginButtonClicked();
    void onSignUpClicked();


private:
    QLabel *logoLabel;
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QLabel *signupLabel;
    QLabel *signupLink;
    RegisterUI *registerWindow = nullptr;
    QuestionUI *questionWindow = nullptr;
    RegisterAdminUI *registerAdminWindow = nullptr;

    UserData currentUser;
    int userType = 0;
};

#endif // LOGINUI_H
