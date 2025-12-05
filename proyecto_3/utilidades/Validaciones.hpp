#ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP

#include <cstring>
#include <iostream>
using namespace std;

class Validaciones {
public:
    static bool validarCedula(const char* cedula);
    static bool validarEmail(const char* email);
    static bool validarTelefono(const char* telefono);
    static bool validarFecha(const char* fecha);
    static bool validarRango(int valor, int min, int max);
};

#endif