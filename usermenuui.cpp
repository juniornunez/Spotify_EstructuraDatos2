#include "usermenuui.h"
#include "userprofileui.h"
#include "loginui.h"
#include "trendingui.h"
#include "songraterui.h"
#include "albumcardwidget.h"

#include <QInputDialog>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>

UserMenuUI::UserMenuUI(const QString &profilePicPath, const QString &username, QWidget *parent)
    : QWidget(parent), username(username), profilePicPath(profilePicPath), currentViewWidget(nullptr)
{
    setStyleSheet("background-color: #191414; color: white;");

    // === PANEL IZQUIERDO ===
    QWidget *sidebarWidget = new QWidget;
    sidebarWidget->setFixedWidth(230);
    sidebarWidget->setStyleSheet("background: #121212;");

    sidebarLayout = new QVBoxLayout(sidebarWidget);
    sidebarLayout->setContentsMargins(24, 18, 18, 24);
    sidebarLayout->setSpacing(16);

    // My Library
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

    loadPlaylists();

    connect(playlistList, &QListWidget::itemDoubleClicked, this, [=](QListWidgetItem *item){
        showPlaylistUI(item->text());
    });

    // Botón Trending
    trendingButton = new QPushButton("Trending");
    trendingButton->setStyleSheet(
        "QPushButton { background-color: #222; color: #1ED760; font-size: 13pt; border-radius: 16px; padding: 8px 0; font-weight: bold; }"
        "QPushButton:hover { background-color: #282828; color: #fff; }"
        );
    sidebarLayout->addWidget(trendingButton);
    connect(trendingButton, &QPushButton::clicked, this, [this]() {
        if (currentViewWidget != nullptr) {
            currentViewWidget->setParent(nullptr);
        }
        TrendingUI *trendingUI = new TrendingUI(this->username, this);
        mainPanelLayout->addWidget(trendingUI);
        currentViewWidget = trendingUI;
    });


    // Botón Rate Songs
    rateSongsButton = new QPushButton("Rate Songs");
    rateSongsButton->setStyleSheet(
        "QPushButton { background-color: #222; color: #1ED760; font-size: 13pt; border-radius: 16px; padding: 8px 0; font-weight: bold; }"
        "QPushButton:hover { background-color: #282828; color: #fff; }"
        );
    sidebarLayout->addWidget(rateSongsButton);

    connect(rateSongsButton, &QPushButton::clicked, this, [=]() {
        if (currentViewWidget != nullptr) {
            currentViewWidget->setParent(nullptr);
        }
        SongRaterUI *rateUI = new SongRaterUI(username, this);
        mainPanelLayout->addWidget(rateUI);
        currentViewWidget = rateUI;
    });

    // === PANEL DERECHO ===
    QWidget *mainPanelWidget = new QWidget;

    QVBoxLayout *panelWithPlaybar = new QVBoxLayout(mainPanelWidget);
    panelWithPlaybar->setContentsMargins(20, 20, 20, 20);
    panelWithPlaybar->setSpacing(0);

    mainPanelLayout = new QVBoxLayout;
    mainPanelLayout->setContentsMargins(0, 0, 0, 0);
    mainPanelLayout->setSpacing(16);

    // Top bar
    topBarLayout = new QHBoxLayout;

    homeButton = new QPushButton;
    homeButton->setIcon(QIcon("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/homeicon.png"));
    homeButton->setIconSize(QSize(36, 36));
    homeButton->setFixedSize(44, 44);
    homeButton->setStyleSheet("QPushButton { background: transparent; border: none; }");
    topBarLayout->addWidget(homeButton, 0, Qt::AlignLeft);

    connect(homeButton, &QPushButton::clicked, this, &UserMenuUI::restoreMainView);

    searchBar = new QLineEdit;
    searchBar->setPlaceholderText("What do you want to play?");
    searchBar->setStyleSheet(
        "QLineEdit { background-color: #222; color: white; border: none; padding: 10px 18px; border-radius: 18px; font-size: 15pt; min-width:300px; }"
        );
    topBarLayout->addWidget(searchBar, 1);

    connect(searchBar, &QLineEdit::textChanged, this, [=](const QString &text){
        QString query = text.trimmed().toLower();
        for (auto *card : songCards)
            card->setVisible(card->getTitle().toLower().contains(query) || card->getArtist().toLower().contains(query) || query.isEmpty());
        for (auto *card : artistCards)
            card->setVisible(card->getArtistName().toLower().contains(query) || query.isEmpty());
    });

    profilePicButton = new QPushButton;
    profilePicButton->setFixedSize(48, 48);
    profilePicButton->setCursor(Qt::PointingHandCursor);
    profilePicButton->setStyleSheet(
        "QPushButton { border-radius: 24px; background: #282828; border: none; padding: 0; }"
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
        profilePicButton->setIcon(QIcon(roundedPic));
        profilePicButton->setIconSize(QSize(48, 48));
    }
    topBarLayout->addWidget(profilePicButton, 0, Qt::AlignRight);
    connect(profilePicButton, &QPushButton::clicked, this, &UserMenuUI::onProfilePicClicked);

    mainPanelLayout->addLayout(topBarLayout);

    // Contenido original
    originalContentWidget = new QWidget;
    QVBoxLayout *originalLayout = new QVBoxLayout(originalContentWidget);
    originalLayout->setContentsMargins(0, 0, 0, 0);

    // My top songs
    QLabel *topSongsLabel = new QLabel("My top songs");
    QFont topSongsFont = topSongsLabel->font();
    topSongsFont.setPointSize(22);
    topSongsFont.setBold(true);
    topSongsLabel->setFont(topSongsFont);
    originalLayout->addSpacing(35);
    originalLayout->addWidget(topSongsLabel, 0, Qt::AlignLeft);

    QWidget *cardsWidget = new QWidget;
    cardsLayout = new QHBoxLayout(cardsWidget);
    QScrollArea *cardsScroll = new QScrollArea;
    cardsScroll->setWidget(cardsWidget);
    cardsScroll->setWidgetResizable(true);
    cardsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    cardsScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardsScroll->setFixedHeight(270);
    originalLayout->addWidget(cardsScroll);

    // Cargar canciones
    QDir singlesDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
    QStringList subdirs = singlesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &songFolder : subdirs) {
        QDir songDir(singlesDir.absoluteFilePath(songFolder));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);
        for (const QString &datFile : datFiles) {
            QString datosPath = songDir.absoluteFilePath(datFile);
            QFile f(datosPath);
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                in.setVersion(QDataStream::Qt_5_15);
                SongData song;
                in >> song;
                f.close();

                song.setFilePath(datosPath);
                songHash.insert(song.getId(), song);

                SongCardWidget *card = new SongCardWidget(song.getCoverPath(), song.getTitle(), song.getArtist(), song.getAudioPath());
                cardsLayout->addWidget(card);
                songCards.append(card);

                connect(card, &SongCardWidget::toggled, this, &UserMenuUI::handleCardToggled);
                connect(card, &SongCardWidget::playPressed, this, &UserMenuUI::handlePlayButtonPressed);
            }
        }
    }

    // Artistas
    originalLayout->addSpacing(50);
    QLabel *topArtistsLabel = new QLabel("Watch out this artists");
    QFont topArtistsFont = topArtistsLabel->font();
    topArtistsFont.setPointSize(22);
    topArtistsFont.setBold(true);
    topArtistsLabel->setFont(topArtistsFont);
    originalLayout->addWidget(topArtistsLabel, 0, Qt::AlignLeft);

    QWidget *artistCardsWidget = new QWidget;
    QHBoxLayout *artistCardsLayout = new QHBoxLayout(artistCardsWidget);
    QScrollArea *artistScroll = new QScrollArea;
    artistScroll->setWidget(artistCardsWidget);
    artistScroll->setWidgetResizable(true);
    artistScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    artistScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    artistScroll->setFixedHeight(300);
    originalLayout->addWidget(artistScroll);

    QDir adminDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata");
    QStringList adminFolders = adminDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &adminName : adminFolders) {
        QDir artistFolder(adminDir.absoluteFilePath(adminName));
        QStringList images = artistFolder.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg", QDir::Files);
        QString artistPic = images.isEmpty() ? "" : artistFolder.absoluteFilePath(images.first());
        ArtistCardWidget *artistCard = new ArtistCardWidget(adminName, artistPic);
        artistCardsLayout->addWidget(artistCard);
        artistCards.append(artistCard);

        connect(artistCard, &ArtistCardWidget::doubleClicked, this, [=](const QString &artistUsername){
            if (currentViewWidget != nullptr) {
                currentViewWidget->setParent(nullptr);
            }

            QString artistPicPath;
            QDir artistFolder(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1").arg(artistUsername));
            QStringList images = artistFolder.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg", QDir::Files);
            if (!images.isEmpty()) {
                artistPicPath = artistFolder.absoluteFilePath(images.first());
            }

            UserProfileUI *profilePage = new UserProfileUI(artistUsername, this);


        });
    }

    // PlayBar
    playBar = new PlayBarUI(username, this);
    playBar->setVisible(false);

    mainPanelLayout->addWidget(originalContentWidget);
    currentViewWidget = originalContentWidget;

    panelWithPlaybar->addLayout(mainPanelLayout, 1);
    panelWithPlaybar->addWidget(playBar, 0, Qt::AlignBottom);

    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sidebarWidget, 0);
    mainLayout->addWidget(mainPanelWidget, 1);
    setLayout(mainLayout);
    setMinimumSize(1050, 620);
    setWindowTitle("Musicfy - Main Menu");
}

