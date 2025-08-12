#include "addsingleui.h"
#include "SongData.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QRegularExpression>
#include <QUuid>

AddSingleUI::AddSingleUI(const QString& adminUsername, QWidget *parent)
    : QWidget(parent), adminUsername(adminUsername)
{
    setStyleSheet("background-color: #191414; color: white;");

    titleLabel = new QLabel("Title:");
    genreLabel = new QLabel("Genre:");
    durationLabel = new QLabel("Duration (mm:ss):");
    descLabel = new QLabel("Description:");
    coverLabel = new QLabel("Cover image:");
    audioLabel = new QLabel("Audio file:");

    titleEdit = new QLineEdit;
    titleEdit->setStyleSheet("background: #121212; color: white; border-radius: 5px; padding: 6px;");
    durationEdit = new QLineEdit;
    durationEdit->setStyleSheet("background: #121212; color: white; border-radius: 5px; padding: 6px;");
    genreCombo = new QComboBox;
    genreCombo->addItems({"Pop","Corridos","Cristianos","Electrónica","Reguetón","Rock","Clásicas"});
    genreCombo->setStyleSheet("background: #222; color: white; border-radius: 5px; padding: 6px;");
    descEdit = new QTextEdit;
    descEdit->setStyleSheet("background: #121212; color: white; border-radius: 5px;");

    coverPathEdit = new QLineEdit; coverPathEdit->setReadOnly(true);
    coverPathEdit->setStyleSheet("background: #222; color: #bbb; border-radius: 5px;");
    selectCoverButton = new QPushButton("Select image");
    selectCoverButton->setStyleSheet("background: #222; color: #1ED760; border-radius: 8px; font-weight: bold;");
    connect(selectCoverButton, &QPushButton::clicked, this, &AddSingleUI::onSelectCoverClicked);

    audioPathEdit = new QLineEdit; audioPathEdit->setReadOnly(true);
    audioPathEdit->setStyleSheet("background: #222; color: #bbb; border-radius: 5px;");
    selectAudioButton = new QPushButton("Select audio");
    selectAudioButton->setStyleSheet("background: #222; color: #1ED760; border-radius: 8px; font-weight: bold;");
    connect(selectAudioButton, &QPushButton::clicked, this, &AddSingleUI::onSelectAudioClicked);

    createSongButton = new QPushButton("Create song");
    createSongButton->setStyleSheet("background-color: #1ED760; color: black; font-size: 14pt; border-radius: 24px; font-weight: bold;");
    connect(createSongButton, &QPushButton::clicked, this, &AddSingleUI::onCreateSongClicked);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(titleLabel); mainLayout->addWidget(titleEdit);
    mainLayout->addWidget(genreLabel); mainLayout->addWidget(genreCombo);
    mainLayout->addWidget(durationLabel); mainLayout->addWidget(durationEdit);
    mainLayout->addWidget(descLabel); mainLayout->addWidget(descEdit);

    QHBoxLayout *coverLayout = new QHBoxLayout;
    coverLayout->addWidget(coverPathEdit); coverLayout->addWidget(selectCoverButton);
    mainLayout->addWidget(coverLabel); mainLayout->addLayout(coverLayout);

    QHBoxLayout *audioLayout = new QHBoxLayout;
    audioLayout->addWidget(audioPathEdit); audioLayout->addWidget(selectAudioButton);
    mainLayout->addWidget(audioLabel); mainLayout->addLayout(audioLayout);

    mainLayout->addSpacing(12);
    mainLayout->addWidget(createSongButton, 0, Qt::AlignCenter);

    setLayout(mainLayout);
    setFixedSize(430, 630);
    setWindowTitle("Upload Single");
}

void AddSingleUI::onSelectCoverClicked() {
    QString file = QFileDialog::getOpenFileName(this, "Select cover image", "", "Images (*.png *.jpg *.jpeg *.bmp)");
    if (!file.isEmpty()) {
        selectedCoverPath = file;
        coverPathEdit->setText(file);
    }
}

void AddSingleUI::onSelectAudioClicked() {
    QString file = QFileDialog::getOpenFileName(this, "Select audio file", "", "Audio Files (*.mp3 *.wav *.flac)");
    if (!file.isEmpty()) {
        selectedAudioPath = file;
        audioPathEdit->setText(file);
    }
}

bool AddSingleUI::validateFields() {
    return !(titleEdit->text().trimmed().isEmpty() ||
             durationEdit->text().trimmed().isEmpty() ||
             descEdit->toPlainText().trimmed().isEmpty() ||
             selectedCoverPath.isEmpty() ||
             selectedAudioPath.isEmpty());
}

QString AddSingleUI::copyFileTo(const QString& sourcePath, const QString& destDir) {
    QDir().mkpath(destDir);
    QFileInfo fileInfo(sourcePath);
    QString destPath = destDir + "/" + fileInfo.fileName();
    QFile::remove(destPath);
    QFile::copy(sourcePath, destPath);
    return destPath;
}

void AddSingleUI::onCreateSongClicked() {
    if (!validateFields()) {
        QMessageBox::warning(this, "Error", "All fields are required.");
        return;
    }

    QString songID = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QString songTitle = titleEdit->text().trimmed();

    QString baseDir = "C:/Users/moiza/Documents/QT/Spotify_Proyecto1/";
    QString globalSongDir = baseDir + "globalsongs/" + songID;
    QString adminSongDir = baseDir + "admindata/" + adminUsername + "/artistsongs/" + songID;

    QString coverGlobal = copyFileTo(selectedCoverPath, globalSongDir);
    QString audioGlobal = copyFileTo(selectedAudioPath, globalSongDir);

    QString coverAdmin = copyFileTo(selectedCoverPath, adminSongDir);
    QString audioAdmin = copyFileTo(selectedAudioPath, adminSongDir);

    SongData data(songID,
                  songTitle,
                  genreCombo->currentText(),
                  durationEdit->text().trimmed(),
                  descEdit->toPlainText().trimmed(),
                  coverGlobal,
                  audioGlobal,
                  adminUsername,
                  QDateTime::currentDateTime());

    saveSongData(globalSongDir, data);
    saveSongData(adminSongDir, data);

    emit songAdded(data);

    QMessageBox::information(this, "Success", "Song uploaded successfully!");
    resetForm();
}

void AddSingleUI::saveSongData(const QString& dir, const SongData& data) {
    QFile file(dir + "/song.dat");
    if (file.open(QIODevice::WriteOnly)) {
        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_5_15);
        out << data;
        file.close();
    }
}

void AddSingleUI::resetForm() {
    titleEdit->clear();
    durationEdit->clear();
    descEdit->clear();
    coverPathEdit->clear();
    audioPathEdit->clear();
    selectedCoverPath.clear();
    selectedAudioPath.clear();
}

