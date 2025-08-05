#ifndef ADDSINGLEUI_H
#define ADDSINGLEUI_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QString>

class AddSingleUI : public QWidget
{
    Q_OBJECT

public:
    explicit AddSingleUI(const QString& adminUsername, QWidget *parent = nullptr);

signals:
    void songAdded(const QString& title, const QString& coverPath, const QString& artist); // Puedes cambiar por SongData*

private slots:
    void onSelectCoverClicked();
    void onSelectAudioClicked();
    void onCreateSongClicked();

private:
    // Datos del admin (para la ruta)
    QString adminUsername;

    void saveSongData(const QString& dir, const QString& title, const QString& genre,
                      const QString& duration, const QString& desc, const QString& coverPath,
                      const QString& audioPath, const QString& artist);
    // Widgets de formulario
    QLabel *titleLabel, *genreLabel, *durationLabel, *descLabel, *coverLabel, *audioLabel;
    QLineEdit *titleEdit, *durationEdit;
    QComboBox *genreCombo;
    QTextEdit *descEdit;
    QLineEdit *coverPathEdit, *audioPathEdit;
    QPushButton *selectCoverButton, *selectAudioButton, *createSongButton;

    // Archivos seleccionados
    QString selectedCoverPath, selectedAudioPath;

    void resetForm();
    bool validateFields();
    QString copyFileTo(const QString& sourcePath, const QString& destDir);
};

#endif // ADDSINGLEUI_H
