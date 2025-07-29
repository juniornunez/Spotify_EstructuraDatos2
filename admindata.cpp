#include "admindata.h"
#include <QFile>
#include <QDataStream>

AdminData::AdminData() : id(-1), activo(false) {}

AdminData::AdminData(int id, const QString &username, const QString &password,
                     const QString &artisticName, const QString &realName,
                     const QString &country, const QString &genre,
                     const QString &bio, const QString &fecha,
                     const QString &imagePath, bool activo)
    : id(id), username(username), password(password), artisticName(artisticName), realName(realName),
    country(country), genre(genre), bio(bio), fecha(fecha), imagePath(imagePath), activo(activo) {}

AdminData AdminData::loadFromFile(const QString &filePath)
{
    QFile binFile(filePath);
    if (!binFile.open(QIODevice::ReadOnly)) return AdminData();
    QDataStream in(&binFile);
    in.setVersion(QDataStream::Qt_5_15);

    int id;
    QString username, password, artisticName, realName, country, genre, bio, fecha, imagePath;
    bool activo;

    in >> id >> username >> password >> artisticName >> realName >> country >> genre >> bio >> fecha >> imagePath >> activo;

    binFile.close();
    return AdminData(id, username, password, artisticName, realName, country, genre, bio, fecha, imagePath, activo);
}

// Getters...
int AdminData::getId() const { return id; }
QString AdminData::getUsername() const { return username; }
QString AdminData::getPassword() const { return password; }
QString AdminData::getArtisticName() const { return artisticName; }
QString AdminData::getRealName() const { return realName; }
QString AdminData::getCountry() const { return country; }
QString AdminData::getGenre() const { return genre; }
QString AdminData::getBio() const { return bio; }
QString AdminData::getFecha() const { return fecha; }
QString AdminData::getImagePath() const { return imagePath; }
bool AdminData::isActive() const { return activo; }
