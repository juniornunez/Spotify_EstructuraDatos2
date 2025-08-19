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
#include <algorithm>

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
    tabs->addTab(createMyStatsTab(), "📈 My Stats");

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

    table->setStyleSheet(
        "QTableWidget { gridline-color: #333; font-size: 14px; }"
        "QHeaderView::section { background-color: #222; color: #1ED760; padding: 8px; font-size: 14px; }"
        "QTableWidget::item { padding: 10px; }"
        );

    table->setRowCount(sorted.size());

    int row = 0;
    for (auto &p : sorted) {
        SongData song = loadSongFromId(p.first);
        QString displayName = song.getTitle() + " - " + song.getArtist();

        QTableWidgetItem *nameItem = new QTableWidgetItem(displayName);
        nameItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        nameItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        QTableWidgetItem *ratingItem = new QTableWidgetItem(QString("⭐ %1").arg(p.second, 0, 'f', 1));
        ratingItem->setTextAlignment(Qt::AlignCenter);
        ratingItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        table->setItem(row, 0, nameItem);
        table->setItem(row, 1, ratingItem);
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

    // 🔹 Convertir a lista y ordenar por plays (descendente)
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
    artistTable->verticalHeader()->setDefaultSectionSize(40);
    artistTable->verticalHeader()->setVisible(false);
    artistTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 🚫 No editable

    artistTable->setStyleSheet(
        "QTableWidget { gridline-color: #333; font-size: 14px; }"
        "QHeaderView::section { background-color: #222; color: #1ED760; padding: 8px; font-size: 14px; }"
        "QTableWidget::item { padding: 10px; }"
        );

    artistTable->setRowCount(sortedArtists.size());
    int row = 0;
    for (auto &p : sortedArtists) {
        QTableWidgetItem *artistItem = new QTableWidgetItem(p.first);
        artistItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        artistItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        QTableWidgetItem *playsItem = new QTableWidgetItem(QString::number(p.second));
        playsItem->setTextAlignment(Qt::AlignCenter);
        playsItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        artistTable->setItem(row, 0, artistItem);
        artistTable->setItem(row, 1, playsItem);
        row++;
    }

    QGroupBox *artistBox = new QGroupBox("🎤 Most Listened Artists");
    QVBoxLayout *artistLayout = new QVBoxLayout(artistBox);
    artistLayout->addWidget(artistTable);
    scrollLayout->addWidget(artistBox);

    // ---------------- TABLA DE CANCIONES ----------------
    QMap<QString,int> songPlays = getAllSongPlayCounts();

    // 🔹 Convertir a lista y ordenar por plays (descendente)
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
    songTable->verticalHeader()->setDefaultSectionSize(40);
    songTable->verticalHeader()->setVisible(false);
    songTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // 🚫 No editable

    songTable->setStyleSheet(
        "QTableWidget { gridline-color: #333; font-size: 14px; }"
        "QHeaderView::section { background-color: #222; color: #1ED760; padding: 8px; font-size: 14px; }"
        "QTableWidget::item { padding: 10px; }"
        );

    songTable->setRowCount(sortedSongs.size());
    row = 0;
    for (auto &p : sortedSongs) {
        QTableWidgetItem *songItem = new QTableWidgetItem(p.first);
        songItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        songItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        QTableWidgetItem *playsItem = new QTableWidgetItem(QString::number(p.second));
        playsItem->setTextAlignment(Qt::AlignCenter);
        playsItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        songTable->setItem(row, 0, songItem);
        songTable->setItem(row, 1, playsItem);
        row++;
    }

    QGroupBox *songBox = new QGroupBox("🎵 Most Listened Songs");
    QVBoxLayout *songLayout = new QVBoxLayout(songBox);
    songLayout->addWidget(songTable);
    scrollLayout->addWidget(songBox);

    // ---------------- SCROLL ----------------
    scroll->setWidget(scrollContent);
    scroll->setWidgetResizable(true);

    layout->addWidget(scroll);
    return tab;
}


// ---------------- TAB 3: MY STATS -----------------
QWidget* TrendingUI::createMyStatsTab()
{
    QWidget *tab = new QWidget;
    QVBoxLayout *layout = new QVBoxLayout(tab);

    QLabel *title = new QLabel("📈 Your Most Played Songs");
    title->setStyleSheet("font-size: 20px; font-weight: bold; margin-bottom: 15px;");
    layout->addWidget(title);

    QTableWidget *table = new QTableWidget(tab);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({ "Song", "Plays" });

    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->verticalHeader()->setDefaultSectionSize(40);
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 🚫 No editable

    table->setStyleSheet(
        "QTableWidget { gridline-color: #333; font-size: 14px; }"
        "QHeaderView::section { background-color: #222; color: #1ED760; padding: 8px; font-size: 14px; }"
        "QTableWidget::item { padding: 10px; }"
        );

    QMap<QString,int> songPlays = getAllSongPlayCounts();
    table->setRowCount(songPlays.size());

    int row = 0;
    for (auto it = songPlays.begin(); it != songPlays.end(); ++it) {
        QTableWidgetItem *songItem = new QTableWidgetItem(it.key());
        songItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        songItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        QTableWidgetItem *playsItem = new QTableWidgetItem(QString::number(it.value()));
        playsItem->setTextAlignment(Qt::AlignCenter);
        playsItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);

        table->setItem(row, 0, songItem);
        table->setItem(row, 1, playsItem);
        row++;
    }

    layout->addWidget(table);
    return tab;
}


// ---------------- HELPERS -----------------
QMap<QString, double> TrendingUI::getAllSongAverageRatings()
{
    // 🔹 songId -> lista de ratings
    QMap<QString, QList<int>> ratings;

    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/ratingsongs");
    for (QString user : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir userDir(baseDir.filePath(user));
        for (QString ratingFile : userDir.entryList(QStringList() << "*.dat", QDir::Files)) {
            QString songId = ratingFile.left(ratingFile.size() - 4); // quitar ".dat"
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

    // 🔹 calcular promedios
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
        QDataStream in(&f);
        in >> plays;
        f.close();
    }
    return plays;
}

QMap<QString,int> TrendingUI::getAllSongPlayCounts()
{
    QMap<QString,int> result;
    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata");
    for (QString artist : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir songsDir(baseDir.filePath(artist + "/songsplays"));
        for (QString song : songsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            int plays = getPlayCount(artist, song);
            result.insert(song, plays);
        }
    }
    return result;
}

QMap<QString,int> TrendingUI::getAllArtistPlayCounts()
{
    QMap<QString,int> result;
    QDir baseDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata");
    for (QString artist : baseDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QDir songsDir(baseDir.filePath(artist + "/songsplays"));
        int total = 0;
        for (QString song : songsDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            total += getPlayCount(artist, song);
        }
        result.insert(artist, total);
    }
    return result;
}

// ---------------- HELPERS -----------------
SongData TrendingUI::loadSongFromId(const QString &songId)
{
    QDir songDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/globalsongs/" + songId);
    QStringList datFiles = songDir.entryList(QStringList() << "*.dat", QDir::Files);
    if (datFiles.isEmpty()) return SongData();

    QFile f(songDir.absoluteFilePath(datFiles.first()));
    if (!f.open(QIODevice::ReadOnly)) return SongData();

    QDataStream in(&f);
    in.setVersion(QDataStream::Qt_5_15);

    SongData song;
    in >> song;
    f.close();

    return song;
}


