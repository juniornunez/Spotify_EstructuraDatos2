#include "SongData.h"

SongData::SongData()
{
}

SongData::SongData(const QString &id,
                   const QString &title,
                   const QString &genre,
                   const QString &duration,
                   const QString &description,
                   const QString &coverPath,
                   const QString &audioPath,
                   const QString &artist,
                   const QDateTime &created)
    : id(id),
    title(title),
    genre(genre),
    duration(duration),
    description(description),
    coverPath(coverPath),
    audioPath(audioPath),
    artist(artist),
    created(created)
{
}

QDataStream& operator<<(QDataStream &out, const SongData &data)
{
    out << data.id
        << data.title
        << data.genre
        << data.duration
        << data.description
        << data.coverPath
        << data.audioPath
        << data.artist
        << data.created;
    return out;
}

QDataStream& operator>>(QDataStream &in, SongData &data)
{
    in >> data.id
        >> data.title
        >> data.genre
        >> data.duration
        >> data.description
        >> data.coverPath
        >> data.audioPath
        >> data.artist
        >> data.created;
    return in;
}
