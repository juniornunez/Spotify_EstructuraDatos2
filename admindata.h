#ifndef ADMINDATA_H
#define ADMINDATA_H

#include <QString>

class AdminData
{
public:
    AdminData();
    AdminData(int id, const QString &username, const QString &password,
              const QString &artisticName, const QString &realName,
              const QString &country, const QString &genre,
              const QString &bio, const QString &fecha,
              bool activo);

    static AdminData loadFromFile(const QString &filePath);

    int getId() const;
    QString getUsername() const;
    QString getPassword() const;
    QString getArtisticName() const;
    QString getRealName() const;
    QString getCountry() const;
    QString getGenre() const;
    QString getBio() const;
    QString getFecha() const;
    bool isActive() const;

    // Ruta calculada en tiempo de ejecución:
    QString getProfileImagePath() const;

private:
    int id;
    QString username, password, artisticName, realName, country, genre, bio, fecha;
    bool activo;
};

#endif // ADMINDATA_H
