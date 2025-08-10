#ifndef SONGCARDWIDGET_H
#define SONGCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class SongCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SongCardWidget(const QString &coverPath,
                            const QString &title,
                            const QString &artist,
                            const QString &audioPath,
                            QWidget *parent = nullptr);

    void setCover(const QString &coverPath);
    void setTitle(const QString &title);
    void setArtist(const QString &artist);

    // === NUEVOS GETTERS ===
    QString getCover() const { return coverPath_; }
    QString getTitle() const { return title_; }
    QString getArtist() const { return artist_; }
    QString getAudioPath() const { return audioPath_; }

    void setSelected(bool s);
    bool isSelected() const;

signals:
    void toggled(SongCardWidget *card, bool selected);
    void playPressed(SongCardWidget *card);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString coverPath_;
    QString title_;
    QString artist_;
    QString audioPath_;
    bool selected = false;

    QVBoxLayout *mainLayout;
    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *artistLabel;
    QPushButton *playButton;
};

#endif // SONGCARDWIDGET_H
