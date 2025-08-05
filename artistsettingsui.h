#ifndef ARTISTSETTINGSUI_H
#define ARTISTSETTINGSUI_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class ArtistSettingsUI : public QDialog
{
    Q_OBJECT

public:
    explicit ArtistSettingsUI(const QString &adminUsername, QWidget *parent = nullptr);
    ~ArtistSettingsUI();

signals:
    void songUploaded(const QString &title, const QString &coverPath, const QString &artist); // <-- ESTA

    void uploadSingleClicked();
    void uploadAlbumClicked();
    void uploadEPClicked();
    void manageMusicClicked();

private:
    QString adminUsername;
    QLabel *titleLabel;
    QPushButton *singleButton;
    QPushButton *albumButton;
    QPushButton *epButton;
    QPushButton *manageMusicButton;
};

#endif // ARTISTSETTINGSUI_H
