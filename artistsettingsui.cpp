#include "artistsettingsui.h"
#include <QVBoxLayout>
#include <QFont>
#include "addsingleui.h"
#include "addalbumui.h"  // ✅ Importar nueva clase

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

    // ✅ Subir Single
    connect(singleButton, &QPushButton::clicked, this, [=]() {
        AddSingleUI *addSingle = new AddSingleUI(adminUsername);
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Subir Single");
        dialog->setModal(true);
        dialog->setStyleSheet("background-color: #191414;");

        QVBoxLayout *dlgLayout = new QVBoxLayout(dialog);
        dlgLayout->addWidget(addSingle);

        connect(addSingle, &AddSingleUI::songAdded, this, [=](const SongData &song) {
            emit songUploaded(song);
            dialog->accept();
            dialog->deleteLater();
        });

        dialog->exec();
    });

    albumButton = new QPushButton("Subir Álbum");
    albumButton->setStyleSheet(singleButton->styleSheet());

    // ✅ Subir Álbum
    connect(albumButton, &QPushButton::clicked, this, [=]() {
        AddAlbumUI *addAlbum = new AddAlbumUI(adminUsername);
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("Subir Álbum");
        dialog->setModal(true);
        dialog->setStyleSheet("background-color: #191414;");

        QVBoxLayout *dlgLayout = new QVBoxLayout(dialog);
        dlgLayout->addWidget(addAlbum);

        connect(addAlbum, &AddAlbumUI::albumAdded, this, [=](const QList<SongData> &songs) {
            emit albumUploaded(songs); // ✅ Nueva señal para álbumes
            dialog->accept();
            dialog->deleteLater();
        });

        dialog->exec();
    });

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
    connect(manageMusicButton, &QPushButton::clicked, this, [=]() {
        emit manageSongsRequested();
        this->close();
    });

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

void ArtistSettingsUI::uploadAlbumClicked() {
    // ya lo movimos al connect directo arriba
}

void ArtistSettingsUI::uploadEPClicked() {
    // TODO: lógica para subir EP
}

void ArtistSettingsUI::manageMusicClicked() {
    // ya lo hace con connect
}
