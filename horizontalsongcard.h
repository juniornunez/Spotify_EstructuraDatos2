#ifndef HORIZONTALSONGCARD_H
#define HORIZONTALSONGCARD_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

class HorizontalSongCard : public QWidget
{
    Q_OBJECT
public:
    explicit HorizontalSongCard(int position,
                                const QString &coverPath,
                                const QString &title,
                                const QString &artist,
                                QWidget *parent = nullptr);

private:
    QLabel *posLabel;
    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *artistLabel;
};

#endif // HORIZONTALSONGCARD_H
