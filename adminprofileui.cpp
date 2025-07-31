#include "adminprofileui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QFont>
#include <QPushButton>
#include <QSpacerItem>
#include <QFileInfo>

AdminProfileUI::AdminProfileUI(
    const QString &profilePicPath,
    const QString &artistName,
    const QString &realName,
    QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background: #191414; color: white;");

    // ---------- FOTO DE PERFIL (GRANDE Y REDONDA) ----------
    QLabel *profilePicLabel = new QLabel;
    QPixmap pix(profilePicPath);
    if (!pix.isNull()) {
        QPixmap roundedPic(pix.scaled(120, 120, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
        QPixmap mask(120, 120);
        mask.fill(Qt::transparent);

        QPainter painter(&mask);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::white);
        painter.drawEllipse(0, 0, 120, 120);
        roundedPic.setMask(mask.createMaskFromColor(Qt::transparent, Qt::MaskInColor));
        profilePicLabel->setPixmap(roundedPic);
    } else {
        profilePicLabel->setText("👤");
        QFont f = profilePicLabel->font();
        f.setPointSize(54);
        profilePicLabel->setFont(f);
        profilePicLabel->setAlignment(Qt::AlignCenter);
    }
    profilePicLabel->setFixedSize(130, 130);
    profilePicLabel->setStyleSheet("background: #222; border-radius: 65px; margin: 0 auto;");

    // ---------- NOMBRES ----------
    QLabel *nameLabel = new QLabel(realName);
    QFont nameFont = nameLabel->font();
    nameFont.setPointSize(18);
    nameFont.setBold(true);
    nameLabel->setFont(nameFont);
    nameLabel->setAlignment(Qt::AlignCenter);

    QLabel *artistLabel = new QLabel(artistName);
    QFont artistFont = artistLabel->font();
    artistFont.setPointSize(14);
    artistFont.setItalic(true);
    artistLabel->setFont(artistFont);
    artistLabel->setStyleSheet("color: #1ED760; letter-spacing:1px;");
    artistLabel->setAlignment(Qt::AlignCenter);

    // ---------- BOTÓN LOG OUT ----------
    QPushButton *logoutButton = new QPushButton("Log out");
    logoutButton->setStyleSheet(
        "QPushButton {"
        " background-color: #1ED760; color: #191414; font-size: 14pt; border: none; padding: 12px 32px; border-radius: 22px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1fdf64; }"
        );
    logoutButton->setFixedWidth(180);

    connect(logoutButton, &QPushButton::clicked, this, &AdminProfileUI::logoutClicked);

    // ---------- LAYOUT PRINCIPAL ----------
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(36);
    mainLayout->addWidget(profilePicLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(18);
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(artistLabel);
    mainLayout->addSpacing(40);
    mainLayout->addStretch();
    mainLayout->addWidget(logoutButton, 0, Qt::AlignCenter);
    mainLayout->addSpacing(36);

    setLayout(mainLayout);
    setFixedSize(360, 440);
    setWindowTitle("Artist Profile");
}

AdminProfileUI::~AdminProfileUI() {}
