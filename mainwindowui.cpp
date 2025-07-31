#include "mainwindowUI.h"
#include <QIcon>
#include <QFileDialog>
#include <QSpacerItem>
#include <QInputDialog>
#include <QSizePolicy>
#include <QMessageBox>

MainWindowUI::MainWindowUI(const QString &profilePicPath, bool isAdmin, QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #191414; color: white;");

    // ----- SIDEBAR -----
    sidebarLayout = new QVBoxLayout;
    sidebarLayout->setSpacing(8);
    sidebarLayout->setContentsMargins(4, 10, 4, 10);

    // "+" botón para nueva playlist
    addPlaylistButton = new QPushButton("+");
    addPlaylistButton->setFixedSize(32, 32);
    addPlaylistButton->setStyleSheet(
        "QPushButton { background-color: #282828; color: #1DB954; font-size: 20pt; border: none; border-radius: 12px; font-weight: bold; }"
        "QPushButton:hover { background-color: #222; color: #fff; }"
        );
    connect(addPlaylistButton, &QPushButton::clicked, this, &MainWindowUI::onAddPlaylistClicked);
    sidebarLayout->addWidget(addPlaylistButton, 0, Qt::AlignTop | Qt::AlignHCenter);

    // Lista de playlists
    playlistList = new QListWidget;
    playlistList->setStyleSheet(
        "QListWidget { background-color: #191414; color: white; border: none; font-size: 11pt; }"
        "QListWidget::item:selected { background: #282828; color: #1DB954; }"
        );
    sidebarLayout->addWidget(playlistList);

    // ----- TOP BAR -----
    topBarLayout = new QHBoxLayout;
    topBarLayout->setContentsMargins(10, 5, 10, 5);
    topBarLayout->setSpacing(18);

    // Icono de casita para Home
    homeIconLabel = new QLabel("🏠");
    QFont iconFont = homeIconLabel->font();
    iconFont.setPointSize(20);
    homeIconLabel->setFont(iconFont);
    homeIconLabel->setFixedWidth(36);
    topBarLayout->addWidget(homeIconLabel, 0, Qt::AlignLeft);

    // ----- BOTONES ADMIN EN TOPBAR -----
    uploadSongButton = new QPushButton("Subir Canción");
    uploadSongButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: #191414; font-weight: bold; border: none; padding: 8px 18px; border-radius: 18px; font-size: 11pt; }"
        "QPushButton:hover { background-color: #1fdf64; }"
        );
    connect(uploadSongButton, &QPushButton::clicked, this, &MainWindowUI::onUploadSongClicked);

    uploadAlbumButton = new QPushButton("Subir Álbum");
    uploadAlbumButton->setStyleSheet(
        "QPushButton { background-color: #1ED760; color: #191414; font-weight: bold; border: none; padding: 8px 18px; border-radius: 18px; font-size: 11pt; }"
        "QPushButton:hover { background-color: #1fdf64; }"
        );
    connect(uploadAlbumButton, &QPushButton::clicked, this, &MainWindowUI::onUploadAlbumClicked);

    // SOLO LOS ADMINS VEN ESTOS BOTONES EN LA BARRA
    if (isAdmin) {
        topBarLayout->addWidget(uploadSongButton, 0, Qt::AlignLeft);
        topBarLayout->addWidget(uploadAlbumButton, 0, Qt::AlignLeft);
    }

    // Barra de búsqueda (centrada)
    searchBar = new QLineEdit;
    searchBar->setPlaceholderText("What do you want to play?");
    searchBar->setStyleSheet(
        "QLineEdit { background-color: #222; color: white; border: none; padding: 8px 16px; border-radius: 18px; font-size: 13pt; }"
        );
    topBarLayout->addWidget(searchBar, 1);

    // Espaciador para empujar foto de perfil a la derecha
    QSpacerItem *spacer = new QSpacerItem(24, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);
    topBarLayout->addItem(spacer);

    // Foto de perfil
    profilePicLabel = new QLabel;
    QPixmap profilePic(profilePicPath);
    if (!profilePic.isNull())
        profilePicLabel->setPixmap(profilePic.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else
        profilePicLabel->setText("👤");
    profilePicLabel->setFixedSize(40, 40);
    profilePicLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    profilePicLabel->setStyleSheet("border-radius: 18px; background: #282828;");
    topBarLayout->addWidget(profilePicLabel, 0, Qt::AlignRight);

    // ----- PANEL PRINCIPAL -----
    mainPanelLayout = new QVBoxLayout;
    mainPanelLayout->addLayout(topBarLayout);
    mainPanelLayout->addSpacing(18);

    // Aquí puedes añadir tus widgets centrales (carátulas, recomendaciones, etc)
    // mainPanelLayout->addWidget(...);

    // ----- LAYOUT PRINCIPAL -----
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setLayout(sidebarLayout);
    sidebarWidget->setFixedWidth(74);
    sidebarWidget->setStyleSheet("background: #121212;");

    mainLayout->addWidget(sidebarWidget);
    mainLayout->addLayout(mainPanelLayout, 1);

    setLayout(mainLayout);
    setMinimumSize(1000, 600);
    setWindowTitle("Musicfy - Main Menu");
}

MainWindowUI::~MainWindowUI() {}

void MainWindowUI::onAddPlaylistClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Nueva Playlist", "Nombre de la playlist:", QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty()) {
        playlistList->addItem(name.trimmed());
    }
}

void MainWindowUI::onUploadSongClicked()
{
    QMessageBox::information(this, "Subir Canción", "Aquí irá la lógica para subir una canción (sólo admins).");
}

void MainWindowUI::onUploadAlbumClicked()
{
    QMessageBox::information(this, "Subir Álbum", "Aquí irá la lógica para subir un álbum (sólo admins).");
}
