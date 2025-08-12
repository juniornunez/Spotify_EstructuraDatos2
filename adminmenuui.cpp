#include "adminmenuui.h"
#include <QInputDialog>
#include <QPixmap>
#include <QFont>
#include <QPainter>
#include <QPainterPath>
#include <QScrollArea>
#include <QDir>
#include <QFile>
#include <QDateTime>
#include "artistsettingsui.h"
#include "addsingleui.h"
#include "adminprofileui.h"
#include "loginUI.h"
#include "trendingui.h"

AdminMenuUI::AdminMenuUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent)
    : QWidget(parent), adminUsername(adminUsername), profilePicPath(profilePicPath)
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
    sidebarLayout->addStretch();

    // Botón Trending
    trendingButton = new QPushButton("Trending");
    trendingButton->setStyleSheet(
        "QPushButton { background-color: #222; color: #1ED760; font-size: 13pt; border-radius: 16px; padding: 8px 0; font-weight: bold; }"
        "QPushButton:hover { background-color: #282828; color: #fff; }"
        );
    sidebarLayout->addWidget(trendingButton);
    connect(trendingButton, &QPushButton::clicked, this, [this]() {
        TrendingUI *trendWin = new TrendingUI(this);
        trendWin->setAttribute(Qt::WA_DeleteOnClose);
        trendWin->show();
    });

    // Botón Artist Settings
    artistSettingsButton = new QPushButton("Artist Settings");
    artistSettingsButton->setStyleSheet(
        "QPushButton { background-color: #222; color: #1ED760; font-size: 13pt; border-radius: 16px; padding: 8px 0; font-weight: bold; }"
        "QPushButton:hover { background-color: #282828; color: #fff; }"
        );
    sidebarLayout->addWidget(artistSettingsButton);
    connect(artistSettingsButton, &QPushButton::clicked, this, &AdminMenuUI::onArtistSettingsClicked);

    // === PANEL PRINCIPAL ===
    QWidget *mainPanelWidget = new QWidget;
    mainPanelLayout = new QVBoxLayout(mainPanelWidget);
    mainPanelLayout->setContentsMargins(20, 20, 20, 20);
    mainPanelLayout->setSpacing(16);

    // Top bar
    topBarLayout = new QHBoxLayout;
    homeIconLabel = new QLabel;
    QPixmap homeIconPixmap("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/assets/homeicon.png");
    homeIconLabel->setPixmap(homeIconPixmap.scaled(36, 36, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    homeIconLabel->setFixedSize(44, 44);
    homeIconLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    topBarLayout->addWidget(homeIconLabel, 0, Qt::AlignLeft);

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

    // Botón perfil
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
    connect(profilePicButton, &QPushButton::clicked, this, &AdminMenuUI::onProfilePicClicked);
    mainPanelLayout->addLayout(topBarLayout);

    // My top songs
    QLabel *topSongsLabel = new QLabel("My top songs");
    QFont topSongsFont = topSongsLabel->font();
    topSongsFont.setPointSize(22);
    topSongsFont.setBold(true);
    topSongsLabel->setFont(topSongsFont);
    mainPanelLayout->addSpacing(35);
    mainPanelLayout->addWidget(topSongsLabel, 0, Qt::AlignLeft);

    QWidget *cardsWidget = new QWidget;
    cardsLayout = new QHBoxLayout(cardsWidget);
    QScrollArea *cardsScroll = new QScrollArea;
    cardsScroll->setWidget(cardsWidget);
    cardsScroll->setWidgetResizable(true);
    cardsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    cardsScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    cardsScroll->setFixedHeight(270);
    mainPanelLayout->addWidget(cardsScroll);

    // Cargar canciones usando hash
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
                SongData song;
                in >> song;
                f.close();

                songHash.insert(song.getId(), song);

                SongCardWidget *card = new SongCardWidget(song.getCoverPath(), song.getTitle(), song.getArtist(), song.getAudioPath());
                cardsLayout->addWidget(card);
                songCards.append(card);

                connect(card, &SongCardWidget::toggled, this, &AdminMenuUI::handleCardToggled);
                connect(card, &SongCardWidget::playPressed, this, &AdminMenuUI::handlePlayButtonPressed);
            }
        }
    }

    // Artistas
    mainPanelLayout->addSpacing(50);
    QLabel *topArtistsLabel = new QLabel("Watch out this artists");
    QFont topArtistsFont = topArtistsLabel->font();
    topArtistsFont.setPointSize(22);
    topArtistsFont.setBold(true);
    topArtistsLabel->setFont(topArtistsFont);
    mainPanelLayout->addWidget(topArtistsLabel, 0, Qt::AlignLeft);

    QWidget *artistCardsWidget = new QWidget;
    QHBoxLayout *artistCardsLayout = new QHBoxLayout(artistCardsWidget);
    QScrollArea *artistScroll = new QScrollArea;
    artistScroll->setWidget(artistCardsWidget);
    artistScroll->setWidgetResizable(true);
    artistScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    artistScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    artistScroll->setFixedHeight(300);
    mainPanelLayout->addWidget(artistScroll);

    QDir adminDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata");
    QStringList adminFolders = adminDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString &adminName : adminFolders) {
        QDir artistFolder(adminDir.absoluteFilePath(adminName));
        QStringList images = artistFolder.entryList(QStringList() << "*.png" << "*.jpg" << "*.jpeg", QDir::Files);
        QString artistPic = images.isEmpty() ? "" : artistFolder.absoluteFilePath(images.first());
        ArtistCardWidget *artistCard = new ArtistCardWidget(adminName, artistPic);
        artistCardsLayout->addWidget(artistCard);
        artistCards.append(artistCard);
        connect(artistCard, &ArtistCardWidget::doubleClicked, this, [=](const QString &name){
            AdminProfileUI *profileWin = new AdminProfileUI(artistPic, name);
            profileWin->setAttribute(Qt::WA_DeleteOnClose);
            profileWin->show();
        });
    }

    // PlayBar
    playBar = new PlayBarUI;
    playBar->setVisible(false);
    mainPanelLayout->addWidget(playBar);

    // Layout principal
    mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(sidebarWidget, 0);
    mainLayout->addWidget(mainPanelWidget, 1);
    setLayout(mainLayout);
    setMinimumSize(1050, 620);
    setWindowTitle("Musicfy - Admin Menu");
}

