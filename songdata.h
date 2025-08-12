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
             const QDateTime &created);

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

    // Setters
    void setId(const QString &newId) { id = newId; }
    void setTitle(const QString &newTitle) { title = newTitle; }
    void setGenre(const QString &newGenre) { genre = newGenre; }
    void setDuration(const QString &newDuration) { duration = newDuration; }
    void setDescription(const QString &newDescription) { description = newDescription; }
    void setCoverPath(const QString &newCoverPath) { coverPath = newCoverPath; }
    void setAudioPath(const QString &newAudioPath) { audioPath = newAudioPath; }
    void setArtist(const QString &newArtist) { artist = newArtist; }
    void setCreated(const QDateTime &newCreated) { created = newCreated; }

    // Operadores para serialización
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
};

#endif // SONGDATA_H
