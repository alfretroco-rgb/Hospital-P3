#include "operacionesPacientes.hpp"
#include "../persistencia/GestorArchivos.hpp"
#include <iostream>
#include <cstring>

void registrarPaciente() {
    std::cout << "=== REGISTRAR NUEVO PACIENTE ===" << std::endl;

    char nombre[50], apellido[50], cedula[20];
    int edad;
    char sexo;

    std::cout << "Nombre: ";
    std::cin.getline(nombre, 50);
    std::cout << "Apellido: ";
    std::cin.getline(apellido, 50);
    std::cout << "Cedula: ";
    std::cin.getline(cedula, 20);
    std::cout << "Edad: ";
    std::cin >> edad;
    std::cout << "Sexo (M/F): ";
    std::cin >> sexo;
    std::cin.ignore();  // Limpiar buffer

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
            std::cout << "Paciente registrado exitosamente:" << std::endl;
            p.mostrarInformacionBasica();
        } else {
            std::cout << "Error al guardar en archivo." << std::endl;
        }
    } else {
        std::cout << "Error: Datos invalidos. Intente nuevamente." << std::endl;
    }
}

void buscarPacientePorID() {
    std::cout << "=== BUSCAR PACIENTE POR ID ===" << std::endl;
    int id;
    std::cout << "ID: ";
    std::cin >> id;
    std::cin.ignore();

    Paciente p;
    if (GestorArchivos::leerPacientePorID(id, p) && !p.isEliminado()) {
        p.mostrarInformacionCompleta();
    } else {
        std::cout << "Paciente no encontrado." << std::endl;
    }
}

void buscarPacientePorCedula() {
    std::cout << "=== BUSCAR PACIENTE POR CEDULA ===" << std::endl;
    char cedula[20];
    std::cout << "Cedula: ";
    std::cin.getline(cedula, 20);

    ArchivoHeader header = GestorArchivos::leerHeader("datos/pacientes.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (GestorArchivos::leerRegistroPorIndice(i, p, "datos/pacientes.bin") && !p.isEliminado() && strcmp(p.getCedula(), cedula) == 0) {
            p.mostrarInformacionCompleta();
            return;
        }
    }
    std::cout << "Paciente no encontrado." << std::endl;
}

void actualizarPaciente() {
    std::cout << "=== ACTUALIZAR PACIENTE ===" << std::endl;
    int id;
    std::cout << "ID del paciente a actualizar: ";
    std::cin >> id;
    std::cin.ignore();

    Paciente p;
    if (!GestorArchivos::leerPacientePorID(id, p) || p.isEliminado()) {
        std::cout << "Paciente no encontrado." << std::endl;
        return;
    }

    char nombre[50], apellido[50], cedula[20];
    int edad;
    char sexo;

    std::cout << "Nuevo nombre: ";
    std::cin.getline(nombre, 50);
    std::cout << "Nuevo apellido: ";
    std::cin.getline(apellido, 50);
    std::cout << "Nueva cedula: ";
    std::cin.getline(cedula, 20);
    std::cout << "Nueva edad: ";
    std::cin >> edad;
    std::cout << "Nuevo sexo (M/F): ";
    std::cin >> sexo;
    std::cin.ignore();

    p.setNombre(nombre);
    p.setApellido(apellido);
    p.setCedula(cedula);
    p.setEdad(edad);
    p.setSexo(sexo);

    if (p.validarDatos()) {
        if (GestorArchivos::actualizarRegistro<Paciente>(id, p, "datos/pacientes.bin")) {
            std::cout << "Paciente actualizado." << std::endl;
        } else {
            std::cout << "Error al actualizar." << std::endl;
        }
    } else {
        std::cout << "Datos invalidos." << std::endl;
    }
}

void eliminarPaciente() {
    std::cout << "=== ELIMINAR PACIENTE ===" << std::endl;
    int id;
    std::cout << "ID del paciente a eliminar: ";
    std::cin >> id;
    std::cin.ignore();

    if (GestorArchivos::eliminarRegistro<Paciente>(id, "datos/pacientes.bin")) {
        std::cout << "Paciente eliminado." << std::endl;
    } else {
        std::cout << "Error al eliminar o paciente no encontrado." << std::endl;
    }
}

void listarTodosPacientes() {
    std::cout << "=== LISTA DE PACIENTES ===" << std::endl;
    ArchivoHeader header = GestorArchivos::leerHeader("datos/pacientes.bin");
    std::cout << "ID\tNombre\tApellido\tCedula\tEdad" << std::endl;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (GestorArchivos::leerRegistroPorIndice(i, p, "datos/pacientes.bin") && !p.isEliminado()) {
            std::cout << p.getId() << "\t" << p.getNombre() << "\t" << p.getApellido() << "\t" << p.getCedula() << "\t" << p.getEdad() << std::endl;
        }
    }
}