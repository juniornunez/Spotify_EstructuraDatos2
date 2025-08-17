#ifndef ALBUMCARDWIDGET_H
#define ALBUMCARDWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class AlbumCardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AlbumCardWidget(const QString &albumName,
                             const QString &coverPath,
                             const QString &artistName,
                             QWidget *parent = nullptr);

signals:

    void albumDoubleClicked(const QString &albumName,
                            const QString &coverPath,
                            const QString &artistName);

protected:

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QString m_albumName;
    QString m_coverPath;
    QString m_artistName;

    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *artistLabel;
};

#endif // ALBUMCARDWIDGET_H
