#include "pacientes/operacionesPacientes.hpp"
#include "persistencia/GestorArchivos.hpp"
#include <iostream>

int main() {
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        std::cerr << "Error al inicializar archivos" << std::endl;
        return 1;
    }

    int opcion;
    do {
        std::cout << "\n=== GESTION DE PACIENTES ===" << std::endl;
        std::cout << "1. Registrar paciente" << std::endl;
        std::cout << "2. Buscar por ID" << std::endl;
        std::cout << "3. Buscar por cedula" << std::endl;
        std::cout << "4. Actualizar paciente" << std::endl;
        std::cout << "5. Eliminar paciente" << std::endl;
        std::cout << "6. Listar todos" << std::endl;
        std::cout << "7. Salir" << std::endl;
        std::cout << "Opcion: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {
            case 1: registrarPaciente(); break;
            case 2: buscarPacientePorID(); break;
            case 3: buscarPacientePorCedula(); break;
            case 4: actualizarPaciente(); break;
            case 5: eliminarPaciente(); break;
            case 6: listarTodosPacientes(); break;
            case 7: break;
            default: std::cout << "Opcion invalida." << std::endl;
        }
    } while (opcion != 7);

    return 0;
}