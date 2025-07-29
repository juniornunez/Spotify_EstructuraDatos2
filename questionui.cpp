#include "questionUI.h"
#include <QVBoxLayout>
#include <QFont>

QuestionUI::QuestionUI(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background-color: #191414;");

    titleLabel = new QLabel("¿Cuenta admin o user?");
    titleLabel->setStyleSheet("color: white;");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    adminButton = new QPushButton("Admin / Artista");
    adminButton->setStyleSheet(
        "QPushButton {"
        "background-color: #1DB954;"
        "color: black;"
        "font-size: 16pt;"
        "border: none;"
        "padding: 16px;"
        "border-radius: 18px;"
        "margin: 10px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #1ED760;"
        "}"
        );

    userButton = new QPushButton("Usuario Común");
    userButton->setStyleSheet(
        "QPushButton {"
        "background-color: #282828;"
        "color: white;"
        "font-size: 16pt;"
        "border: 1.5px solid #1DB954;"
        "padding: 16px;"
        "border-radius: 18px;"
        "margin: 10px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background-color: #444;"
        "color: #1DB954;"
        "}"
        );

    connect(adminButton, &QPushButton::clicked, this, &QuestionUI::adminSelected);
    connect(userButton, &QPushButton::clicked, this, &QuestionUI::userSelected);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addStretch();
    mainLayout->addWidget(titleLabel, 0, Qt::AlignCenter);
    mainLayout->addSpacing(40);
    mainLayout->addWidget(adminButton);
    mainLayout->addWidget(userButton);
    mainLayout->addSpacing(60);
    mainLayout->addStretch();

    setLayout(mainLayout);
    setFixedSize(420, 320);
    setWindowTitle("Selecciona tipo de cuenta");
}

QuestionUI::~QuestionUI() {}
