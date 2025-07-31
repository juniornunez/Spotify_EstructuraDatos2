#include "loginUI.h"
#include <QPixmap>
#include <QFont>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include "MainWindowUI.h"

LoginUI::LoginUI(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #191414;");

    // Logo Spotify
    logoLabel = new QLabel;
    QPixmap logo("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/logo/logo.png"); // Usa recursos (ajusta la ruta si es necesario)
    logoLabel->setPixmap(logo.scaled(56, 56, Qt::KeepAspectRatio, Qt::SmoothTransformation));
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
        "background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;"
        );

    // Password
    passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("color: white; font-size: 12pt;");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet(
        "background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;"
        );

    // Botón verde Continue
    loginButton = new QPushButton("Continue");
    loginButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: black; font-size: 14pt; border: none; padding: 10px; border-radius: 24px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1fdf64; }"
        );
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
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debes ingresar username y contraseña.");
        return;
    }

    bool isAdmin = false;
    QString profilePicPath;
    QString dataDir;

    // 1. Buscar en userdata (usuarios normales)
    dataDir = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/userdata/" + username;
    if (QDir(dataDir).exists()) {
        QFile binFile(dataDir + "/info.bin");
        if (binFile.open(QIODevice::ReadOnly)) {
            QDataStream in(&binFile);
            in.setVersion(QDataStream::Qt_5_15);
            QString uname, pass, imgPath;
            int edad, type;
            in >> uname >> pass >> edad >> imgPath >> type;
            binFile.close();
            if (uname == username && pass == password) {
                profilePicPath = imgPath;
                isAdmin = false;
            } else {
                QMessageBox::warning(this, "Error", "Credenciales incorrectas.");
                return;
            }
        } else {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo de usuario.");
            return;
        }
    }
    // 2. Buscar en admindata (admins/artistas)
    else {
        dataDir = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/" + username;
        if (QDir(dataDir).exists()) {
            QFile binFile(dataDir + "/info.bin");
            if (binFile.open(QIODevice::ReadOnly)) {
                QDataStream in(&binFile);
                in.setVersion(QDataStream::Qt_5_15);
                int uniqueId;
                QString uname, pass, artisticName, realName, country, genre, bio, fecha, imgPath;
                bool activo;
                in >> uniqueId >> uname >> pass >> artisticName >> realName >> country >> genre >> bio >> fecha >> imgPath >> activo;
                binFile.close();
                if (uname == username && pass == password && activo) {
                    profilePicPath = imgPath;
                    isAdmin = true;
                } else {
                    QMessageBox::warning(this, "Error", "Credenciales incorrectas o usuario desactivado.");
                    return;
                }
            } else {
                QMessageBox::warning(this, "Error", "No se pudo abrir el archivo de admin.");
                return;
            }
        } else {
            QMessageBox::warning(this, "Error", "Usuario no encontrado.");
            return;
        }
    }

    // Login exitoso
    QMessageBox::information(this, "Login exitoso", "Bienvenido, " + username);

    // Abre el menú principal y pasa el tipo de usuario
    MainWindowUI *mainMenu = new MainWindowUI(profilePicPath, isAdmin);
    mainMenu->show();
    close();
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
