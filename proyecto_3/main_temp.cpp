#include "pacientes/operacionesPacientes.hpp"
#include "persistencia/GestorArchivos.hpp"
#include <iostream>

int main() {
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        std::cerr << "Error al inicializar archivos" << std::endl;
        return 1;
    }
    registrarPaciente();
    return 0;
}