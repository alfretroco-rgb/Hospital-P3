#include "operacionesCitas.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>
using namespace std;

void agendarCita() {
    cout << "=== AGENDAR CITA ===" << endl;

    int idPaciente, idDoctor;
    char fecha[11], hora[6], motivo[150];

    cout << "ID Paciente: ";
    cin >> idPaciente;
    cout << "ID Doctor: ";
    cin >> idDoctor;
    cin.ignore();
    cout << "Fecha (DD/MM/AAAA): ";
    cin.getline(fecha, 11);
    cout << "Hora (HH:MM): ";
    cin.getline(hora, 6);
    cout << "Motivo: ";
    cin.getline(motivo, 150);

    ArchivoHeader header = GestorArchivos::leerHeader("datos/citas.bin");
    int id = header.proximoID++;
    GestorArchivos::actualizarHeader("datos/citas.bin", header);

    Cita c(id, idPaciente, idDoctor, fecha, hora, motivo);

    if (c.validarDatos()) {
        if (GestorArchivos::agregarRegistro(c, "datos/citas.bin")) {
            cout << "Cita agendada." << endl;
            c.mostrarInformacionBasica();
        } else {
            cout << "Error al guardar." << endl;
        }
    } else {
        cout << "Datos invalidos." << endl;
    }
}

void buscarCitaPorID() {
    cout << "=== BUSCAR CITA POR ID ===" << endl;
    int id;
    cout << "ID: ";
    cin >> id;
    cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Cita>(id, "datos/citas.bin");
    if (indice != -1) {
        Cita c;
        GestorArchivos::leerRegistroPorIndice(indice, c, "datos/citas.bin");
        if (!c.isEliminado()) {
            c.mostrarInformacionCompleta();
            return;
        }
    }
    cout << "Cita no encontrada." << endl;
}

void listarCitasPendientes() {
    cout << "=== CITAS PENDIENTES ===" << endl;
    ArchivoHeader header = GestorArchivos::leerHeader("datos/citas.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Cita c;
        if (GestorArchivos::leerRegistroPorIndice(i, c, "datos/citas.bin") && !c.isEliminado() && !c.isAtendida() && strcmp(c.getEstado(), "Cancelada") != 0) {
            c.mostrarInformacionBasica();
        }
    }
}

void atenderCita() {
    cout << "=== ATENDER CITA ===" << endl;
    int id;
    cout << "ID Cita: ";
    cin >> id;
    cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Cita>(id, "datos/citas.bin");
    if (indice != -1) {
        Cita c;
        GestorArchivos::leerRegistroPorIndice(indice, c, "datos/citas.bin");
        if (!c.isEliminado() && !c.isAtendida()) {
            c.marcarComoAtendida();
            GestorArchivos::actualizarRegistro(id, c, "datos/citas.bin");
            cout << "Cita atendida." << endl;
        } else {
            cout << "Cita ya atendida o eliminada." << endl;
        }
    } else {
        cout << "Cita no encontrada." << endl;
    }
}

void cancelarCita() {
    cout << "=== CANCELAR CITA ===" << endl;
    int id;
    cout << "ID Cita: ";
    cin >> id;
    cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Cita>(id, "datos/citas.bin");
    if (indice != -1) {
        Cita c;
        GestorArchivos::leerRegistroPorIndice(indice, c, "datos/citas.bin");
        if (!c.isEliminado()) {
            c.cancelar();
            GestorArchivos::actualizarRegistro(id, c, "datos/citas.bin");
            cout << "Cita cancelada." << endl;
        }
    } else {
        cout << "Cita no encontrada." << endl;
    }
}