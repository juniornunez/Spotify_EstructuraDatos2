#ifndef TRENDINGUI_H
#define TRENDINGUI_H

#include <QWidget>
#include <QTabWidget>
#include <QMap>
#include <QDateTime>
#include "songdata.h"

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
    QWidget* createMyStatsTab(const QString &username);

    // --- Helpers generales ---
    QMap<QString, double> getAllSongAverageRatings();
    int getPlayCount(const QString &artist, const QString &song);
    QMap<QString,int> getAllSongPlayCounts();
    QMap<QString,int> getAllArtistPlayCounts();
    SongData loadSongFromId(const QString &id);

    SongData loadSongFromTitle(const QString &songTitle);
    // --- Helpers personales ---
    int getTotalSongsListened(const QString &username);
    qint64 getTotalListeningTime(const QString &username);
    QMap<QString,int> getPersonalSongPlayCounts(const QString &username);
    double getAverageRatingGiven(const QString &username);
    QList<QPair<QString, QDateTime>> getLastRatedSongs(const QString &username, int limit);
};

#endif // TRENDINGUI_H
