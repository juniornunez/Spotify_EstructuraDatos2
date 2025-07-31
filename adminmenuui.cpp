#include "adminmenuui.h"
#include <QInputDialog>
#include <QPixmap>
#include <QFont>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include "artistsettingsui.h"
#include <qpainter.h>

AdminMenuUI::AdminMenuUI(const QString &profilePicPath, QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #191414; color: white;");

    // -------- PANEL IZQUIERDO (sidebar) --------
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setFixedWidth(230);
    sidebarWidget->setStyleSheet("background: #121212;");

    sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(24, 18, 18, 24);
    sidebarLayout->setSpacing(16);

    // "My Library" + botón +
    QHBoxLayout *libraryBarLayout = new QHBoxLayout;
    QLabel *libraryLabel = new QLabel("My Library");
    QFont libraryFont = libraryLabel->font();
    libraryFont.setPointSize(18);
    libraryFont.setBold(true);
    libraryLabel->setFont(libraryFont);

    addPlaylistButton = new QPushButton("+");
    addPlaylistButton->setFixedSize(32, 32);
    addPlaylistButton->setStyleSheet(
        "QPushButton { background-color: #191414; color: #1ED760; font-size: 20pt; border: none; border-radius: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #2a2a2a; color: #fff; }"
        );
    connect(addPlaylistButton, &QPushButton::clicked, this, &AdminMenuUI::onAddPlaylistClicked);

    libraryBarLayout->addWidget(libraryLabel);
    libraryBarLayout->addStretch();
    libraryBarLayout->addWidget(addPlaylistButton);

    sidebarLayout->addLayout(libraryBarLayout);

    // Lista de playlists
    playlistList = new QListWidget;
    playlistList->setStyleSheet(
        "QListWidget { background-color: #121212; color: #bbb; border: none; font-size: 13pt; }"
        "QListWidget::item:selected { background: #282828; color: #1ED760; }"
        );
    sidebarLayout->addWidget(playlistList, 1);

    // Espaciador para empujar el botón Artist Settings hasta abajo
    sidebarLayout->addStretch();

    // Botón "Artist Settings"
    artistSettingsButton = new QPushButton("Artist Settings");
    artistSettingsButton->setStyleSheet(
        "QPushButton { background-color: #222; color: #1ED760; font-size: 13pt; border-radius: 16px; padding: 8px 0; font-weight: bold; }"
        "QPushButton:hover { background-color: #282828; color: #fff; }"
        );
    sidebarLayout->addWidget(artistSettingsButton);

    connect(artistSettingsButton, &QPushButton::clicked, this, &AdminMenuUI::onArtistSettingsClicked);



    // -------- PANEL DERECHO (main panel) --------
    QWidget *mainPanelWidget = new QWidget;
    mainPanelWidget->setStyleSheet("background: transparent;");
    mainPanelLayout = new QVBoxLayout(mainPanelWidget);
    mainPanelLayout->setContentsMargins(20, 20, 20, 20);
    mainPanelLayout->setSpacing(16);

    // ----- Top bar -----
    topBarLayout = new QHBoxLayout;
    topBarLayout->setSpacing(16);

    //Home
    homeIconLabel = new QLabel;
    QPixmap homeIconPixmap("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/homeicon.png");
    homeIconLabel->setPixmap(homeIconPixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    homeIconLabel->setFixedSize(44, 44);
    homeIconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    topBarLayout->addWidget(homeIconLabel, 0, Qt::AlignLeft);

    // Barra de búsqueda
    searchBar = new QLineEdit;
    searchBar->setPlaceholderText("What do you want to play?");
    searchBar->setStyleSheet(
        "QLineEdit { background-color: #222; color: white; border: none; padding: 10px 18px; border-radius: 18px; font-size: 15pt; min-width:300px; }"
        );
    topBarLayout->addWidget(searchBar, 1);

    // --- Foto de perfil como BOTÓN CIRCULAR ---
    profilePicButton = new QPushButton;
    profilePicButton->setFixedSize(48, 48);
    profilePicButton->setCursor(Qt::PointingHandCursor);
    profilePicButton->setStyleSheet(
        "QPushButton {"
        "border-radius: 24px;"
        "background: #282828;"
        "border: none;"
        "padding: 0;"
        "}"
        "QPushButton:focus { outline: none; }"
        );

    QPixmap profilePic(profilePicPath);
    if (!profilePic.isNull()) {
        QPixmap roundedPic(48, 48);
        roundedPic.fill(Qt::transparent);
        QPainter painter(&roundedPic);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 48, 48);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, profilePic.scaled(48, 48, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        painter.end();

        profilePicButton->setIcon(QIcon(roundedPic));
        profilePicButton->setIconSize(QSize(48, 48));
    } else {
        profilePicButton->setText("👤");
        profilePicButton->setStyleSheet(
            "QPushButton {"
            "border-radius: 24px; background: #282828; color: #999; font-size: 20pt; border: none; }"
            );
    }

    topBarLayout->addWidget(profilePicButton, 0, Qt::AlignRight);

    connect(profilePicButton, &QPushButton::clicked, this, &AdminMenuUI::onProfilePicClicked);

    // Añadir barra superior al panel principal
    mainPanelLayout->addLayout(topBarLayout);

    // ------------ NUEVO CONTENIDO PRINCIPAL ----------------
    // "My top songs"
    QLabel *topSongsLabel = new QLabel("My top songs");
    QFont topSongsFont = topSongsLabel->font();
    topSongsFont.setPointSize(22);
    topSongsFont.setBold(true);
    topSongsLabel->setFont(topSongsFont);
    topSongsLabel->setStyleSheet("color: white;");
    mainPanelLayout->addSpacing(35);
    mainPanelLayout->addWidget(topSongsLabel, 0, Qt::AlignLeft);

    // "Top artists"
    QLabel *topArtistsLabel = new QLabel("Top artists");
    QFont topArtistsFont = topArtistsLabel->font();
    topArtistsFont.setPointSize(22);
    topArtistsFont.setBold(true);
    topArtistsLabel->setFont(topArtistsFont);
    topArtistsLabel->setStyleSheet("color: white; margin-top:20px;");
    mainPanelLayout->addSpacing(170);
    mainPanelLayout->addWidget(topArtistsLabel, 0, Qt::AlignLeft);

    mainPanelLayout->addStretch();

    // ---- LAYOUT PRINCIPAL ----
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(sidebarWidget, 0);
    mainLayout->addWidget(mainPanelWidget, 1);

    setLayout(mainLayout);
    setMinimumSize(1050, 620);
    setWindowTitle("Musicfy - Admin Menu");
}

AdminMenuUI::~AdminMenuUI() {}

void AdminMenuUI::onAddPlaylistClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Nueva Playlist", "Nombre de la playlist:", QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty()) {
        playlistList->addItem(name.trimmed());
    }
}

void AdminMenuUI::onArtistSettingsClicked()
{
    ArtistSettingsUI *settingsWindow = new ArtistSettingsUI();
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose); // Se elimina sola al cerrar
    settingsWindow->show();
}

void AdminMenuUI::onProfilePicClicked()
{
    // Aquí podrías abrir una ventana de perfil, opciones, o lo que prefieras.
    // Por ejemplo: ProfileOptionsUI *options = new ProfileOptionsUI(); options->show();
    // QMessageBox::information(this, "Perfil", "Aquí irán opciones del perfil.");
}

