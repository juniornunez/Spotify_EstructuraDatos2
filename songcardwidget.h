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
    explicit SongCardWidget(const QString& coverPath,
                            const QString& title,
                            const QString& artist,
                            QWidget *parent = nullptr);

    void setCover(const QString& coverPath);
    void setTitle(const QString& title);
    void setArtist(const QString& artist);

    void setSelected(bool selected);
    bool isSelected() const;

signals:
    void toggled(SongCardWidget* card, bool selected);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *artistLabel;
    QVBoxLayout *mainLayout;
    QPushButton *playButton;

    bool selected = false;
};

#endif // SONGCARDWIDGET_H
