#include "operacionesDoctores.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>

void registrarDoctor() {
    std::cout << "=== REGISTRAR NUEVO DOCTOR ===" << std::endl;

    char nombre[50], apellido[50], cedula[20], especialidad[50];
    int anios;
    float costo;

    std::cout << "Nombre: ";
    std::cin.getline(nombre, 50);
    std::cout << "Apellido: ";
    std::cin.getline(apellido, 50);
    std::cout << "Cedula: ";
    std::cin.getline(cedula, 20);
    std::cout << "Especialidad: ";
    std::cin.getline(especialidad, 50);
    std::cout << "Anios de experiencia: ";
    std::cin >> anios;
    std::cout << "Costo de consulta: ";
    std::cin >> costo;
    std::cin.ignore();

    ArchivoHeader header = GestorArchivos::leerHeader("datos/doctores.bin");
    int id = header.proximoID++;
    GestorArchivos::actualizarHeader("datos/doctores.bin", header);

    Doctor d(id, nombre, apellido, cedula, especialidad, anios, costo);

    if (d.validarDatos()) {
        if (GestorArchivos::agregarRegistro(d, "datos/doctores.bin")) {
            std::cout << "Doctor registrado exitosamente." << std::endl;
            d.mostrarInformacionBasica();
        } else {
            std::cout << "Error al guardar." << std::endl;
        }
    } else {
        std::cout << "Datos invalidos." << std::endl;
    }
}

void buscarDoctorPorID() {
    std::cout << "=== BUSCAR DOCTOR POR ID ===" << std::endl;
    int id;
    std::cout << "ID: ";
    std::cin >> id;
    std::cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Doctor>(id, "datos/doctores.bin");
    if (indice != -1) {
        Doctor d;
        GestorArchivos::leerRegistroPorIndice(indice, d, "datos/doctores.bin");
        if (!d.isEliminado()) {
            d.mostrarInformacionCompleta();
            return;
        }
    }
    std::cout << "Doctor no encontrado." << std::endl;
}

void listarDoctores() {
    std::cout << "=== LISTA DE DOCTORES ===" << std::endl;
    ArchivoHeader header = GestorArchivos::leerHeader("datos/doctores.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d;
        if (GestorArchivos::leerRegistroPorIndice(i, d, "datos/doctores.bin") && !d.isEliminado()) {
            d.mostrarInformacionBasica();
        }
    }
}

void actualizarDoctor() {
    std::cout << "=== ACTUALIZAR DOCTOR ===" << std::endl;
    int id;
    std::cout << "ID del doctor: ";
    std::cin >> id;
    std::cin.ignore();

    int indice = GestorArchivos::buscarIndiceDeID<Doctor>(id, "datos/doctores.bin");
    if (indice == -1) {
        std::cout << "Doctor no encontrado." << std::endl;
        return;
    }

    Doctor d;
    GestorArchivos::leerRegistroPorIndice(indice, d, "datos/doctores.bin");
    if (d.isEliminado()) {
        std::cout << "Doctor eliminado." << std::endl;
        return;
    }

    char nombre[50], apellido[50], especialidad[50];
    int anios;
    float costo;

    std::cout << "Nuevo nombre: ";
    std::cin.getline(nombre, 50);
    std::cout << "Nuevo apellido: ";
    std::cin.getline(apellido, 50);
    std::cout << "Nueva especialidad: ";
    std::cin.getline(especialidad, 50);
    std::cout << "Nuevos anios: ";
    std::cin >> anios;
    std::cout << "Nuevo costo: ";
    std::cin >> costo;
    std::cin.ignore();

    d.setNombre(nombre);
    d.setApellido(apellido);
    d.setEspecialidad(especialidad);
    d.setAniosExperiencia(anios);
    d.setCostoConsulta(costo);

    if (d.validarDatos()) {
        GestorArchivos::actualizarRegistro(id, d, "datos/doctores.bin");
        std::cout << "Doctor actualizado." << std::endl;
    } else {
        std::cout << "Datos invalidos." << std::endl;
    }
}

void eliminarDoctor() {
    std::cout << "=== ELIMINAR DOCTOR ===" << std::endl;
    int id;
    std::cout << "ID del doctor: ";
    std::cin >> id;
    std::cin.ignore();

    if (GestorArchivos::eliminarRegistro<Doctor>(id, "datos/doctores.bin")) {
        std::cout << "Doctor eliminado." << std::endl;
    } else {
        std::cout << "Error." << std::endl;
    }
}