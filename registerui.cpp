#include "registerUI.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QIntValidator>
#include <QDebug>

RegisterUI::RegisterUI(QWidget *parent)
    : QWidget(parent)
{
    usersBasePath = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/userdata";

    setStyleSheet("background-color: #191414;");

    // Logo
    logoLabel = new QLabel;
    QPixmap logo("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/logo/logo.png");
    logoLabel->setPixmap(logo.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);

    // Título
    titleLabel = new QLabel("Sign up for Musicfy");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: white;");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(22);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Username
    usernameLabel = new QLabel("Username:");
    usernameLabel->setStyleSheet("color: white; font-size: 12pt;");
    usernameEdit = new QLineEdit;
    usernameEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Contraseña
    passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("color: white; font-size: 12pt;");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Edad
    ageLabel = new QLabel("Age:");
    ageLabel->setStyleSheet("color: white; font-size: 12pt;");
    ageEdit = new QLineEdit;
    ageEdit->setValidator(new QIntValidator(1, 200, this));
    ageEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Imagen
    imageLabel = new QLabel("Profile picture:");
    imageLabel->setStyleSheet("color: white; font-size: 12pt;");
    imagePathEdit = new QLineEdit;
    imagePathEdit->setReadOnly(true);
    imagePathEdit->setStyleSheet("background-color: #222; color: #eee; border: 1px solid #535353; padding: 7px; font-size: 11pt; border-radius: 5px;");
    selectImageButton = new QPushButton("Select image");
    selectImageButton->setStyleSheet(
        "QPushButton { background: #222; color: #1ED760; font-weight: bold; border-radius: 8px; padding: 6px 14px; }"
        "QPushButton:hover { background: #2a2a2a; }"
        );
    connect(selectImageButton, &QPushButton::clicked, this, &RegisterUI::onSelectImageClicked);

    // Tipo de usuario
    typeLabel = new QLabel("Account type:");
    typeLabel->setStyleSheet("color: white; font-size: 12pt;");
    typeCombo = new QComboBox;
    typeCombo->addItem("User");
    typeCombo->addItem("Admin");
    typeCombo->setStyleSheet("background-color: #222; color: white; border: 1px solid #535353; padding: 6px; font-size: 12pt; border-radius: 6px;");

    // Botón de crear cuenta
    createAccountButton = new QPushButton("Create account");
    createAccountButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: black; font-size: 14pt; border: none; padding: 10px; border-radius: 24px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1fdf64; }"
        );
    connect(createAccountButton, &QPushButton::clicked, this, &RegisterUI::onCreateAccountClicked);

    // "Already have an account?"
    loginLabel = new QLabel("Already have an account?");
    loginLabel->setStyleSheet("color: white; font-size: 11pt;");
    loginLink = new QLabel("<a style='color:#1DB954;text-decoration: underline;' href='#'>Log in to Musicfy</a>");
    loginLink->setStyleSheet("color: #1DB954; font-size: 11pt;");
    loginLink->setTextFormat(Qt::RichText);
    loginLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    loginLink->setOpenExternalLinks(false);
    connect(loginLink, &QLabel::linkActivated, this, &RegisterUI::onGoToLoginClicked);

    // Layouts
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->addWidget(imagePathEdit);
    imageLayout->addWidget(selectImageButton);

    QHBoxLayout *loginLayout = new QHBoxLayout;
    loginLayout->addWidget(loginLabel);
    loginLayout->addWidget(loginLink);
    loginLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(logoLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(22);

    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(ageLabel);
    mainLayout->addWidget(ageEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(imageLayout);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(typeLabel);
    mainLayout->addWidget(typeCombo);
    mainLayout->addSpacing(17);

    mainLayout->addWidget(createAccountButton);
    mainLayout->addSpacing(20);

    mainLayout->addLayout(loginLayout);
    mainLayout->addSpacing(6);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setFixedSize(420, 600);
    setWindowTitle("Musicfy - Register");
}

RegisterUI::~RegisterUI() {}

void RegisterUI::onSelectImageClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Profile Picture");
    if (!file.isEmpty()) {
        imagePathEdit->setText(file);
        selectedImagePath = file;
    }
}

bool RegisterUI::usernameExists(const QString &username)
{
    QDir baseDir(usersBasePath);
    return baseDir.exists(username);
}

void RegisterUI::onCreateAccountClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString ageStr = ageEdit->text().trimmed();
    QString imagePath = imagePathEdit->text().trimmed();
    QString accountType = typeCombo->currentText();

    // Validaciones
    if (username.isEmpty() || password.isEmpty() || ageStr.isEmpty() || imagePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required.");
        return;
    }
    bool ok;
    int age = ageStr.toInt(&ok);
    if (!ok || age <= 0) {
        QMessageBox::warning(this, "Error", "Age must be a positive number.");
        return;
    }
    if (usernameExists(username)) {
        QMessageBox::warning(this, "Error", "Username already exists. Choose another.");
        return;
    }

    // Crear carpeta usuario
    QDir().mkpath(usersBasePath + "/" + username);

    // Copiar imagen
    QFileInfo imgInfo(imagePath);
    QString imageDest = usersBasePath + "/" + username + "/profile" + imgInfo.suffix().prepend(".");
    if (!QFile::copy(imagePath, imageDest)) {
        QMessageBox::warning(this, "Error", "Couldn't copy image.");
        return;
    }

    // Guardar datos binarios
    QString binPath = usersBasePath + "/" + username + "/info.bin";
    QFile binFile(binPath);
    if (!binFile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Couldn't create user data.");
        return;
    }
    QDataStream out(&binFile);
    out.setVersion(QDataStream::Qt_5_15);
    out << username << password << age << imageDest << (accountType == "Admin" ? 1 : 2);
    binFile.close();

    QMessageBox::information(this, "Success", "Account created successfully!");
    emit accountCreated();
    close();
}

void RegisterUI::onGoToLoginClicked()
{
    emit goToLogin();
    close();
}
