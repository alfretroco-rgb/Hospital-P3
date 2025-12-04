#include "operacionesCitas.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>

void agendarCita() {
    std::cout << "=== AGENDAR CITA ===" << std::endl;

    int idPaciente, idDoctor;
    char fecha[11], hora[6], motivo[150];

    std::cout << "ID Paciente: ";
    std::cin >> idPaciente;
    std::cout << "ID Doctor: ";
    std::cin >> idDoctor;
    std::cin.ignore();
    std::cout << "Fecha (DD/MM/AAAA): ";
    std::cin.getline(fecha, 11);
    std::cout << "Hora (HH:MM): ";
    std::cin.getline(hora, 6);
    std::cout << "Motivo: ";
    std::cin.getline(motivo, 150);

    ArchivoHeader header = GestorArchivos::leerHeader("datos/citas.bin");
    int id = header.proximoID++;
    GestorArchivos::actualizarHeader("datos/citas.bin", header);

    Cita c(id, idPaciente, idDoctor, fecha, hora, motivo);

    if (c.validarDatos()) {
        if (GestorArchivos::agregarRegistro(c, "datos/citas.bin")) {
            std::cout << "Cita agendada." << std::endl;
            c.mostrarInformacionBasica();
        } else {
            std::cout << "Error al guardar." << std::endl;
        }
    } else {
        std::cout << "Datos invalidos." << std::endl;
    }
}

void buscarCitaPorID() {
    std::cout << "=== BUSCAR CITA POR ID ===" << std::endl;
    int id;
    std::cout << "ID: ";
    std::cin >> id;
    std::cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Cita>(id, "datos/citas.bin");
    if (indice != -1) {
        Cita c;
        GestorArchivos::leerRegistroPorIndice(indice, c, "datos/citas.bin");
        if (!c.isEliminado()) {
            c.mostrarInformacionCompleta();
            return;
        }
    }
    std::cout << "Cita no encontrada." << std::endl;
}

void listarCitasPendientes() {
    std::cout << "=== CITAS PENDIENTES ===" << std::endl;
    ArchivoHeader header = GestorArchivos::leerHeader("datos/citas.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Cita c;
        if (GestorArchivos::leerRegistroPorIndice(i, c, "datos/citas.bin") && !c.isEliminado() && !c.isAtendida() && strcmp(c.getEstado(), "Cancelada") != 0) {
            c.mostrarInformacionBasica();
        }
    }
}

void atenderCita() {
    std::cout << "=== ATENDER CITA ===" << std::endl;
    int id;
    std::cout << "ID Cita: ";
    std::cin >> id;
    std::cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Cita>(id, "datos/citas.bin");
    if (indice != -1) {
        Cita c;
        GestorArchivos::leerRegistroPorIndice(indice, c, "datos/citas.bin");
        if (!c.isEliminado() && !c.isAtendida()) {
            c.marcarComoAtendida();
            GestorArchivos::actualizarRegistro(id, c, "datos/citas.bin");
            std::cout << "Cita atendida." << std::endl;
        } else {
            std::cout << "Cita ya atendida o eliminada." << std::endl;
        }
    } else {
        std::cout << "Cita no encontrada." << std::endl;
    }
}

void cancelarCita() {
    std::cout << "=== CANCELAR CITA ===" << std::endl;
    int id;
    std::cout << "ID Cita: ";
    std::cin >> id;
    std::cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Cita>(id, "datos/citas.bin");
    if (indice != -1) {
        Cita c;
        GestorArchivos::leerRegistroPorIndice(indice, c, "datos/citas.bin");
        if (!c.isEliminado()) {
            c.cancelar();
            GestorArchivos::actualizarRegistro(id, c, "datos/citas.bin");
            std::cout << "Cita cancelada." << std::endl;
        }
    } else {
        std::cout << "Cita no encontrada." << std::endl;
    }
}