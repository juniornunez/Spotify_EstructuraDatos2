#include "registerAdminUI.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QMessageBox>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QIntValidator>
#include <cstdlib>
#include <ctime>
#include <QFileDialog>

RegisterAdminUI::RegisterAdminUI(QWidget *parent)
    : QWidget(parent)
{
    adminsBasePath = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata";
    setStyleSheet("background-color: #191414;");

    // Logo
    logoLabel = new QLabel;
    QPixmap logo("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/logo/logo.png");
    logoLabel->setPixmap(logo.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    logoLabel->setAlignment(Qt::AlignCenter);

    // Título
    titleLabel = new QLabel("Admin/Artist Registration");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: white;");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Username
    usernameLabel = new QLabel("Username:");
    usernameLabel->setStyleSheet("color: white; font-size: 12pt;");
    usernameEdit = new QLineEdit;
    usernameEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Password
    passwordLabel = new QLabel("Password:");
    passwordLabel->setStyleSheet("color: white; font-size: 12pt;");
    passwordEdit = new QLineEdit;
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Artistic Name
    artisticNameLabel = new QLabel("Artistic name:");
    artisticNameLabel->setStyleSheet("color: white; font-size: 12pt;");
    artisticNameEdit = new QLineEdit;
    artisticNameEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Real Name
    realNameLabel = new QLabel("Real name:");
    realNameLabel->setStyleSheet("color: white; font-size: 12pt;");
    realNameEdit = new QLineEdit;
    realNameEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Country
    countryLabel = new QLabel("Country:");
    countryLabel->setStyleSheet("color: white; font-size: 12pt;");
    countryEdit = new QComboBox;
    countryEdit -> addItems(
    {
        "Afganistán", "Albania", "Alemania", "Andorra", "Angola", "Antigua y Barbuda", "Arabia Saudita", "Argelia", "Argentina",
        "Armenia", "Australia", "Austria", "Azerbaiyán", "Bahamas", "Bangladés", "Baréin", "Barbados", "Bélgica",
        "Belice", "Benín", "Bielorrusia", "Birmania", "Bolivia", "Bosnia y Herzegovina", "Botsuana", "Brasil",
        "Brunéi", "Bulgaria", "Burkina Faso", "Burundi", "Bután", "Cabo Verde", "Camboya", "Camerún", "Canadá",
        "Catar", "Chad", "Chile", "China", "Chipre", "Ciudad del Vaticano", "Colombia", "Comoras", "Corea del Norte",
        "Corea del Sur", "Costa de Marfil", "Costa Rica", "Croacia", "Cuba", "Dinamarca", "Dominica", "Ecuador",
        "Egipto", "El Salvador", "Emiratos Árabes Unidos", "Eritrea", "Eslovaquia", "Eslovenia", "España", "Estados Unidos",
        "Estonia", "Etiopía", "Filipinas", "Finlandia", "Fiyi", "Francia", "Gabón", "Gambia", "Georgia", "Ghana",
        "Granada", "Grecia", "Guatemala", "Guyana", "Guinea", "Guinea-Bisáu", "Guinea Ecuatorial", "Haití", "Honduras",
        "Hungría", "India", "Indonesia", "Irak", "Irán", "Irlanda", "Islandia", "Islas Marshall", "Islas Salomón",
        "Israel", "Italia", "Jamaica", "Japón", "Jordania", "Kazajistán", "Kenia", "Kirguistán", "Kiribati", "Kuwait",
        "Laos", "Lesoto", "Letonia", "Líbano", "Liberia", "Libia", "Liechtenstein", "Lituania", "Luxemburgo", "Macedonia del Norte",
        "Madagascar", "Malasia", "Malaui", "Maldivas", "Malí", "Malta", "Marruecos", "Mauricio", "Mauritania", "México",
        "Micronesia", "Moldavia", "Mónaco", "Mongolia", "Montenegro", "Mozambique", "Namibia", "Nauru", "Nepal",
        "Nicaragua", "Níger", "Nigeria", "Noruega", "Nueva Zelanda", "Omán", "Países Bajos", "Pakistán", "Palaos",
        "Panamá", "Papúa Nueva Guinea", "Paraguay", "Perú", "Polonia", "Portugal", "Reino Unido", "República Centroafricana",
        "República Checa", "República del Congo", "República Democrática del Congo", "República Dominicana", "Ruanda",
        "Rumanía", "Rusia", "Samoa", "San Cristóbal y Nieves", "San Marino", "San Vicente y las Granadinas", "Santa Lucía",
        "Santo Tomé y Príncipe", "Senegal", "Serbia", "Seychelles", "Sierra Leona", "Singapur", "Siria", "Somalia",
        "Sri Lanka", "Suazilandia", "Sudáfrica", "Sudán", "Sudán del Sur", "Suecia", "Suiza", "Surinam", "Tailandia",
        "Tanzania", "Tayikistán", "Timor Oriental", "Togo", "Tonga", "Trinidad y Tobago", "Túnez", "Turkmenistán", "Turquía",
        "Tuvalu", "Ucrania", "Uganda", "Uruguay", "Uzbekistán", "Vanuatu", "Venezuela", "Vietnam", "Yemen", "Yibuti",
        "Zambia", "Zimbabue"
    });

    countryEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; padding: 8px; font-size: 12pt; border-radius: 6px;");

    // Genre
    genreLabel = new QLabel("Main genre:");
    genreLabel->setStyleSheet("color: white; font-size: 12pt;");
    genreCombo = new QComboBox;
    genreCombo->addItems({"Pop","Corridos","Cristianos","Electrónica","Reguetón","Rock","Clásicas"});
    genreCombo->setStyleSheet("background-color: #222; color: white; border: 1px solid #535353; padding: 6px; font-size: 12pt; border-radius: 6px;");

    // Bio
    bioLabel = new QLabel("Bio:");
    bioLabel->setStyleSheet("color: white; font-size: 12pt;");
    bioEdit = new QTextEdit;
    bioEdit->setStyleSheet("background-color: #121212; color: white; border: 1px solid #535353; font-size: 12pt; border-radius: 6px;");
    bioEdit->setFixedHeight(60);

    // Image
    imageLabel = new QLabel("Profile image:");
    imageLabel->setStyleSheet("color: white; font-size: 12pt;");
    imagePathEdit = new QLineEdit;
    imagePathEdit->setReadOnly(true);
    imagePathEdit->setStyleSheet("background-color: #222; color: #eee; border: 1px solid #535353; padding: 7px; font-size: 11pt; border-radius: 5px;");
    selectImageButton = new QPushButton("Select image");
    selectImageButton->setStyleSheet(
        "QPushButton { background: #222; color: #1ED760; font-weight: bold; border-radius: 8px; padding: 6px 14px; }"
        "QPushButton:hover { background: #2a2a2a; }"
        );
    connect(selectImageButton, &QPushButton::clicked, this, &RegisterAdminUI::onSelectImageClicked);

    // Create button
    createAdminButton = new QPushButton("Create admin/artist");
    createAdminButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: black; font-size: 14pt; border: none; padding: 10px; border-radius: 24px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1fdf64; }"
        );
    connect(createAdminButton, &QPushButton::clicked, this, &RegisterAdminUI::onCreateAdminClicked);

    // Login link
    loginLabel = new QLabel("Already have an account?");
    loginLabel->setStyleSheet("color: white; font-size: 11pt;");
    loginLink = new QLabel("<a style='color:#1DB954;text-decoration: underline;' href='#'>Log in to Musicfy</a>");
    loginLink->setStyleSheet("color: #1DB954; font-size: 11pt;");
    loginLink->setTextFormat(Qt::RichText);
    loginLink->setTextInteractionFlags(Qt::TextBrowserInteraction);
    loginLink->setOpenExternalLinks(false);
    connect(loginLink, &QLabel::linkActivated, this, &RegisterAdminUI::onGoToLoginClicked);

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
    mainLayout->addSpacing(12);

    mainLayout->addWidget(usernameLabel);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(artisticNameLabel);
    mainLayout->addWidget(artisticNameEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(realNameLabel);
    mainLayout->addWidget(realNameEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(countryLabel);
    mainLayout->addWidget(countryEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(genreLabel);
    mainLayout->addWidget(genreCombo);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(bioLabel);
    mainLayout->addWidget(bioEdit);
    mainLayout->addSpacing(7);

    mainLayout->addWidget(imageLabel);
    mainLayout->addLayout(imageLayout);
    mainLayout->addSpacing(15);

    mainLayout->addWidget(createAdminButton);
    mainLayout->addSpacing(20);

    mainLayout->addLayout(loginLayout);
    mainLayout->addSpacing(6);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setFixedSize(420, 800);
    setWindowTitle("Musicfy - Admin/Artist Registration");
}

RegisterAdminUI::~RegisterAdminUI() {}

void RegisterAdminUI::onSelectImageClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Select Profile Image");
    if (!file.isEmpty()) {
        imagePathEdit->setText(file);
        selectedImagePath = file;
    }
}

bool RegisterAdminUI::usernameExists(const QString &username)
{
    QDir baseDir(adminsBasePath);
    return baseDir.exists(username);
}

void RegisterAdminUI::onCreateAdminClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QString artisticName = artisticNameEdit->text().trimmed();
    QString realName = realNameEdit->text().trimmed();
    QString country = countryEdit->currentText();
    QString genre = genreCombo->currentText();
    QString bio = bioEdit->toPlainText().trimmed();
    QString imagePath = imagePathEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty() || artisticName.isEmpty() ||
        realName.isEmpty() || country.isEmpty() || genre.isEmpty() || bio.isEmpty() || imagePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required.");
        return;
    }
    if (usernameExists(username)) {
        QMessageBox::warning(this, "Error", "Username already exists. Choose another.");
        return;
    }

    // Crear carpeta de admin
    QDir().mkpath(adminsBasePath + "/" + username);

    // Copiar imagen
    QFileInfo imgInfo(imagePath);
    QString imageDest = adminsBasePath + "/" + username + "/profile" + imgInfo.suffix().prepend(".");
    if (!QFile::copy(imagePath, imageDest)) {
        QMessageBox::warning(this, "Error", "Couldn't copy image.");
        return;
    }

    // Guardar datos binarios de admin
    QString binPath = adminsBasePath + "/" + username + "/info.bin";
    QFile binFile(binPath);
    if (!binFile.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Couldn't create admin data.");
        return;
    }
    QDataStream out(&binFile);
    out.setVersion(QDataStream::Qt_5_15);

    // ID automático usando std::rand
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    int uniqueId = std::rand();
    QString fecha = QDateTime::currentDateTime().toString(Qt::ISODate);

    out << uniqueId << username << password << artisticName << realName << country << genre << bio << fecha << imageDest << true; // activo=true
    binFile.close();

    QMessageBox::information(this, "Success", "Admin/Artist account created successfully!");
    emit adminAccountCreated();
    close();
}

void RegisterAdminUI::onGoToLoginClicked()
{
    emit goToLogin();
    this->close();
}
