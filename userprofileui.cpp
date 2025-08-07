#include "userprofileui.h"
#include <QVBoxLayout>
#include <QFile>
#include <QDir>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QFont>
#include <QSpacerItem>

UserProfileUI::UserProfileUI(const QString &username, QWidget *parent)
    : QWidget(parent), m_username(username)
{
    setStyleSheet("background-color: #191414; color: white;");

    // TAMAÑO FIJO MÁS GRANDE
    setFixedSize(400, 420);

    loadUserData(username);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(0, 40, 0, 40);

    // --- Foto de perfil circular (MÁS GRANDE) ---
    profilePicLabel = new QLabel;
    profilePicLabel->setFixedSize(180, 180);

    QPixmap pix(m_profilePicPath);
    if (!pix.isNull()) {
        QPixmap roundedPic(180, 180);
        roundedPic.fill(Qt::transparent);
        QPainter painter(&roundedPic);
        painter.setRenderHint(QPainter::Antialiasing);
        QPainterPath path;
        path.addEllipse(0, 0, 180, 180);
        painter.setClipPath(path);
        painter.drawPixmap(0, 0, pix.scaled(180, 180, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        painter.end();
        profilePicLabel->setPixmap(roundedPic);
    } else {
        profilePicLabel->setStyleSheet("background: #232323; border-radius: 90px;");
    }

    // --- Username ---
    usernameLabel = new QLabel(m_username);
    QFont userFont = usernameLabel->font();
    userFont.setPointSize(25);
    userFont.setBold(true);
    usernameLabel->setFont(userFont);
    usernameLabel->setAlignment(Qt::AlignCenter);
    usernameLabel->setStyleSheet("color: #1ED760;");

    // --- Edad ---
    ageLabel = new QLabel(m_age >= 0 ? QString("Edad: %1 años").arg(m_age) : QString("Edad: --"));
    QFont ageFont = ageLabel->font();
    ageFont.setPointSize(18);
    ageLabel->setFont(ageFont);
    ageLabel->setAlignment(Qt::AlignCenter);
    ageLabel->setStyleSheet("color: #bbb;");

    mainLayout->addSpacing(12);
    mainLayout->addWidget(profilePicLabel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(25);
    mainLayout->addWidget(usernameLabel);
    mainLayout->addSpacing(14);
    mainLayout->addWidget(ageLabel);

    setLayout(mainLayout);
}

UserProfileUI::~UserProfileUI() {}

void UserProfileUI::loadUserData(const QString &username)
{
    // Foto de perfil
    m_profilePicPath = findProfilePic(username);

    // Edad (lee de archivo edad.txt)
    QString agePath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/userdata/%1/edad.txt").arg(username);
    QFile ageFile(agePath);
    if (ageFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QString ageStr = QString::fromUtf8(ageFile.readAll()).trimmed();
        m_age = ageStr.toInt();
        ageFile.close();
    } else {
        m_age = -1;
    }
}

QString UserProfileUI::findProfilePic(const QString &username)
{
    QDir userDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/userdata/" + username);
    QStringList filters = {"profile.jpg", "profile.jpeg", "profile.png", "profile.bmp"};
    QStringList matches = userDir.entryList(filters, QDir::Files);
    if (!matches.isEmpty())
        return userDir.absoluteFilePath(matches.first());
    return "";
}
