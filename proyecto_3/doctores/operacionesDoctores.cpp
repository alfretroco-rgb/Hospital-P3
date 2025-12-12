#include "operacionesDoctores.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>
using namespace std;

void registrarDoctor() {
    cout << "=== REGISTRAR NUEVO DOCTOR ===" << endl;

    char nombre[50], apellido[50], cedula[20], especialidad[50];
    int anios;
    float costo;

    cout << "Nombre: ";
    cin.getline(nombre, 50);
    cout << "Apellido: ";
    cin.getline(apellido, 50);
    cout << "Cedula: ";
    cin.getline(cedula, 20);
    cout << "Especialidad: ";
    cin.getline(especialidad, 50);
    cout << "Anios de experiencia: ";
    cin >> anios;
    cout << "Costo de consulta: ";
    cin >> costo;
    cin.ignore();

    ArchivoHeader header = GestorArchivos::leerHeader("datos/doctores.bin");
    int id = header.proximoID++;
    GestorArchivos::actualizarHeader("datos/doctores.bin", header);

    Doctor d(id, nombre, apellido, cedula, especialidad, anios, costo);

    if (d.validarDatos()) {
        if (GestorArchivos::agregarRegistro(d, "datos/doctores.bin")) {
            cout << "Doctor registrado exitosamente." << endl;
            d.mostrarInformacionBasica();
        } else {
            cout << "Error al guardar." << endl;
        }
    } else {
        cout << "Datos invalidos." << endl;
    }
}

void buscarDoctorPorID() {
    cout << "=== BUSCAR DOCTOR POR ID ===" << endl;
    int id;
    cout << "ID: ";
    cin >> id;
    cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Doctor>(id, "datos/doctores.bin");
    if (indice != -1) {
        Doctor d;
        GestorArchivos::leerRegistroPorIndice(indice, d, "datos/doctores.bin");
        if (!d.isEliminado()) {
            d.mostrarInformacionCompleta();
            return;
        }
    }
    cout << "Doctor no encontrado." << endl;
}

void listarDoctores() {
    cout << "=== LISTA DE DOCTORES ===" << endl;
    ArchivoHeader header = GestorArchivos::leerHeader("datos/doctores.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d;
        if (GestorArchivos::leerRegistroPorIndice(i, d, "datos/doctores.bin") && !d.isEliminado()) {
            d.mostrarInformacionBasica();
        }
    }
}

void listarDoctoresPorEspecialidad() {
    cout << "=== LISTA DE DOCTORES POR ESPECIALIDAD ===" << endl;
    char especialidad[50];
    cout << "Especialidad: ";
    cin.getline(especialidad, 50);

    ArchivoHeader header = GestorArchivos::leerHeader("datos/doctores.bin");
    bool encontrado = false;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d;
        if (GestorArchivos::leerRegistroPorIndice(i, d, "datos/doctores.bin") && !d.isEliminado()) {
            if (strcmp(d.getEspecialidad(), especialidad) == 0) {
                d.mostrarInformacionBasica();
                encontrado = true;
            }
        }
    }
    if (!encontrado) {
        cout << "No se encontraron doctores con esa especialidad." << endl;
    }
}

void actualizarDoctor() {
    cout << "=== ACTUALIZAR DOCTOR ===" << endl;
    int id;
    cout << "ID del doctor: ";
    cin >> id;
    cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Doctor>(id, "datos/doctores.bin");
    if (indice == -1) {
        cout << "Doctor no encontrado." << endl;
        return;
    }

    Doctor d;
    GestorArchivos::leerRegistroPorIndice(indice, d, "datos/doctores.bin");
    if (d.isEliminado()) {
        cout << "Doctor eliminado." << endl;
        return;
    }

    char nombre[50], apellido[50], especialidad[50];
    int anios;
    float costo;

    cout << "Nuevo nombre: ";
    cin.getline(nombre, 50);
    cout << "Nuevo apellido: ";
    cin.getline(apellido, 50);
    cout << "Nueva especialidad: ";
    cin.getline(especialidad, 50);
    cout << "Nuevos anios: ";
    cin >> anios;
    cout << "Nuevo costo: ";
    cin >> costo;
    cin.ignore();

    d.setNombre(nombre);
    d.setApellido(apellido);
    d.setEspecialidad(especialidad);
    d.setAniosExperiencia(anios);
    d.setCostoConsulta(costo);

    if (d.validarDatos()) {
        GestorArchivos::actualizarRegistro(id, d, "datos/doctores.bin");
        cout << "Doctor actualizado." << endl;
    } else {
        cout << "Datos invalidos." << endl;
    }
}

void eliminarDoctor() {
    cout << "=== ELIMINAR DOCTOR ===" << endl;
    int id;
    cout << "ID del doctor: ";
    cin >> id;
    cin.ignore();

    if (GestorArchivos::eliminarRegistro<Doctor>(id, "datos/doctores.bin")) {
        cout << "Doctor eliminado." << endl;
    } else {
        cout << "Error." << endl;
    }
}