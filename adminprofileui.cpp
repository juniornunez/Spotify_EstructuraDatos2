#include "adminprofileui.h"
#include "HorizontalSongCard.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QDebug>
#include "songdata.h"
#include "admindata.h"

AdminProfileUI::AdminProfileUI(const QString &profilePicPath, const QString &adminUsername, QWidget *parent)
    : QWidget(parent),
    m_profilePicPath(profilePicPath),
    m_adminUsername(adminUsername)
{
    setStyleSheet("background-color: #191414; color: white;");

    loadAdminData(adminUsername);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(32, 28, 32, 16);
    mainLayout->setSpacing(18);

    setupTopBar();
    QHBoxLayout *topBarLayout = new QHBoxLayout;
    topBarLayout->setSpacing(18);
    topBarLayout->addWidget(profilePicLabel, 0, Qt::AlignVCenter);
    topBarLayout->addWidget(adminNameLabel, 0, Qt::AlignVCenter);
    topBarLayout->addSpacing(20);

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

    mainStack = new QStackedWidget(this);

    setupMusicAndAlbumsTab();
    setupArtistProfileTab();

    mainStack->addWidget(musicAndAlbumsWidget);
    mainStack->addWidget(artistProfileWidget);
    mainStack->setCurrentIndex(0);

    mainLayout->addWidget(mainStack, 1);
    setLayout(mainLayout);
}

void AdminProfileUI::setupTopBar()
{
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
    QVBoxLayout *outerLayout = new QVBoxLayout(musicAndAlbumsWidget);
    outerLayout->setSpacing(16);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    // === Título de canciones ===
    QLabel *songsTitle = new QLabel("Songs");
    QFont songTitleFont = songsTitle->font();
    songTitleFont.setPointSize(20);
    songTitleFont.setBold(true);
    songsTitle->setFont(songTitleFont);
    songsTitle->setStyleSheet("color: white; margin-top: 14px;");
    outerLayout->addWidget(songsTitle, 0, Qt::AlignLeft);

    // Contenedor de canciones
    QWidget *songsContainer = new QWidget;
    QVBoxLayout *songsLayout = new QVBoxLayout(songsContainer);
    songsLayout->setContentsMargins(0, 0, 0, 0);
    songsLayout->setSpacing(4);

    QString songsPath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/artistsongs")
                            .arg(m_adminUsername);
    QDir songsDir(songsPath);
    QStringList songFolders = songsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    int index = 1;
    for (const QString &folder : songFolders) {
        QDir songDir(songsDir.absoluteFilePath(folder));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);
        for (const QString &datFile : datFiles) {
            QFile f(songDir.absoluteFilePath(datFile));
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                in.setVersion(QDataStream::Qt_5_15);
                SongData song;
                in >> song;
                f.close();

                // Crear tarjeta horizontal (pos, cover, title, artist, duration)
                HorizontalSongCard *card = new HorizontalSongCard(
                    index,
                    song.getCoverPath(),
                    song.getTitle(),
                    song.getArtist()
                    );
                songsLayout->addWidget(card);
                index++;
            }
        }
    }

    songsLayout->addStretch();

    QScrollArea *songsScroll = new QScrollArea;
    songsScroll->setWidget(songsContainer);
    songsScroll->setWidgetResizable(true);
    songsScroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    outerLayout->addWidget(songsScroll);

    // === Título de Albums ===
    QLabel *albumsTitle = new QLabel("Albums");
    QFont albumTitleFont = albumsTitle->font();
    albumTitleFont.setPointSize(20);
    albumTitleFont.setBold(true);
    albumsTitle->setFont(albumTitleFont);
    albumsTitle->setStyleSheet("color: white; margin-top: 18px;");
    outerLayout->addWidget(albumsTitle, 0, Qt::AlignLeft);
}


void AdminProfileUI::setupArtistProfileTab()
{
    artistProfileWidget = new QWidget;
    QVBoxLayout *artistProfileLayout = new QVBoxLayout(artistProfileWidget);
    artistProfileLayout->setSpacing(22);
    artistProfileLayout->setContentsMargins(40, 20, 40, 20);

    // Foto de perfil grande
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

    // Título "About"
    QLabel *aboutTitle = new QLabel("About");
    QFont aboutFont = aboutTitle->font();
    aboutFont.setPointSize(18);
    aboutFont.setBold(true);
    aboutTitle->setFont(aboutFont);
    aboutTitle->setStyleSheet("color: white; margin-bottom: 7px;");
    artistProfileLayout->addWidget(aboutTitle, 0, Qt::AlignLeft);

    // Datos reales del admin
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


void AdminProfileUI::loadAdminData(const QString &adminUsername)
{
    QString filePath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/info.dat")
    .arg(adminUsername);

    QFile file(filePath);
    if (!file.exists()) {
        m_realName = "Nombre no establecido";
        m_stageName = adminUsername;
        m_country = "País no establecido";
        m_bio = "Bio no establecida";
        return;
    }

    if (file.open(QIODevice::ReadOnly)) {
        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_5_15);

        int id;
        QString username, password, artisticName, realName, country, genre, bio, fecha;
        bool activo;

        in >> id >> username >> password >> artisticName >> realName >> country >> genre >> bio >> fecha >> activo;

        m_realName = realName;
        m_stageName = artisticName;
        m_country = country;
        m_bio = bio;

        file.close();
    }
}


