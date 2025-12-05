#include "operacionesHistorial.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>
using namespace std;

void agregarConsulta() {
    cout << "=== AGREGAR CONSULTA AL HISTORIAL ===" << endl;

    int idPaciente, idDoctor;
    char diagnostico[200], tratamiento[200];
    float costo;

    cout << "ID Paciente: ";
    cin >> idPaciente;
    cout << "ID Doctor: ";
    cin >> idDoctor;
    cin.ignore();
    cout << "Diagnostico: ";
    cin.getline(diagnostico, 200);
    cout << "Tratamiento: ";
    cin.getline(tratamiento, 200);
    cout << "Costo: ";
    cin >> costo;
    cin.ignore();

    ArchivoHeader header = GestorArchivos::leerHeader("datos/historiales.bin");
    int id = header.proximoID++;
    GestorArchivos::actualizarHeader("datos/historiales.bin", header);

    HistorialMedico h(id, idPaciente, diagnostico, tratamiento, idDoctor, costo);

    if (h.validarDatos()) {
        if (GestorArchivos::agregarRegistro(h, "datos/historiales.bin")) {
            cout << "Consulta agregada." << endl;
            h.mostrarInformacion();
        } else {
            cout << "Error." << endl;
        }
    } else {
        cout << "Datos invalidos." << endl;
    }
}

void mostrarHistorialPaciente() {
    cout << "=== HISTORIAL MEDICO DE PACIENTE ===" << endl;
    int idPaciente;
    cout << "ID Paciente: ";
    cin >> idPaciente;
    cin.ignore();

    ArchivoHeader header = GestorArchivos::leerHeader("datos/historiales.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        HistorialMedico h;
        if (GestorArchivos::leerRegistroPorIndice(i, h, "datos/historiales.bin") && !h.isEliminado() && h.getIdPaciente() == idPaciente) {
            h.mostrarInformacion();
            cout << "---" << endl;
        }
    }
}