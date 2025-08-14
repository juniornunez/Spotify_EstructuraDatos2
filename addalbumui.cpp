#include "addalbumui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QDateTime>
#include <QUuid>
#include <QFileInfo>
#include <QIntValidator>
#include "adminmenuui.h"

AddAlbumUI::AddAlbumUI(const QString& adminUsername, QWidget *parent)
    : QWidget(parent), adminUsername(adminUsername), songCount(0)
{
    setStyleSheet("background-color: #191414; color: white;");

    // Paso inicial: datos del álbum
    QLabel *albumNameLabel = new QLabel("Nombre del álbum:");
    albumNameEdit = new QLineEdit;
    albumNameEdit->setStyleSheet("background: #121212; color: white; border-radius: 5px; padding: 6px;");

    QLabel *songCountLabel = new QLabel("Cantidad de canciones:");
    songCountEdit = new QLineEdit;
    songCountEdit->setValidator(new QIntValidator(1, 50, this));
    songCountEdit->setStyleSheet("background: #121212; color: white; border-radius: 5px; padding: 6px;");

    QLabel *albumCoverLabel = new QLabel("Carátula del álbum:");
    albumCoverPathEdit = new QLineEdit;
    albumCoverPathEdit->setReadOnly(true);
    QPushButton *selectAlbumCoverButton = new QPushButton("Seleccionar imagen");
    connect(selectAlbumCoverButton, &QPushButton::clicked, this, &AddAlbumUI::onSelectAlbumCoverClicked);

    QHBoxLayout *coverLayout = new QHBoxLayout;
    coverLayout->addWidget(albumCoverPathEdit);
    coverLayout->addWidget(selectAlbumCoverButton);

    continueButton = new QPushButton("Continuar");
    continueButton->setStyleSheet("background-color: #1ED760; color: black; font-weight: bold; border-radius: 20px; padding: 8px 16px;");
    connect(continueButton, &QPushButton::clicked, this, &AddAlbumUI::onContinueClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(albumNameLabel);
    mainLayout->addWidget(albumNameEdit);
    mainLayout->addWidget(songCountLabel);
    mainLayout->addWidget(songCountEdit);
    mainLayout->addWidget(albumCoverLabel);
    mainLayout->addLayout(coverLayout);
    mainLayout->addWidget(continueButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
    setFixedSize(450, 350);
    setWindowTitle("Crear Álbum");
}

void AddAlbumUI::onSelectAlbumCoverClicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Seleccionar imagen del álbum", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!file.isEmpty()) {
        albumCoverPathEdit->setText(file);
    }
}

void AddAlbumUI::onContinueClicked()
{
    albumName = albumNameEdit->text().trimmed();
    songCount = songCountEdit->text().toInt();
    albumCoverPath = albumCoverPathEdit->text();

    if (albumName.isEmpty() || songCount <= 0 || albumCoverPath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Por favor completa todos los campos y selecciona la carátula.");
        return;
    }

    // Crear interfaz de pestañas
    QVBoxLayout *newLayout = new QVBoxLayout;
    tabWidget = new QTabWidget;

    for (int i = 1; i <= songCount; i++) {
        tabWidget->addTab(createSongTab(i), QString("Canción %1").arg(i));
    }

    uploadAlbumButton = new QPushButton("Subir Álbum");
    uploadAlbumButton->setStyleSheet("background-color: #1ED760; color: black; font-weight: bold; border-radius: 20px; padding: 8px 16px;");
    connect(uploadAlbumButton, &QPushButton::clicked, this, &AddAlbumUI::onCreateAlbumClicked);

    newLayout->addWidget(tabWidget);
    newLayout->addWidget(uploadAlbumButton, 0, Qt::AlignCenter);

    QLayout *oldLayout = this->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    setLayout(newLayout);
    setFixedSize(600, 700);
}

