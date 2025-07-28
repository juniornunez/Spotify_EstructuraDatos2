#ifndef REGISTERUI_H
#define REGISTERUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFileDialog>
#include "userdata.h"

class RegisterUI : public QWidget
{
    Q_OBJECT

public:
    explicit RegisterUI(QWidget *parent = nullptr);
    ~RegisterUI();

signals:
    void accountCreated();
    void goToLogin();

private slots:
    void onSelectImageClicked();
    void onCreateAccountClicked();
    void onGoToLoginClicked();

private:
    QLabel *logoLabel;
    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordEdit;
    QLabel *ageLabel;
    QLineEdit *ageEdit;
    QLabel *imageLabel;
    QLineEdit *imagePathEdit;
    QPushButton *selectImageButton;
    QLabel *typeLabel;
    QComboBox *typeCombo;
    QPushButton *createAccountButton;
    QLabel *loginLabel;
    QLabel *loginLink;

    QString selectedImagePath;
    QString usersBasePath;
    bool usernameExists(const QString &username);
};

#endif // REGISTERUI_H
