#include "adminprofileui.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QDebug>

AdminProfileUI::AdminProfileUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent)
    : QWidget(parent),
    m_profilePicPath(profilePicPath),
    m_adminUsername(adminUsername)
{
    setStyleSheet("background-color: #191414; color: white;");

    // --- Cargar datos del admin ---
    loadAdminData(adminUsername);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 28, 32, 16);
    mainLayout->setSpacing(18);

    // Top Bar
    setupTopBar();
    QHBoxLayout *topBarLayout = new QHBoxLayout;
    topBarLayout->setSpacing(18);
    topBarLayout->addWidget(profilePicLabel, 0, Qt::AlignVCenter);
    topBarLayout->addWidget(adminNameLabel, 0, Qt::AlignVCenter);
    topBarLayout->addSpacing(20);

    // Tabs
    tabMusicButton = new QPushButton("Music & Albums");
    tabProfileButton = new QPushButton("Artist Profile");
    tabMusicButton->setCheckable(true);
    tabProfileButton->setCheckable(true);
    tabMusicButton->setChecked(true);

    tabMusicButton->setStyleSheet(
        "QPushButton { background: transparent; color: #bbb; font-size: 17px; font-weight: bold; padding: 7px 22px; border: none; border-bottom: 2.5px solid transparent; }"
        "QPushButton:checked { color: #1ED760; border-bottom: 2.5px solid #1ED760; }"
        "QPushButton:hover { color: #1ED760; }"
        );
    tabProfileButton->setStyleSheet(tabMusicButton->styleSheet());

    connect(tabMusicButton, &QPushButton::clicked, this, &AdminProfileUI::onTabMusicClicked);
    connect(tabProfileButton, &QPushButton::clicked, this, &AdminProfileUI::onTabProfileClicked);

    topBarLayout->addWidget(tabMusicButton);
    topBarLayout->addWidget(tabProfileButton);
    topBarLayout->addStretch();

    mainLayout->addLayout(topBarLayout);

    // Main stack (for tab content)
    mainStack = new QStackedWidget(this);

    setupMusicAndAlbumsTab();
    setupArtistProfileTab();

    mainStack->addWidget(musicAndAlbumsWidget);
    mainStack->addWidget(artistProfileWidget);

    mainStack->setCurrentIndex(0); // Default tab
    mainLayout->addWidget(mainStack, 1);

    setLayout(mainLayout);
}

void AdminProfileUI::setupTopBar()
{
    // --- Profile Picture Circular ---
    profilePicLabel = new QLabel;
    profilePicLabel->setFixedSize(64, 64);

    QPixmap pix(m_profilePicPath);
    if (!pix.isNull()) {
        QPixmap roundedPic(64, 64);
        roundedPic.fill(Qt::transparent);
        QPainter painter(&roundedPic);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 64, 64);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pix.scaled(64, 64, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        painter.end();
        profilePicLabel->setPixmap(roundedPic);
    } else {
        profilePicLabel->setStyleSheet("background: #222; border-radius: 32px;");
    }

    adminNameLabel = new QLabel(m_stageName.isEmpty() ? m_adminUsername : m_stageName);
    QFont nameFont = adminNameLabel->font();
    nameFont.setPointSize(22);
    nameFont.setBold(true);
    adminNameLabel->setFont(nameFont);
    adminNameLabel->setStyleSheet("color: white;");
}

