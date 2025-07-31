#ifndef ARTISTSETTINGSUI_H
#define ARTISTSETTINGSUI_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class ArtistSettingsUI : public QWidget
{
    Q_OBJECT

public:
    explicit ArtistSettingsUI(QWidget *parent = nullptr);
    ~ArtistSettingsUI();

signals:
    void uploadSingleClicked();
    void uploadAlbumClicked();
    void uploadEPClicked();
    void manageMusicClicked();

private:
    QLabel *titleLabel;
    QPushButton *singleButton;
    QPushButton *albumButton;
    QPushButton *epButton;
    QPushButton *manageMusicButton;
};

#endif // ARTISTSETTINGSUI_H
