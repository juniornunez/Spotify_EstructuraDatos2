#include "trendingui.h"
#include <QVBoxLayout>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QScrollArea>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QGroupBox>
#include <QTime>
#include <algorithm>
#include "playbarui.h"
// ---------------- CONSTRUCTOR -----------------
TrendingUI::TrendingUI(const QString &username, QWidget *parent)
    : QWidget(parent), currentUser(username)
{
    setStyleSheet("background-color: #191414; color: white;");
    setMinimumSize(1000, 700);
    setWindowTitle("Trending - Musicfy");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tabs = new QTabWidget(this);
    tabs->setStyleSheet(
        "QTabBar::tab { background: #222; color: white; padding: 10px 20px; border-radius: 8px; }"
        "QTabBar::tab:selected { background: #1ED760; color: black; }"
        "QTableWidget { background: #222; color: white; gridline-color: #444; }"
        "QHeaderView::section { background-color: #333; color: #1ED760; padding: 5px; border: none; }"
        "QGroupBox { border: 1px solid #444; border-radius: 8px; margin-top: 10px; font-weight: bold; padding: 10px; }"
        );

    tabs->addTab(createTopRatedSongsTab(), "⭐ Top Rated Songs");
    tabs->addTab(createTopArtistsSongsTab(), "📊 Top Artists/Songs");
    tabs->addTab(createMyStatsTab(username), "📈 My Stats");

    mainLayout->addWidget(tabs);
}

// ---------------- TAB 1: TOP RATED SONGS -----------------
QWidget* TrendingUI::createTopRatedSongsTab()
{
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QMap<QString, double> ratings = getAllSongAverageRatings();

    // ordenar por mejor promedio
    QList<std::pair<QString, double>> sorted;
    for (auto it = ratings.begin(); it != ratings.end(); ++it)
        sorted.append({it.key(), it.value()});
    std::sort(sorted.begin(), sorted.end(),
              [](auto &a, auto &b){ return a.second > b.second; });

    QLabel *title = new QLabel("⭐ Songs with Best Ratings");
    title->setStyleSheet("font-size: 20px; font-weight: bold; margin-bottom: 15px;");
    layout->addWidget(title);

    QTableWidget *table = new QTableWidget(tab);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({ "Song", "Average Stars" });

    // ✅ Ajustes visuales
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->verticalHeader()->setDefaultSectionSize(40);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 🚫 No editable

    table->setRowCount(sorted.size());

    int row = 0;
    for (auto &p : sorted) {
        SongData song = loadSongFromId(p.first);
        QString displayName = song.getTitle() + " - " + song.getArtist();

        table->setItem(row, 0, new QTableWidgetItem(displayName));
        table->setItem(row, 1, new QTableWidgetItem(QString("⭐ %1").arg(p.second, 0, 'f', 1)));
        row++;
    }

    layout->addWidget(table);
    return tab;
}

// ---------------- TAB 2: TOP ARTISTS / SONGS -----------------
QWidget* TrendingUI::createTopArtistsSongsTab()
{
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QScrollArea *scroll = new QScrollArea;
    QWidget *scrollContent = new QWidget;
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollContent);

    // ---------------- TABLA DE ARTISTAS ----------------
    QMap<QString,int> artistPlays = getAllArtistPlayCounts();

    QList<std::pair<QString,int>> sortedArtists;
    for (auto it = artistPlays.begin(); it != artistPlays.end(); ++it)
        sortedArtists.append({it.key(), it.value()});
    std::sort(sortedArtists.begin(), sortedArtists.end(),
              [](auto &a, auto &b){ return a.second > b.second; });

    QTableWidget *artistTable = new QTableWidget(scrollContent);
    artistTable->setColumnCount(2);
    artistTable->setHorizontalHeaderLabels({ "Artist", "Total Plays" });
    artistTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    artistTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    artistTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    artistTable->setRowCount(sortedArtists.size());
    int row = 0;
    for (auto &p : sortedArtists) {
        artistTable->setItem(row, 0, new QTableWidgetItem(p.first));
        artistTable->setItem(row, 1, new QTableWidgetItem(QString::number(p.second)));
        row++;
    }

    QGroupBox *artistBox = new QGroupBox("🎤 Most Listened Artists");
    QVBoxLayout *artistLayout = new QVBoxLayout(artistBox);
    artistLayout->addWidget(artistTable);
    scrollLayout->addWidget(artistBox);

    // ---------------- TABLA DE CANCIONES ----------------
    QMap<QString,int> songPlays = getAllSongPlayCounts();

    QList<std::pair<QString,int>> sortedSongs;
    for (auto it = songPlays.begin(); it != songPlays.end(); ++it)
        sortedSongs.append({it.key(), it.value()});
    std::sort(sortedSongs.begin(), sortedSongs.end(),
              [](auto &a, auto &b){ return a.second > b.second; });

    QTableWidget *songTable = new QTableWidget(scrollContent);
    songTable->setColumnCount(2);
    songTable->setHorizontalHeaderLabels({ "Song", "Plays" });
    songTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    songTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    songTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    songTable->setRowCount(sortedSongs.size());
    row = 0;
    for (auto &p : sortedSongs) {
        songTable->setItem(row, 0, new QTableWidgetItem(p.first));
        songTable->setItem(row, 1, new QTableWidgetItem(QString::number(p.second)));
        row++;
    }

    QGroupBox *songBox = new QGroupBox("🎵 Most Listened Songs");
    QVBoxLayout *songLayout = new QVBoxLayout(songBox);
    songLayout->addWidget(songTable);
    scrollLayout->addWidget(songBox);

    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);
    layout->addWidget(scroll);
    return tab;
}

