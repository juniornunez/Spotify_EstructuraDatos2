#ifndef HORIZONTALSONGCARD_H
#define HORIZONTALSONGCARD_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QString>
#include <QMouseEvent>

class HorizontalSongCard : public QWidget
{
    Q_OBJECT

signals:
    void songDoubleClicked(const QString &coverPath,
                           const QString &title,
                           const QString &artist,
                           const QString &audioPath);

public:
    explicit HorizontalSongCard(int position,
                                const QString &coverPath,
                                const QString &title,
                                const QString &artist,
                                const QString &audioPath,   // 🔑 nuevo parámetro
                                QWidget *parent = nullptr);

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    QLabel *posLabel;
    QLabel *coverLabel;
    QLabel *titleLabel;
    QLabel *artistLabel;

    // 🔑 datos guardados para emitir en la señal
    QString coverPath;
    QString audioPath;
};

#endif // HORIZONTALSONGCARD_H
