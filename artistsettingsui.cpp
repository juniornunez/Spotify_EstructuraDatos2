#include "artistsettingsui.h"
#include <QVBoxLayout>
#include <QFont>

ArtistSettingsUI::ArtistSettingsUI(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #191414;");

    titleLabel = new QLabel("Artist Settings");
    titleLabel->setStyleSheet("color: white;");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(19);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    singleButton = new QPushButton("Subir Single");
    singleButton->setStyleSheet(
        "QPushButton {"
        "background-color: #1DB954;"
        "color: black;"
        "font-size: 10pt;"
        "border: none;"
        "padding: 16px;"
        "border-radius: 18px;"
        "margin: 12px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #1ED760;"
        "}"
        );

    albumButton = new QPushButton("Subir Álbum");
    albumButton->setStyleSheet(singleButton->styleSheet());

    epButton = new QPushButton("Subir EP");
    epButton->setStyleSheet(singleButton->styleSheet());

    manageMusicButton = new QPushButton("Gestionar mi música");
    manageMusicButton->setStyleSheet(
        "QPushButton {"
        "background-color: #282828;"
        "color: white;"
        "font-size: 10pt;"
        "border: 1.5px solid #1DB954;"
        "padding: 16px;"
        "border-radius: 18px;"
        "margin: 12px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #444;"
        "color: #1DB954;"
        "}"
        );

    connect(singleButton, &QPushButton::clicked, this, &ArtistSettingsUI::uploadSingleClicked);
    connect(albumButton, &QPushButton::clicked, this, &ArtistSettingsUI::uploadAlbumClicked);
    connect(epButton, &QPushButton::clicked, this, &ArtistSettingsUI::uploadEPClicked);
    connect(manageMusicButton, &QPushButton::clicked, this, &ArtistSettingsUI::manageMusicClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(34);
    mainLayout->addWidget(singleButton);
    mainLayout->addWidget(albumButton);
    mainLayout->addWidget(epButton);
    mainLayout->addWidget(manageMusicButton);
    mainLayout->addSpacing(38);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setFixedSize(420, 420);
    setWindowTitle("Artist Settings");
}

ArtistSettingsUI::~ArtistSettingsUI() {}
