#include "admindata.h"
#include <QFile>
#include <QDataStream>
#include <QDir>

AdminData::AdminData() : id(-1), activo(false) {}

AdminData::AdminData(int id, const QString &username, const QString &password,
                     const QString &artisticName, const QString &realName,
                     const QString &country, const QString &genre,
                     const QString &bio, const QString &fecha,
                     bool activo)
    : id(id), username(username), password(password), artisticName(artisticName), realName(realName),
    country(country), genre(genre), bio(bio), fecha(fecha), activo(activo) {}

AdminData AdminData::loadFromFile(const QString &filePath)
{
    QFile datFile(filePath);
    if (!datFile.open(QIODevice::ReadOnly)) return AdminData();
    QDataStream in(&datFile);
    in.setVersion(QDataStream::Qt_5_15);

    int id;
    QString username, password, artisticName, realName, country, genre, bio, fecha;
    bool activo;

    in >> id >> username >> password >> artisticName >> realName >> country >> genre >> bio >> fecha >> activo;

    datFile.close();
    return AdminData(id, username, password, artisticName, realName, country, genre, bio, fecha, activo);
}

// Getters
int AdminData::getId() const { return id; }
QString AdminData::getUsername() const { return username; }
QString AdminData::getPassword() const { return password; }
QString AdminData::getArtisticName() const { return artisticName; }
QString AdminData::getRealName() const { return realName; }
QString AdminData::getCountry() const { return country; }
QString AdminData::getGenre() const { return genre; }
QString AdminData::getBio() const { return bio; }
QString AdminData::getFecha() const { return fecha; }
bool AdminData::isActive() const { return activo; }

// Calcula el path de la imagen en tiempo de ejecución
QString AdminData::getProfileImagePath() const
{
    // Busca profile.(jpg|png|jpeg|bmp) en la carpeta del usuario
    QDir adminDir("C:/Users/moiza/Documents/QT/Spotify_Proyecto1/admindata/" + username);
    QStringList filters = {"profile.jpg", "profile.jpeg", "profile.png", "profile.bmp"};
    QStringList matches = adminDir.entryList(filters, QDir::Files);
    if (!matches.isEmpty())
        return adminDir.absoluteFilePath(matches.first());
    return ""; // No hay imagen
}
