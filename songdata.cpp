#include "songdata.h"

SongData::SongData()
    : trackNumber(-1) // default: no es parte de álbum
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
                   const QDateTime &created,
                   const QString &albumName,
                   int trackNumber)
    : id(id),
    title(title),
    genre(genre),
    duration(duration),
    description(description),
    coverPath(coverPath),
    audioPath(audioPath),
    artist(artist),
    created(created),
    albumName(albumName),
    trackNumber(trackNumber)
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
        << data.created
        << data.albumName
        << data.trackNumber;
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

    // Lectura segura: si el archivo no tiene álbum/trackNumber (viejos .dat), dejar valores por defecto
    if (!in.atEnd()) {
        in >> data.albumName;
    } else {
        data.albumName = "";
    }

    if (!in.atEnd()) {
        in >> data.trackNumber;
    } else {
        data.trackNumber = -1;
    }

    return in;
}
