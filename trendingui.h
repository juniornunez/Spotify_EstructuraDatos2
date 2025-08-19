#ifndef TRENDINGUI_H
#define TRENDINGUI_H

#include <QDialog>
#include <QTabWidget>
#include <QMap>
#include "songdata.h"   // ⚠️ Asegúrate de incluir tu clase SongData

class TrendingUI : public QWidget
{
    Q_OBJECT

public:
    explicit TrendingUI(const QString &username, QWidget *parent = nullptr);

private:
    QString currentUser;
    QTabWidget *tabs;

    QWidget* createTopRatedSongsTab();
    QWidget* createTopArtistsSongsTab();
    QWidget* createMyStatsTab();

    // helpers
    QMap<QString, double> getAllSongAverageRatings(); // ⭐ promedio global
    SongData loadSongFromId(const QString &songId);   // 🔹 NUEVO
    int getPlayCount(const QString &artist, const QString &song);
    QMap<QString,int> getAllSongPlayCounts();
    QMap<QString,int> getAllArtistPlayCounts();
};

#endif // TRENDINGUI_H
