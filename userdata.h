#ifndef USERDATA_H
#define USERDATA_H

#include <string>

class UserData {
public:
    UserData();
    UserData(int id, const std::string& username, const std::string& password, int tipo, bool activo);

    bool authenticate(const std::string& username, const std::string& password);
    int getId() const;
    std::string getUsername() const;
    int getType() const;
    bool isActive() const;
    bool loadByUsername(const std::string& username);

    // Nuevos: para guardar usuario
    bool saveToFile(const std::string& filename = "usuarios.dat") const;

private:
    int id;
    std::string username;
    std::string password;
    int tipo;   // 1 = admin/artista, 2 = usuario
    bool activo;

    bool matchPassword(const std::string& pass) const;
};

#endif // USERDATA_H
