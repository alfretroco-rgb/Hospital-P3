#include "operacionesPacientes.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>
using namespace std;

void registrarPaciente() {
    cout << "=== REGISTRAR NUEVO PACIENTE ===" << endl;

    char nombre[50], apellido[50], cedula[20];
    int edad;
    char sexo;

    cout << "Nombre: ";
    cin.getline(nombre, 50);
    cout << "Apellido: ";
    cin.getline(apellido, 50);
    cout << "Cedula: ";
    cin.getline(cedula, 20);
    cout << "Edad: ";
    cin >> edad;
    cout << "Sexo (M/F): ";
    cin >> sexo;
    cin.ignore();  // Limpiar buffer

    // Generar ID real
    ArchivoHeader header = GestorArchivos::leerHeader("datos/pacientes.bin");
    int id = header.proximoID++;
    GestorArchivos::actualizarHeader("datos/pacientes.bin", header);

    // Crear paciente
    Paciente p(id, nombre, apellido, cedula);
    p.setEdad(edad);
    p.setSexo(sexo);

    if (p.validarDatos()) {
        if (GestorArchivos::guardarPaciente(p)) {
            cout << "Paciente registrado exitosamente:" << endl;
            p.mostrarInformacionBasica();
        } else {
            cout << "Error al guardar en archivo." << endl;
        }
    } else {
        cout << "Error: Datos invalidos. Intente nuevamente." << endl;
    }
}

void buscarPacientePorID() {
    cout << "=== BUSCAR PACIENTE POR ID ===" << endl;
    int id;
    cout << "ID: ";
    cin >> id;
    cin.ignore();

    Paciente p;
    if (GestorArchivos::leerPacientePorID(id, p) && !p.isEliminado()) {
        p.mostrarInformacionCompleta();
    } else {
        cout << "Paciente no encontrado." << endl;
    }
}

void buscarPacientePorCedula() {
    cout << "=== BUSCAR PACIENTE POR CEDULA ===" << endl;
    char cedula[20];
    cout << "Cedula: ";
    cin.getline(cedula, 20);

    ArchivoHeader header = GestorArchivos::leerHeader("datos/pacientes.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (GestorArchivos::leerRegistroPorIndice(i, p, "datos/pacientes.bin") && !p.isEliminado() && strcmp(p.getCedula(), cedula) == 0) {
            p.mostrarInformacionCompleta();
            return;
        }
    }
    cout << "Paciente no encontrado." << endl;
}

void actualizarPaciente() {
    cout << "=== ACTUALIZAR PACIENTE ===" << endl;
    int id;
    cout << "ID del paciente a actualizar: ";
    cin >> id;
    cin.ignore();

    Paciente p;
    if (!GestorArchivos::leerPacientePorID(id, p) || p.isEliminado()) {
        cout << "Paciente no encontrado." << endl;
        return;
    }

    char nombre[50], apellido[50], cedula[20];
    int edad;
    char sexo;

    cout << "Nuevo nombre: ";
    cin.getline(nombre, 50);
    cout << "Nuevo apellido: ";
    cin.getline(apellido, 50);
    cout << "Nueva cedula: ";
    cin.getline(cedula, 20);
    cout << "Nueva edad: ";
    cin >> edad;
    cout << "Nuevo sexo (M/F): ";
    cin >> sexo;
    cin.ignore();

    p.setNombre(nombre);
    p.setApellido(apellido);
    p.setCedula(cedula);
    p.setEdad(edad);
    p.setSexo(sexo);

    if (p.validarDatos()) {
        if (GestorArchivos::actualizarRegistro<Paciente>(id, p, "datos/pacientes.bin")) {
            cout << "Paciente actualizado." << endl;
        } else {
            cout << "Error al actualizar." << endl;
        }
    } else {
        cout << "Datos invalidos." << endl;
    }
}

void eliminarPaciente() {
    cout << "=== ELIMINAR PACIENTE ===" << endl;
    int id;
    cout << "ID del paciente a eliminar: ";
    cin >> id;
    cin.ignore();

    if (GestorArchivos::eliminarRegistro<Paciente>(id, "datos/pacientes.bin")) {
        cout << "Paciente eliminado." << endl;
    } else {
        cout << "Error al eliminar o paciente no encontrado." << endl;
    }
}

void listarTodosPacientes() {
    cout << "=== LISTA DE PACIENTES ===" << endl;
    ArchivoHeader header = GestorArchivos::leerHeader("datos/pacientes.bin");
    cout << "ID\tNombre\tApellido\tCedula\tEdad" << endl;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (GestorArchivos::leerRegistroPorIndice(i, p, "datos/pacientes.bin") && !p.isEliminado()) {
            cout << p.getId() << "\t" << p.getNombre() << "\t" << p.getApellido() << "\t" << p.getCedula() << "\t" << p.getEdad() << endl;
        }
    }
}