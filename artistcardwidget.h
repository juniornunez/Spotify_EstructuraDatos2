#ifndef ARTISTCARDWIDGET_H
#define ARTISTCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QMouseEvent>

class ArtistCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ArtistCardWidget(const QString &artistName,
                              const QString &profilePicPath,
                              QWidget *parent = nullptr);

    QString getArtistName() const { return artistName; }

signals:
    void doubleClicked(const QString &artistName);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QString artistName;
    QLabel *coverLabel;
    QLabel *nameLabel;
};

#endif // ARTISTCARDWIDGET_H
