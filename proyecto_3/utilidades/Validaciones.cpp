#include "Validaciones.hpp"
#include <cctype>
using namespace std;

bool Validaciones::validarCedula(const char* cedula) {
    if (!cedula || strlen(cedula) == 0 || strlen(cedula) > 19) return false;
    for (size_t i = 0; i < strlen(cedula); ++i) {
        if (!isdigit(cedula[i])) return false;
    }
    return true;
}

bool Validaciones::validarEmail(const char* email) {
    if (!email || strlen(email) == 0) return false;
    const char* at = strchr(email, '@');
    return at != nullptr && at != email && strchr(at + 1, '@') == nullptr;
}

bool Validaciones::validarTelefono(const char* telefono) {
    if (!telefono || strlen(telefono) == 0 || strlen(telefono) > 14) return false;
    for (size_t i = 0; i < strlen(telefono); ++i) {
        if (!isdigit(telefono[i]) && telefono[i] != '-') return false;
    }
    return true;
}

bool Validaciones::validarFecha(const char* fecha) {
    if (!fecha || strlen(fecha) != 10) return false;
    if (fecha[2] != '/' || fecha[5] != '/') return false;
    // Simple check: dígitos en posiciones correctas
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(fecha[i])) return false;
    }
    return true;
}

bool Validaciones::validarRango(int valor, int min, int max) {
    return valor >= min && valor <= max;
}