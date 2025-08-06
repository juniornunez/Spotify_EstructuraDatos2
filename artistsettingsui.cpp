#include "artistsettingsui.h"
#include <QVBoxLayout>
#include <QFont>
#include "addsingleui.h"

ArtistSettingsUI::ArtistSettingsUI(const QString &adminUsername, QWidget *parent)
    : QDialog(parent), adminUsername(adminUsername)
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

    // Solo esta conexión para abrir el popup de AddSingleUI
    connect(singleButton, &QPushButton::clicked, this, [=]() {
        AddSingleUI *addSingle = new AddSingleUI(adminUsername);
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Subir Single");
        dialog->setModal(true);
        dialog->setStyleSheet("background-color: #191414;");

        QVBoxLayout *dlgLayout = new QVBoxLayout(dialog);
        dlgLayout->addWidget(addSingle);

        // Cuando termine de agregar canción, cierra el dialog y manda la señal hacia arriba si la quieres usar afuera
        connect(addSingle, &AddSingleUI::songAdded, this, [=](const QString &title, const QString &coverPath, const QString &artist, const QString &audioPath){
            emit songUploaded(title, coverPath, artist, audioPath); // ¡Ahora pasa el cuarto parámetro!
            dialog->accept();
            dialog->deleteLater();
        });


        dialog->exec();
    });

    albumButton = new QPushButton("Subir Álbum");
    albumButton->setStyleSheet(singleButton->styleSheet());
    connect(albumButton, &QPushButton::clicked, this, &ArtistSettingsUI::uploadAlbumClicked);

    epButton = new QPushButton("Subir EP");
    epButton->setStyleSheet(singleButton->styleSheet());
    connect(epButton, &QPushButton::clicked, this, &ArtistSettingsUI::uploadEPClicked);

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