AdminMenuUI::~AdminMenuUI() {}

void AdminMenuUI::onAddPlaylistClicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Nueva Playlist", "Nombre de la playlist:", QLineEdit::Normal, "", &ok);
    if (ok && !name.trimmed().isEmpty())
        playlistList->addItem(name.trimmed());
}

void AdminMenuUI::onArtistSettingsClicked() {
    ArtistSettingsUI *settingsWindow = new ArtistSettingsUI(adminUsername, this);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();

    connect(settingsWindow, &ArtistSettingsUI::songUploaded, this, [=](const SongData& song){
        SongCardWidget *newCard = new SongCardWidget(song.getCoverPath(), song.getTitle(), song.getArtist(), song.getAudioPath());
        this->cardsLayout->addWidget(newCard);
        songCards.append(newCard);
        songHash.insert(song.getId(), song);

        connect(newCard, &SongCardWidget::toggled, this, &AdminMenuUI::handleCardToggled);
        connect(newCard, &SongCardWidget::playPressed, this, &AdminMenuUI::handlePlayButtonPressed);
    });

}


void AdminMenuUI::onProfilePicClicked() {
    QMenu *menu = new QMenu(this);
    QAction *profileAction = new QAction("Ver Perfil", this);
    QAction *signOutAction = new QAction("Sign Out", this);
    menu->addAction(profileAction);
    menu->addSeparator();
    menu->addAction(signOutAction);

    connect(profileAction, &QAction::triggered, this, [this]() {
        AdminProfileUI *profileWindow = new AdminProfileUI(profilePicPath, adminUsername);
        profileWindow->setAttribute(Qt::WA_DeleteOnClose);
        profileWindow->show();
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

void AdminMenuUI::handleCardToggled(SongCardWidget* card, bool nowSelected) {
    if (nowSelected) {
        for (SongCardWidget* c : songCards)
            if (c != card) c->setSelected(false);
        currentSelectedCard = card;
    } else {
        if (currentSelectedCard == card) currentSelectedCard = nullptr;
    }
}

void AdminMenuUI::handlePlayButtonPressed(SongCardWidget* card) {
    playBar->setSongInfo(card->getCover(), card->getTitle(), card->getArtist(), card->getAudioPath());
    playBar->setVisible(true);
    playBar->play();
}
