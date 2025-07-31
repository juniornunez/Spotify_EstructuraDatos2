#include "usermenuui.h"
#include <QInputDialog>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QSizePolicy>
#include <QPushButton>

UserMenuUI::UserMenuUI(const QString &profilePicPath, QWidget *parent)
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
    connect(addPlaylistButton, &QPushButton::clicked, this, &UserMenuUI::onAddPlaylistClicked);

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

    // Foto de perfil como BOTÓN CIRCULAR
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

    connect(profilePicButton, &QPushButton::clicked, this, &UserMenuUI::onProfilePicClicked);

    // Añadir barra superior al panel principal
    mainPanelLayout->addLayout(topBarLayout);

    // Relleno principal (aquí puedes agregar widgets, carátulas, etc.)
    QLabel *emptyHint = new QLabel("<i>No hay playlists aún</i>");
    emptyHint->setStyleSheet("color: #888; font-size: 13pt;");
    mainPanelLayout->addSpacing(48);
    mainPanelLayout->addWidget(emptyHint, 0, Qt::AlignCenter);
    mainPanelLayout->addStretch();

    // ---- LAYOUT PRINCIPAL ----
    mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(sidebarWidget, 0);
    mainLayout->addWidget(mainPanelWidget, 1);

    setLayout(mainLayout);
    setMinimumSize(1050, 620);
    setWindowTitle("Musicfy - Main Menu");
}

UserMenuUI::~UserMenuUI() {}

void UserMenuUI::onAddPlaylistClicked()
{
    bool ok;
    QString name = QInputDialog::getText(this, "Nueva Playlist", "Nombre de la playlist:", QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty()) {
        playlistList->addItem(name.trimmed());
    }
}

void UserMenuUI::onProfilePicClicked()
{
    // Aquí podrías abrir una ventana de perfil, opciones, etc.
    // QMessageBox::information(this, "Perfil", "Aquí irán opciones del usuario.");
}
