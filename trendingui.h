#ifndef TRENDINGUI_H
#define TRENDINGUI_H

#include <QDialog>
#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>

class TrendingUI : public QDialog
{
    Q_OBJECT
public:
    explicit TrendingUI(QWidget *parent = nullptr);

private:
    QLabel *topSongsLabel;
    QListWidget *topSongsList;
    QLabel *albumsLabel;
    QWidget *artistsContainer;
};

#endif // TRENDINGUI_H
