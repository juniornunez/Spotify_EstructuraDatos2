#include "userdata.h"
#include <fstream>
#include <cstring>

UserData::UserData() : id(-1), tipo(2), activo(false) {}

UserData::UserData(int id, const std::string& username, const std::string& password, int tipo, bool activo)
    : id(id), username(username), password(password), tipo(tipo), activo(activo) {}

bool UserData::authenticate(const std::string& uname, const std::string& pass) {
    if (loadByUsername(uname)) {
        if (activo && matchPassword(pass)) {
            return true;
        }
    }
    return false;
}

bool UserData::loadByUsername(const std::string& uname) {
    std::ifstream file("usuarios.dat", std::ios::binary);
    if (!file) return false;

    struct RawUser {
        int id;
        char username[32];
        char password[32];
        int tipo;
        bool activo;
    } u;

    while (file.read(reinterpret_cast<char*>(&u), sizeof(u))) {
        if (uname == std::string(u.username) && u.activo) {
            id = u.id;
            username = u.username;
            password = u.password;
            tipo = u.tipo;
            activo = u.activo;
            return true;
        }
    }
    return false;
}

bool UserData::matchPassword(const std::string& pass) const {
    return pass == password;
}

int UserData::getId() const { return id; }
std::string UserData::getUsername() const { return username; }
int UserData::getType() const { return tipo; }
bool UserData::isActive() const { return activo; }