void AdminProfileUI::setupMusicAndAlbumsTab()
{
    musicAndAlbumsWidget = new QWidget;
    musicAndAlbumsLayout = new QVBoxLayout(musicAndAlbumsWidget);
    musicAndAlbumsLayout->setSpacing(16);
    musicAndAlbumsLayout->setContentsMargins(0, 0, 0, 0);

    // Songs
    QLabel *songsTitle = new QLabel("Songs");
    QFont songTitleFont = songsTitle->font();
    songTitleFont.setPointSize(20);
    songTitleFont.setBold(true);
    songsTitle->setFont(songTitleFont);
    songsTitle->setStyleSheet("color: white; margin-top: 14px;");
    musicAndAlbumsLayout->addWidget(songsTitle, 0, Qt::AlignLeft);

    // Example songs list (replace with SongCardWidget if needed)
    songsList = new QListWidget;
    songsList->setStyleSheet(
        "QListWidget { background: #191414; color: white; border: none; font-size: 15px; }"
        "QListWidget::item { border-bottom: 1px solid #282828; min-height: 46px; }"
        "QListWidget::item:selected { background: #282828; color: #1ED760; }"
        );
    // TODO: Aquí podrías cargar las canciones reales del admin si lo deseas
    for (int i = 0; i < 5; ++i)
        songsList->addItem(QString("Es ejemplo Ingee %1").arg(i+1));
    musicAndAlbumsLayout->addWidget(songsList);

    // Albums
    QLabel *albumsTitle = new QLabel("Albums");
    QFont albumTitleFont = albumsTitle->font();
    albumTitleFont.setPointSize(20);
    albumTitleFont.setBold(true);
    albumsTitle->setFont(albumTitleFont);
    albumsTitle->setStyleSheet("color: white; margin-top: 18px;");
    musicAndAlbumsLayout->addWidget(albumsTitle, 0, Qt::AlignLeft);

    // Horizontal scroll for albums (replace with your custom cards later)
    QWidget *albumsContainer = new QWidget;
    QHBoxLayout *albumsLayout = new QHBoxLayout(albumsContainer);
    albumsLayout->setSpacing(16);
    albumsLayout->setContentsMargins(0, 0, 0, 0);

    for (int i = 0; i < 6; ++i) {
        QLabel *albumCover = new QLabel;
        albumCover->setFixedSize(110, 110);
        albumCover->setStyleSheet("background: #232323; border-radius: 10px;");
        albumCover->setPixmap(QPixmap(":/assets/default_album.png").scaled(110, 110, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        albumsLayout->addWidget(albumCover);
    }

    QScrollArea *albumsScroll = new QScrollArea;
    albumsScroll->setWidget(albumsContainer);
    albumsScroll->setWidgetResizable(true);
    albumsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    albumsScroll->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    albumsScroll->setFixedHeight(140);
    musicAndAlbumsLayout->addWidget(albumsScroll);
}

void AdminProfileUI::setupArtistProfileTab()
{
    artistProfileWidget = new QWidget;
    artistProfileLayout = new QVBoxLayout(artistProfileWidget);
    artistProfileLayout->setSpacing(22);
    artistProfileLayout->setContentsMargins(40, 20, 40, 20);

    // Profile image big
    profilePicBig = new QLabel;
    profilePicBig->setFixedSize(130, 130);
    profilePicBig->setStyleSheet("border-radius: 65px; background: #232323;");
    QPixmap pix(m_profilePicPath);
    if (!pix.isNull()) {
        QPixmap roundedPic(130, 130);
        roundedPic.fill(Qt::transparent);
        QPainter painter(&roundedPic);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 130, 130);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pix.scaled(130, 130, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        painter.end();
        profilePicBig->setPixmap(roundedPic);
    }
    artistProfileLayout->addWidget(profilePicBig, 0, Qt::AlignHCenter);

    // About section
    QLabel *aboutTitle = new QLabel("About");
    QFont aboutFont = aboutTitle->font();
    aboutFont.setPointSize(18);
    aboutFont.setBold(true);
    aboutTitle->setFont(aboutFont);
    aboutTitle->setStyleSheet("color: white; margin-bottom: 7px;");
    artistProfileLayout->addWidget(aboutTitle, 0, Qt::AlignLeft);

    // Datos del admin/artista
    realNameValue = new QLabel("Real Name: <b>" + m_realName + "</b>");
    stageNameValue = new QLabel("Stage Name: <b>" + (m_stageName.isEmpty() ? m_adminUsername : m_stageName) + "</b>");
    countryValue   = new QLabel("Country: <b>" + m_country + "</b>");
    bioValue       = new QLabel("Bio:<br>" + m_bio);

    for (QLabel* l : {realNameValue, stageNameValue, countryValue, bioValue}) {
        l->setStyleSheet("color: #bbb; font-size: 15px; margin-bottom: 3px;");
        artistProfileLayout->addWidget(l, 0, Qt::AlignLeft);
    }
}

void AdminProfileUI::onTabMusicClicked()
{
    tabMusicButton->setChecked(true);
    tabProfileButton->setChecked(false);
    mainStack->setCurrentIndex(0);
}

void AdminProfileUI::onTabProfileClicked()
{
    tabMusicButton->setChecked(false);
    tabProfileButton->setChecked(true);
    mainStack->setCurrentIndex(1);
}

// ----------- Cargar datos del admin desde archivo -----------
void AdminProfileUI::loadAdminData(const QString &adminUsername)
{
    QString filePath = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/" + adminUsername + "/perfil.dat";
    QFile file(filePath);
    if (!file.exists()) {
        qWarning() << "No se encontró perfil para admin" << adminUsername;
        m_realName = "Nombre no encontrado";
        m_stageName = adminUsername;
        m_country = "Desconocido";
        m_bio = "";
        return;
    }

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);
        QString realName, stageName, country, bio;
        in >> realName >> stageName >> country >> bio;
        m_realName = realName;
        m_stageName = stageName;
        m_country = country;
        m_bio = bio;
        file.close();
    }
}
