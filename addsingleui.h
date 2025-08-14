#ifndef ADDSINGLEUI_H
#define ADDSINGLEUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include "SongData.h"

class AddSingleUI : public QWidget
{
    Q_OBJECT
public:
    explicit AddSingleUI(const QString& adminUsername, QWidget *parent = nullptr);

signals:
    void songAdded(const SongData& data);

private slots:
    void onSelectCoverClicked();
    void onSelectAudioClicked();
    void onCreateSongClicked();

private:
    bool validateFields();
    QString copyFileTo(const QString& sourcePath, const QString& destDir);
    void saveSongData(const QString& dir, const SongData& data);
    void resetForm();

    QString adminUsername;
    QString selectedCoverPath;
    QString selectedAudioPath;

    QLabel *titleLabel;
    QLabel *genreLabel;
    QLabel *durationLabel;
    QLabel *descLabel;
    QLabel *coverLabel;
    QLabel *audioLabel;

    QLineEdit *titleEdit;
    QLineEdit *durationEdit;
    QComboBox *genreCombo;
    QTextEdit *descEdit;
    QLineEdit *coverPathEdit;
    QPushButton *selectCoverButton;
    QLineEdit *audioPathEdit;
    QPushButton *selectAudioButton;
    QPushButton *createSongButton;
};

#endif // ADDSINGLEUI_H

