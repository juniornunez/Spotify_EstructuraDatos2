#ifndef SONGDATA_H
#define SONGDATA_H

#include <QString>
#include <QDateTime>
#include <QDataStream>

class SongData
{
public:
    SongData();
    SongData(const QString &id,
             const QString &title,
             const QString &genre,
             const QString &duration,
             const QString &description,
             const QString &coverPath,
             const QString &audioPath,
             const QString &artist,
             const QDateTime &created,
             const QString &albumName = "",
             int trackNumber = -1);

    // Getters
    QString getId() const { return id; }
    QString getTitle() const { return title; }
    QString getGenre() const { return genre; }
    QString getDuration() const { return duration; }
    QString getDescription() const { return description; }
    QString getCoverPath() const { return coverPath; }
    QString getAudioPath() const { return audioPath; }
    QString getArtist() const { return artist; }
    QDateTime getCreated() const { return created; }
    QString getAlbumName() const { return albumName; }
    int getTrackNumber() const { return trackNumber; }

    // Setter para filePath (ruta .dat)
    QString getFilePath() const { return filePath; }
    void setFilePath(const QString &path) { filePath = path; }

    // Serialización
    friend QDataStream& operator<<(QDataStream &out, const SongData &data);
    friend QDataStream& operator>>(QDataStream &in, SongData &data);

private:
    QString id;
    QString title;
    QString genre;
    QString duration;
    QString description;
    QString coverPath;
    QString audioPath;
    QString artist;
    QDateTime created;

    QString albumName;   // NUEVO: nombre del álbum (vacío si es single)
    int trackNumber;     // NUEVO: número de pista (solo si pertenece a un álbum)

    QString filePath;    // Ruta del archivo .dat
};

#endif // SONGDATA_H
