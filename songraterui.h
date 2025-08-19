#ifndef SONGRATERUI_H
#define SONGRATERUI_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QScrollArea>
#include <QHash>
#include "songdata.h"
#include "horizontalsongcard.h"

class SongRaterUI : public QWidget
{
    Q_OBJECT
public:
    explicit SongRaterUI(const QString &username, QWidget *parent = nullptr);

    // 📊 Estadísticas personales
    double getAverageRatingGiven() const;                      // Promedio de calificaciones dadas
    QList<QPair<QString, int>> getLastRatedSongs(int count);   // Últimas canciones calificadas

private:
    void loadSongs();
    void saveSongRating(const QString &songId, int rating);
    int loadSongRating(const QString &songId);
    void deleteSongRating(const QString &songId);

    QString username;

    // 📌 Layouts y UI
    QVBoxLayout *mainLayout;
    QLineEdit *searchBar;
    QScrollArea *scroll;
    QWidget *container;
    QVBoxLayout *songsLayout;

    // 📌 Estructuras auxiliares
    QHash<QString, SongData> songHash;  // SongId -> SongData
    QHash<QString, QWidget*> rowHash;   // SongId -> Widget de la fila
};

#endif // SONGRATERUI_H
