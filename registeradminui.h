#ifndef REGISTERADMINUI_H
#define REGISTERADMINUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include "admindata.h"

class RegisterAdminUI : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterAdminUI(QWidget *parent = nullptr);
    ~RegisterAdminUI();

signals:
    void adminAccountCreated();
    void goToLogin();

private slots:
    void onSelectImageClicked();
    void onCreateAdminClicked();
    void onGoToLoginClicked();

private:
    QLabel *logoLabel;
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QLabel *artisticNameLabel;
    QLineEdit *artisticNameEdit;
    QLabel *realNameLabel;
    QLineEdit *realNameEdit;
    QLabel *countryLabel;
    QLineEdit *countryEdit;
    QLabel *genreLabel;
    QComboBox *genreCombo;
    QLabel *bioLabel;
    QTextEdit *bioEdit;
    QLabel *imageLabel;
    QLineEdit *imagePathEdit;
    QPushButton *selectImageButton;
    QPushButton *createAdminButton;
    QLabel *loginLabel;
    QLabel *loginLink;

    QString selectedImagePath;
    QString adminsBasePath;
    bool usernameExists(const QString &username);
};

#endif // REGISTERADMINUI_H