QWidget* AddAlbumUI::createSongTab(int trackNumber)
{
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *titleLabel = new QLabel("Título:");
    QLineEdit *titleEdit = new QLineEdit;
    titleEdit->setObjectName("titleEdit");

    QLabel *genreLabel = new QLabel("Género:");
    QComboBox *genreCombo = new QComboBox;
    genreCombo->addItems({"Pop","Corridos","Cristianos","Electrónica","Reguetón","Rock","Clásicas"});
    genreCombo->setObjectName("genreCombo");

    QLabel *durationLabel = new QLabel("Duración (mm:ss):");
    QLineEdit *durationEdit = new QLineEdit;
    durationEdit->setObjectName("durationEdit");

    QLabel *descLabel = new QLabel("Descripción:");
    QTextEdit *descEdit = new QTextEdit;
    descEdit->setObjectName("descEdit");

    QLabel *audioLabel = new QLabel("Archivo de audio:");
    QLineEdit *audioPathEdit = new QLineEdit;
    audioPathEdit->setReadOnly(true);
    audioPathEdit->setObjectName("audioPathEdit");
    QPushButton *selectAudioButton = new QPushButton("Seleccionar audio");
    connect(selectAudioButton, &QPushButton::clicked, this, [=]() { onSelectAudioClicked(tab); });

    layout->addWidget(titleLabel); layout->addWidget(titleEdit);
    layout->addWidget(genreLabel); layout->addWidget(genreCombo);
    layout->addWidget(durationLabel); layout->addWidget(durationEdit);
    layout->addWidget(descLabel); layout->addWidget(descEdit);

    QHBoxLayout *audioLayout = new QHBoxLayout;
    audioLayout->addWidget(audioPathEdit); audioLayout->addWidget(selectAudioButton);
    layout->addWidget(audioLabel); layout->addLayout(audioLayout);

    return tab;
}

void AddAlbumUI::onSelectAudioClicked(QWidget *tab)
{
    QString file = QFileDialog::getOpenFileName(this, "Seleccionar audio", "", "Audio Files (*.mp3 *.wav *.flac)");
    if (!file.isEmpty()) {
        tab->findChild<QLineEdit*>("audioPathEdit")->setText(file);
    }
}

bool AddAlbumUI::validateSongTab(QWidget *tab)
{
    return !(tab->findChild<QLineEdit*>("titleEdit")->text().trimmed().isEmpty() ||
             tab->findChild<QLineEdit*>("durationEdit")->text().trimmed().isEmpty() ||
             tab->findChild<QTextEdit*>("descEdit")->toPlainText().trimmed().isEmpty() ||
             tab->findChild<QLineEdit*>("audioPathEdit")->text().isEmpty());
}

QString AddAlbumUI::copyFileTo(const QString& sourcePath, const QString& destDir)
{
    QDir().mkpath(destDir);
    QFileInfo fileInfo(sourcePath);
    QString destPath = destDir + "/" + fileInfo.fileName();
    QFile::remove(destPath);
    QFile::copy(sourcePath, destPath);
    return destPath;
}

void AddAlbumUI::saveSongData(const QString& dir, const SongData& data)
{
    QFile file(dir + "/song.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15);
        out << data;
        file.close();
    }
}

void AddAlbumUI::onCreateAlbumClicked()
{
    QList<SongData> songs;

    for (int i = 0; i < songCount; i++) {
        QWidget *tab = tabWidget->widget(i);
        if (!validateSongTab(tab)) {
            QMessageBox::warning(this, "Error", QString("Faltan campos en la canción %1").arg(i+1));
            return;
        }

        QString songID = QUuid::createUuid().toString(QUuid::WithoutBraces);
        QString songTitle = tab->findChild<QLineEdit*>("titleEdit")->text().trimmed();
        QString genre = tab->findChild<QComboBox*>("genreCombo")->currentText();
        QString duration = tab->findChild<QLineEdit*>("durationEdit")->text().trimmed();
        QString description = tab->findChild<QTextEdit*>("descEdit")->toPlainText().trimmed();
        QString audioPath = tab->findChild<QLineEdit*>("audioPathEdit")->text();

        QString baseDir = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/";
        QString globalSongDir = baseDir + "globalsongs/" + songID;
        QString adminSongDir = baseDir + "admindata/" + adminUsername + "/artistsongs/" + songID;

        QString coverGlobal = copyFileTo(albumCoverPath, globalSongDir); // misma carátula
        QString audioGlobal = copyFileTo(audioPath, globalSongDir);

        copyFileTo(albumCoverPath, adminSongDir);
        copyFileTo(audioPath, adminSongDir);

        SongData data(songID,
                      songTitle,
                      genre,
                      duration,
                      description,
                      coverGlobal,
                      audioGlobal,
                      adminUsername,
                      QDateTime::currentDateTime(),
                      albumName,
                      i+1);

        saveSongData(globalSongDir, data);
        saveSongData(adminSongDir, data);

        songs.append(data);

        emit songAdded(data); // Igual que single
    }

    emit albumAdded(songs);

    // 🔹 Refrescar My Top Songs
    QWidget *p = parentWidget();
    while (p && !qobject_cast<AdminMenuUI*>(p))
        p = p->parentWidget();

    if (auto adminMenu = qobject_cast<AdminMenuUI*>(p)) {
        adminMenu->restoreMainView();
    }

    QMessageBox::information(this, "Éxito", "Álbum subido correctamente.");
    close();
}
