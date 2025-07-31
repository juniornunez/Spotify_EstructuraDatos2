#include "userdata.h"
#include <fstream>
#include <cstring>

// Tamaños para los campos (para mantener los structs en binario)
const int USERNAME_SIZE = 32;
const int PASSWORD_SIZE = 32;

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
        char username[USERNAME_SIZE];
        char password[PASSWORD_SIZE];
        int tipo;
        bool activo;
    } u;

    while (file.read(reinterpret_cast<char*>(&u), sizeof(u))) {
        std::string currentUsername(u.username, strnlen(u.username, USERNAME_SIZE));
        if (uname == currentUsername && u.activo) {
            id = u.id;
            username = currentUsername;
            password = std::string(u.password, strnlen(u.password, PASSWORD_SIZE));
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

// --- NUEVO: guardar usuario en .dat ---
bool UserData::saveToFile(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    if (!file) return false;

    struct RawUser {
        int id;
        char username[USERNAME_SIZE];
        char password[PASSWORD_SIZE];
        int tipo;
        bool activo;
    } u;

    u.id = id;
    std::memset(u.username, 0, USERNAME_SIZE);
    std::memset(u.password, 0, PASSWORD_SIZE);
    std::strncpy(u.username, username.c_str(), USERNAME_SIZE - 1);
    std::strncpy(u.password, password.c_str(), PASSWORD_SIZE - 1);
    u.tipo = tipo;
    u.activo = activo;

    file.write(reinterpret_cast<char*>(&u), sizeof(u));
    return true;
}

