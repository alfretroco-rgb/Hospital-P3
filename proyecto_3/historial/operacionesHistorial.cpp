#include "operacionesHistorial.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>

void agregarConsulta() {
    std::cout << "=== AGREGAR CONSULTA AL HISTORIAL ===" << std::endl;

    int idPaciente, idDoctor;
    char diagnostico[200], tratamiento[200];
    float costo;

    std::cout << "ID Paciente: ";
    std::cin >> idPaciente;
    std::cout << "ID Doctor: ";
    std::cin >> idDoctor;
    std::cin.ignore();
    std::cout << "Diagnostico: ";
    std::cin.getline(diagnostico, 200);
    std::cout << "Tratamiento: ";
    std::cin.getline(tratamiento, 200);
    std::cout << "Costo: ";
    std::cin >> costo;
    std::cin.ignore();

    ArchivoHeader header = GestorArchivos::leerHeader("datos/historiales.bin");
    int id = header.proximoID++;
    GestorArchivos::actualizarHeader("datos/historiales.bin", header);

    HistorialMedico h(id, idPaciente, diagnostico, tratamiento, idDoctor, costo);

    if (h.validarDatos()) {
        if (GestorArchivos::agregarRegistro(h, "datos/historiales.bin")) {
            std::cout << "Consulta agregada." << std::endl;
            h.mostrarInformacion();
        } else {
            std::cout << "Error." << std::endl;
        }
    } else {
        std::cout << "Datos invalidos." << std::endl;
    }
}

void mostrarHistorialPaciente() {
    std::cout << "=== HISTORIAL MEDICO DE PACIENTE ===" << std::endl;
    int idPaciente;
    std::cout << "ID Paciente: ";
    std::cin >> idPaciente;
    std::cin.ignore();

    ArchivoHeader header = GestorArchivos::leerHeader("datos/historiales.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        HistorialMedico h;
        if (GestorArchivos::leerRegistroPorIndice(i, h, "datos/historiales.bin") && !h.isEliminado() && h.getIdPaciente() == idPaciente) {
            h.mostrarInformacion();
            std::cout << "---" << std::endl;
        }
    }
}