#include "usermenuui.h"
#include "userprofileui.h"
#include "loginui.h"
#include <QInputDialog>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QScrollArea>
#include <QMenu>
#include "trendingui.h"
UserMenuUI::UserMenuUI(const QString &profilePicPath, const QString &username, QWidget *parent)
    : QWidget(parent),
    m_username(username),
    m_profilePicPath(profilePicPath)
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

    sidebarLayout->addStretch();

    // -------- PANEL DERECHO (main panel) --------
    QWidget *mainPanelWidget = new QWidget;
    mainPanelWidget->setStyleSheet("background: transparent;");
    mainPanelLayout = new QVBoxLayout(mainPanelWidget);
    mainPanelLayout->setContentsMargins(20, 20, 20, 20);
    mainPanelLayout->setSpacing(16);

    // ----- Top bar -----
    topBarLayout = new QHBoxLayout;
    topBarLayout->setSpacing(16);

    // Home
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


    // Botón "Trending"
    trendingButton = new QPushButton("Trending");
    trendingButton->setStyleSheet(
        "QPushButton { background-color: #222; color: #1ED760; font-size: 13pt; border-radius: 16px; padding: 8px 0; font-weight: bold; }"
        "QPushButton:hover { background-color: #282828; color: #fff; }"
        );
    sidebarLayout->addWidget(trendingButton);

    // Conexión para abrir el TrendingUI
    connect(trendingButton, &QPushButton::clicked, this, [this]() {
        TrendingUI *trendWin = new TrendingUI(this);
        trendWin->setAttribute(Qt::WA_DeleteOnClose);
        trendWin->show();
    });


    topBarLayout->addWidget(profilePicButton, 0, Qt::AlignRight);

    connect(profilePicButton, &QPushButton::clicked, this, &UserMenuUI::onProfilePicClicked);

    // Añadir barra superior al panel principal
    mainPanelLayout->addLayout(topBarLayout);

    // "My top songs"
    QLabel *topSongsLabel = new QLabel("My top songs");
    QFont topSongsFont = topSongsLabel->font();
    topSongsFont.setPointSize(22);
    topSongsFont.setBold(true);
    topSongsLabel->setFont(topSongsFont);
    topSongsLabel->setStyleSheet("color: white;");
    mainPanelLayout->addSpacing(35);
    mainPanelLayout->addWidget(topSongsLabel, 0, Qt::AlignLeft);

    // --- Cards de canciones con scroll ---
    QWidget *cardsWidget = new QWidget;
    QHBoxLayout *cardsLayout = new QHBoxLayout(cardsWidget);
    cardsLayout->setContentsMargins(0, 0, 0, 0);
    cardsLayout->setSpacing(16);
    cardsWidget->setStyleSheet("background: transparent;");
    QScrollArea *cardsScroll = new QScrollArea;
    cardsScroll->setWidget(cardsWidget);
    cardsScroll->setWidgetResizable(true);
    cardsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    cardsScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardsScroll->setFixedHeight(270);
    mainPanelLayout->addWidget(cardsScroll);

    this->cardsLayout = cardsLayout;

    // --- CARGAR TODAS LAS CANCIONES DEL FOLDER GLOBALSONGS (USANDO .DAT) ---
    QDir singlesDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
    QStringList subdirs = singlesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &songFolder : subdirs) {
        QDir songDir(singlesDir.absoluteFilePath(songFolder));
        QString datosPath = songDir.absoluteFilePath("datos" + songFolder + ".dat");
        if (QFile::exists(datosPath)) {
            QFile f(datosPath);
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                in.setVersion(QDataStream::Qt_5_15);

                QString title, genre, duration, desc, coverPath, audioPath, artist;
                QDateTime created;
                in >> title >> genre >> duration >> desc >> coverPath >> audioPath >> artist >> created;
                f.close();

                SongCardWidget *card = new SongCardWidget(coverPath, title, artist, audioPath);
                cardsLayout->addWidget(card);
                songCards.append(card);

                connect(card, &SongCardWidget::toggled, this, &UserMenuUI::handleCardToggled);
                // CONECTA PARA REPRODUCCIÓN
                connect(card, &SongCardWidget::playPressed, this, [=]() {
                    this->handlePlayButtonPressed(audioPath, coverPath, title, artist);
                });
            }
        }
    }

    // "Top artists"
    QLabel *topArtistsLabel = new QLabel("Watch out this artists");
    QFont topArtistsFont = topArtistsLabel->font();
    topArtistsFont.setPointSize(22);
    topArtistsFont.setBold(true);
    topArtistsLabel->setFont(topArtistsFont);
    topArtistsLabel->setStyleSheet("color: white; margin-top:20px;");
    mainPanelLayout->addSpacing(170);
    mainPanelLayout->addWidget(topArtistsLabel, 0, Qt::AlignLeft);

    mainPanelLayout->addStretch();

    // ---- BARRA DE REPRODUCCIÓN ----
    playBar = new PlayBarUI;
    playBar->setVisible(false);
    mainPanelLayout->addWidget(playBar);

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
    QMenu *menu = new QMenu(this);

    QAction *profileAction = new QAction("Ver Perfil", this);
    QAction *signOutAction = new QAction("Sign Out", this);

    menu->addAction(profileAction);
    menu->addSeparator();
    menu->addAction(signOutAction);

    connect(profileAction, &QAction::triggered, this, [this]() {
        UserProfileUI *profileWindow = new UserProfileUI(m_username);
        profileWindow->setAttribute(Qt::WA_DeleteOnClose);
        profileWindow->setWindowModality(Qt::ApplicationModal);
        profileWindow->show();
        profileWindow->raise();
        profileWindow->activateWindow();
    });

    connect(signOutAction, &QAction::triggered, this, [this]() {
        QWidget *login = new LoginUI();
        login->setAttribute(Qt::WA_DeleteOnClose);
        login->show();
        this->window()->close();
    });

    QPoint pos = profilePicButton->mapToGlobal(QPoint(0, profilePicButton->height()));
    menu->exec(pos);
}

void UserMenuUI::handleCardToggled(SongCardWidget* card, bool nowSelected)
{
    if (nowSelected) {
        for (SongCardWidget* other : songCards) {
            if (other != card) other->setSelected(false);
        }
    }
}

// --- ESTA ES LA FUNCIÓN CLAVE PARA LA REPRODUCCIÓN DE AUDIO ---
void UserMenuUI::handlePlayButtonPressed(const QString &audioPath, const QString &cover, const QString &title, const QString &artist)
{
    playBar->setSongInfo(cover, title, artist, audioPath);
    playBar->setVisible(true);
    playBar->play();
}