// ---------------- TAB 3: MY STATS -----------------
QWidget* TrendingUI::createMyStatsTab(const QString &username)
{
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    // --- Total Songs Listened ---
    int totalSongs = PlayBarUI::getTotalSongsListened(username);
    QLabel *songsLabel = new QLabel(QString("🎧 Total Songs Listened: %1").arg(totalSongs));
    songsLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    layout->addWidget(songsLabel);

    // --- Most Listened Songs ---
    QGroupBox *songsBox = new QGroupBox("🔥 Most Listened Songs");
    QVBoxLayout *songsLayout = new QVBoxLayout(songsBox);

    QMap<QString,int> plays = PlayBarUI::getPersonalSongPlayCounts(username);

    QList<std::pair<QString,int>> sortedPlays;
    for (auto it = plays.begin(); it != plays.end(); ++it)
        sortedPlays.append({it.key(), it.value()});
    std::sort(sortedPlays.begin(), sortedPlays.end(),
              [](auto &a, auto &b){ return a.second > b.second; });

    QTableWidget *songsTable = new QTableWidget;
    songsTable->setColumnCount(2);
    songsTable->setHorizontalHeaderLabels({"Song", "Plays"});
    songsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    songsTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    songsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    songsTable->setRowCount(sortedPlays.size());

    int row = 0;
    for (auto &p : sortedPlays) {
        SongData song = loadSongFromId(p.first);

        if (song.getId().isEmpty()) {
            // ⚠️ no encontró por ID, probamos con título
            song = loadSongFromTitle(p.first);
        }

        QString displayName;
        if (!song.getTitle().isEmpty())
            displayName = song.getTitle() + " - " + song.getArtist();
        else
            displayName = p.first; // por si acaso

        songsTable->setItem(row, 0, new QTableWidgetItem(displayName));
        songsTable->setItem(row, 1, new QTableWidgetItem(QString::number(p.second)));
        row++;
    }


    songsLayout->addWidget(songsTable);
    layout->addWidget(songsBox);

    // --- Average Rating Given ---
    double avgRating = getAverageRatingGiven(username);
    QLabel *ratingLabel = new QLabel(QString("⭐ Average Rating Given: %1").arg(avgRating, 0, 'f', 1));
    ratingLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    layout->addWidget(ratingLabel);

    // --- Last Rated Songs ---
    QGroupBox *lastRatedBox = new QGroupBox("📝 Last Rated Songs");
    QVBoxLayout *lastRatedLayout = new QVBoxLayout(lastRatedBox);

    QList<QPair<QString, QDateTime>> lastRated = getLastRatedSongs(username, 5);

    QTableWidget *lastRatedTable = new QTableWidget;
    lastRatedTable->setColumnCount(2);
    lastRatedTable->setHorizontalHeaderLabels({"Song", "Rated At"});
    lastRatedTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    lastRatedTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    lastRatedTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    lastRatedTable->setRowCount(lastRated.size());

    row = 0;
    for (auto &p : lastRated) {
        SongData song = loadSongFromId(p.first); // ✅ mostrar título+artista
        QString displayName = song.getTitle() + " - " + song.getArtist();

        lastRatedTable->setItem(row, 0, new QTableWidgetItem(displayName));
        lastRatedTable->setItem(row, 1,
                                new QTableWidgetItem(p.second.toString("dd/MM/yyyy hh:mm")));
        row++;
    }
    lastRatedLayout->addWidget(lastRatedTable);
    layout->addWidget(lastRatedBox);

    layout->addStretch();
    return tab;
}


// ---------------- PROMEDIOS (usa ID en lugar de título) ----------------
QMap<QString, double> TrendingUI::getAllSongAverageRatings()
{
    QMap<QString, QList<int>> ratings;

    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/ratingsongs");
    for (QString user : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir userDir(baseDir.filePath(user));
        for (QString ratingFile : userDir.entryList(QStringList() << "*.dat", QDir::Files)) {
            QString songId = QFileInfo(ratingFile).baseName(); // ✅ usar ID
            QFile f(userDir.filePath(ratingFile));
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f);
                in.setVersion(QDataStream::Qt_5_15);
                int rating; in >> rating;
                ratings[songId].append(rating);
                f.close();
            }
        }
    }

    QMap<QString, double> averages;
    for (auto it = ratings.begin(); it != ratings.end(); ++it) {
        double sum = 0;
        for (int r : it.value()) sum += r;
        averages[it.key()] = sum / it.value().size();
    }

    return averages;
}