// ================= FUNCIONES =================

void UserMenuUI::onAddPlaylistClicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Nueva Playlist",
                                         "Nombre de la playlist:",
                                         QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty()) {
        QString playlistName = name.trimmed();

        QString coverPath = QFileDialog::getOpenFileName(this,
                                                         "Seleccionar portada de la playlist",
                                                         "",
                                                         "Imágenes (*.png *.jpg *.jpeg)");
        if (coverPath.isEmpty()) {
            QMessageBox::warning(this, "Sin portada", "Debes seleccionar una imagen de portada.");
            return;
        }

        QDir userPlaylistsDir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/playlists_%1").arg(username));
        if (!userPlaylistsDir.exists()) {
            QDir().mkpath(userPlaylistsDir.absolutePath());
        }

        QString playlistDirPath = userPlaylistsDir.filePath(playlistName);
        QDir().mkpath(playlistDirPath);

        QString destCoverPath = playlistDirPath + "/cover.jpg";
        QFile::copy(coverPath, destCoverPath);

        playlistList->addItem(playlistName);
    }
}

void UserMenuUI::onProfilePicClicked() {
    QMenu *menu = new QMenu(this);
    QAction *profileAction = new QAction("Ver Perfil", this);
    QAction *signOutAction = new QAction("Sign Out", this);
    menu->addAction(profileAction);
    menu->addSeparator();
    menu->addAction(signOutAction);

    connect(profileAction, &QAction::triggered, this, [this]() {
        showUserProfileUI();
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

void UserMenuUI::handleCardToggled(SongCardWidget* card, bool nowSelected) {
    if (nowSelected) {
        for (SongCardWidget* c : songCards)
            if (c != card) c->setSelected(false);
        currentSelectedCard = card;
    } else {
        if (currentSelectedCard == card) currentSelectedCard = nullptr;
    }
}

void UserMenuUI::handlePlayButtonPressed(SongCardWidget* card) {
    playBar->setSongInfo(card->getCover(), card->getTitle(), card->getArtist(), card->getAudioPath());
    playBar->setVisible(true);
    playBar->play();
}

void UserMenuUI::restoreMainView() {
    if (currentViewWidget != nullptr && currentViewWidget != originalContentWidget) {
        currentViewWidget->setParent(nullptr);
    }
    mainPanelLayout->addWidget(originalContentWidget);
    currentViewWidget = originalContentWidget;
}

void UserMenuUI::showPlaylistUI(const QString &playlistName) {
    if (currentViewWidget != nullptr) {
        currentViewWidget->setParent(nullptr);
    }
    PlaylistDisplayUI *playlistUI = new PlaylistDisplayUI(playlistName, username, this);
    mainPanelLayout->addWidget(playlistUI);
    currentViewWidget = playlistUI;

    connect(playBar, &PlayBarUI::requestNextSong, playlistUI, &PlaylistDisplayUI::playNextSong);
    connect(playlistUI, &PlaylistDisplayUI::songSelected, this, [this](const QString &cover,
                                                                       const QString &title,
                                                                       const QString &artist,
                                                                       const QString &audioPath){
        playBar->setSongInfo(cover, title, artist, audioPath);
        playBar->setVisible(true);
        playBar->play();
    });
}

void UserMenuUI::showUserProfileUI() {
    if (currentViewWidget != nullptr) {
        currentViewWidget->setParent(nullptr);
    }

    // Usa tu UserProfileUI simplificado (solo username y parent)
    UserProfileUI *profilePage = new UserProfileUI(username, this);
    mainPanelLayout->addWidget(profilePage);
    currentViewWidget = profilePage;
}


void UserMenuUI::showAlbumUI(const QString &albumName,
                             const QString &coverPath,
                             const QString &artistName) {
    qDebug() << "➡️ showAlbumUI llamado con:" << albumName << coverPath << artistName;

    if (currentViewWidget != nullptr) {
        currentViewWidget->setParent(nullptr);
    }

    PlaylistDisplayUI *albumView = new PlaylistDisplayUI(albumName, coverPath, artistName, username, this);
    mainPanelLayout->addWidget(albumView);
    currentViewWidget = albumView;

    connect(playBar, &PlayBarUI::requestNextSong, albumView, &PlaylistDisplayUI::playNextSong);
    connect(albumView, &PlaylistDisplayUI::songSelected, this, [this](const QString &cover,
                                                                      const QString &title,
                                                                      const QString &artist,
                                                                      const QString &audioPath){
        playBar->setSongInfo(cover, title, artist, audioPath);
        playBar->setVisible(true);
        playBar->play();
    });
}

void UserMenuUI::loadPlaylists() {
    QDir userPlaylistsDir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/playlists_%1").arg(username));
    if (userPlaylistsDir.exists()) {
        QStringList playlists = userPlaylistsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (const QString &playlist : playlists) {
            playlistList->addItem(playlist);
        }
    }
}

