#include "loginUI.h"
#include <QPixmap>
#include <QFont>
#include <QMessageBox>
#include <QSpacerItem>
#include <QDesktopServices>
#include <QUrl>

LoginUI::LoginUI(QWidget *parent)
    : QWidget(parent)
{
    // Fondo negro total
    setStyleSheet("background-color: #191414;");

    // Logo Spotify
    logoLabel = new QLabel;
    QPixmap logo("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/logo/logo.png");
    logoLabel->setPixmap(logo.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);

    // Título
    titleLabel = new QLabel("Log in to Musicfy");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: white;");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Username
    usernameLabel = new QLabel("Username:");
    usernameLabel->setStyleSheet("color: white; font-size: 12pt;");
    usernameEdit = new QLineEdit;
    usernameEdit->setStyleSheet(
        "background-color: #121212;"
        "color: white;"
        "border: 1px solid #535353;"
        "padding: 8px;"
        "font-size: 12pt;"
        "border-radius: 6px;"
        );

    // Password
    passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("color: white; font-size: 12pt;");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet(
        "background-color: #121212;"
        "color: white;"
        "border: 1px solid #535353;"
        "padding: 8px;"
        "font-size: 12pt;"
        "border-radius: 6px;"
        );

    // Botón verde Continue
    loginButton = new QPushButton("Continue");
    loginButton->setStyleSheet(
        "QPushButton {"
        "background-color: #1ED760;"
        "color: black;"
        "font-size: 14pt;"
        "border: none;"
        "padding: 10px;"
        "border-radius: 24px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #1fdf64;"
        "}"
        );

    // Señales
    connect(loginButton, &QPushButton::clicked, this, &LoginUI::onLoginButtonClicked);

    // "Don't have an account?" + link
    signupLabel = new QLabel("Don't have an account?");
    signupLabel->setStyleSheet("color: white; font-size: 11pt;");
    signupLink = new QLabel("<a style='color:#1DB954;text-decoration: underline;' href='#'>Sign up for musicfy</a>");
    signupLink->setStyleSheet("color: #1DB954; font-size: 11pt;");
    signupLink->setTextFormat(Qt::RichText);
    signupLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    signupLink->setOpenExternalLinks(false);
    connect(signupLink, &QLabel::linkActivated, this, &LoginUI::onSignUpClicked);

    // Layout para link y texto
    QHBoxLayout *signupLayout = new QHBoxLayout;
    signupLayout->addWidget(signupLabel);
    signupLayout->addWidget(signupLink);
    signupLayout->addStretch();

    // Layout vertical principal
    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addStretch();
    mainLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(30);

    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addSpacing(12);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addSpacing(24);

    mainLayout->addWidget(loginButton);
    mainLayout->addSpacing(20);

    mainLayout->addLayout(signupLayout);
    mainLayout->addSpacing(10);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setFixedSize(420, 540);
    setWindowTitle("Musicfy - Login");
}

LoginUI::~LoginUI() {}

void LoginUI::onLoginButtonClicked()
{
    QString username = usernameEdit->text();
    QString password = passwordEdit->text();

    UserData user;
    if (user.authenticate(username.toStdString(), password.toStdString())) {
        currentUser = user;
        userType = user.getType();
        QMessageBox::information(this, "Login exitoso", "Bienvenido, " + username);
        emit loginSuccess(userType);
        close();
    } else {
        QMessageBox::warning(this, "Error", "Credenciales incorrectas.");
    }
}

void LoginUI::onSignUpClicked()
{
    // Evita abrir más de una ventana de pregunta
    if (!questionWindow) {
        questionWindow = new QuestionUI;
        // Si elige admin/artista
        connect(questionWindow, &QuestionUI::adminSelected, this, [this]() {
            if (!registerAdminWindow) {
                registerAdminWindow = new RegisterAdminUI;
                connect(registerAdminWindow, &RegisterAdminUI::goToLogin, this, [this]() {
                    this->show();
                    registerAdminWindow->deleteLater();
                    registerAdminWindow = nullptr;
                });
            }
            registerAdminWindow->show();
            questionWindow->close();
            questionWindow->deleteLater();
            questionWindow = nullptr;
            this->hide();
        });
        // Si elige usuario común
        connect(questionWindow, &QuestionUI::userSelected, this, [this]() {
            if (!registerWindow) {
                registerWindow = new RegisterUI;
                connect(registerWindow, &RegisterUI::goToLogin, this, [this]() {
                    this->show();
                    registerWindow->deleteLater();
                    registerWindow = nullptr;
                });
            }
            registerWindow->show();
            questionWindow->close();
            questionWindow->deleteLater();
            questionWindow = nullptr;
            this->hide();
        });
    }
    questionWindow->show();
    this->hide();
}


UserData LoginUI::getCurrentUser() const { return currentUser; }
int LoginUI::getUserType() const { return userType; }