int TrendingUI::getPlayCount(const QString &artist, const QString &song)
{
    QString filePath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/%1/songsplays/%2/plays.dat")
    .arg(artist, song);
    QFile f(filePath);
    int plays = 0;
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream in(&f); in >> plays;
    }
    return plays;
}

QMap<QString,int> TrendingUI::getAllSongPlayCounts()
{
    QMap<QString,int> result;
    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata");

    for (QString user : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir playsDir(baseDir.filePath(user + "/songsplays"));
        QStringList files = playsDir.entryList(QStringList() << "*.dat", QDir::Files);

        for (QString file : files) {
            QString songTitle = file.left(file.size() - 4); // quitar ".dat"
            QFile f(playsDir.filePath(file));
            if (f.open(QIODevice::ReadOnly)) {
                QDataStream in(&f); int plays; in >> plays;
                result[songTitle] += plays; // acumulamos plays globales
                f.close();
            }
        }
    }
    return result;
}

QMap<QString,int> TrendingUI::getAllArtistPlayCounts()
{
    QMap<QString,int> result;
    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata");

    for (QString user : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir playsDir(baseDir.filePath(user + "/songsplays"));
        QStringList files = playsDir.entryList(QStringList() << "*.dat", QDir::Files);

        for (QString file : files) {
            QString songTitle = file.left(file.size() - 4);
            SongData song = loadSongFromTitle(songTitle);
            if (!song.getArtist().isEmpty()) {
                QFile f(playsDir.filePath(file));
                if (f.open(QIODevice::ReadOnly)) {
                    QDataStream in(&f); int plays; in >> plays;
                    result[song.getArtist()] += plays; // acumulamos plays por artista
                    f.close();
                }
            }
        }
    }
    return result;
}




// ---------------- HELPERS PERSONALES -----------------
int TrendingUI::getTotalSongsListened(const QString &username)
{
    QDir dir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/userdata/%1/plays").arg(username));
    return dir.exists() ? dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot).size() : 0;
}

qint64 TrendingUI::getTotalListeningTime(const QString &username)
{
    QString filePath = QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/userdata/%1/total_time.dat").arg(username);
    QFile f(filePath);
    qint64 total = 0;
    if (f.open(QIODevice::ReadOnly)) {
        QDataStream in(&f); in >> total;
    }
    return total;
}

QMap<QString,int> TrendingUI::getPersonalSongPlayCounts(const QString &username)
{
    QMap<QString,int> plays;
    QDir dir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/userdata/%1/songsplays").arg(username));

    // ✅ iterar carpetas con IDs de canción
    for (QString songId : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QFile f(dir.filePath(songId + "/plays.dat"));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f);
            int count;
            in >> count;
            plays.insert(songId, count); // ✅ usar ID como clave
        }
    }
    return plays;
}


double TrendingUI::getAverageRatingGiven(const QString &username)
{
    QDir dir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/ratingsongs/%1").arg(username));
    QStringList files = dir.entryList(QStringList() << "*.dat", QDir::Files);
    int total = 0, count = 0;
    for (QString file : files) {
        QFile f(dir.filePath(file));
        if (f.open(QIODevice::ReadOnly)) {
            QDataStream in(&f); int rating; in >> rating;
            total += rating; count++;
        }
    }
    return count > 0 ? (double)total / count : 0.0;
}

QList<QPair<QString, QDateTime>> TrendingUI::getLastRatedSongs(const QString &username, int limit)
{
    QList<QPair<QString, QDateTime>> result;
    QDir userDir(QString("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/ratingsongs/%1").arg(username));
    QStringList files = userDir.entryList(QStringList() << "*.dat", QDir::Files, QDir::Time);

    int count = 0;
    for (const QString &file : files) {
        if (count >= limit) break;
        QString songId = QFileInfo(file).baseName(); // ✅ usar ID
        QFileInfo info(userDir.filePath(file));
        result.append({songId, info.lastModified()});
        count++;
    }

    return result;
}

SongData TrendingUI::loadSongFromTitle(const QString &title)
{
    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
    for (QString folder : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir songDir(baseDir.filePath(folder));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);

        if (datFiles.isEmpty()) continue;

        QFile f(songDir.absoluteFilePath(datFiles.first()));
        if (!f.open(QIODevice::ReadOnly)) continue;

        QDataStream in(&f);
        in.setVersion(QDataStream::Qt_5_15);

        SongData song;
        in >> song;
        f.close();

        if (song.getTitle() == title) {
            return song; // ✅ coincidencia por título
        }
    }

    return SongData(); // vacío si no se encontró
}

SongData TrendingUI::loadSongFromId(const QString &id)
{
    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs");
    for (QString folder : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir songDir(baseDir.filePath(folder));
        QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);

        if (datFiles.isEmpty()) continue;

        QFile f(songDir.absoluteFilePath(datFiles.first()));
        if (!f.open(QIODevice::ReadOnly)) continue;

        QDataStream in(&f);
        in.setVersion(QDataStream::Qt_5_15);

        SongData song;
        in >> song;
        f.close();

        if (song.getId() == id) {
            return song; // ✅ coincidencia por ID
        }
    }

    return SongData(); // vacío si no se encontró
}


