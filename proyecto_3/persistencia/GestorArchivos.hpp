#ifndef GESTOR_ARCHIVOS_HPP
#define GESTOR_ARCHIVOS_HPP

#include <fstream>
#include <iostream>
#include <cstring>
#include "Constantes.hpp"
using namespace std;

// Estructura de header 
struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};

class GestorArchivos {
private:
    // Métodos auxiliares privados
    static long calcularPosicion(int indice, size_t tamanoRegistro);
    static bool verificarArchivo(const char* nombreArchivo);

public:
    // Inicialización
    static bool inicializarSistemaArchivos();

    // Operaciones de header
    static ArchivoHeader leerHeader(const char* nombreArchivo);
    static bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);

    // Operaciones genéricas con templates
    template<typename T>
    static bool agregarRegistro(const T& registro, const char* archivo) {
        ArchivoHeader header = leerHeader(archivo);
        ofstream file(archivo, ios::binary | ios::app);
        if (!file.is_open()) return false;
        file.write((char*)&registro, sizeof(T));
        header.cantidadRegistros++;
        header.registrosActivos++;
        actualizarHeader(archivo, header);
        file.close();
        return true;
    }

    template<typename T>
    static bool leerRegistroPorIndice(int indice, T& registro, const char* archivo) {
        ifstream file(archivo, ios::binary);
        if (!file.is_open()) return false;
        long posicion = calcularPosicion(indice, sizeof(T));
        file.seekg(posicion);
        file.read((char*)&registro, sizeof(T));
        file.close();
        return file.good();
    }

    template<typename T>
    static int buscarIndiceDeID(int id, const char* archivo) {
        ArchivoHeader header = leerHeader(archivo);
        ifstream file(archivo, ios::binary);
        if (!file.is_open()) return -1;

        for (int i = 0; i < header.cantidadRegistros; ++i) {
            long posicion = calcularPosicion(i, sizeof(T));
            file.seekg(posicion);
            int registroID;
            file.read((char*)&registroID, sizeof(int));
            if (registroID == id) {
                file.close();
                return i;
            }
        }
        file.close();
        return -1;
    }

    template<typename T>
    static bool actualizarRegistro(int id, const T& registro, const char* archivo) {
        int indice = buscarIndiceDeID<T>(id, archivo);
        if (indice == -1) return false;

        ofstream file(archivo, ios::binary | ios::in | ios::out);
        if (!file.is_open()) return false;
        long posicion = calcularPosicion(indice, sizeof(T));
        file.seekp(posicion);
        file.write((char*)&registro, sizeof(T));
        file.close();
        return true;
    }

    template<typename T>
    static bool eliminarRegistro(int id, const char* archivo) {
        int indice = buscarIndiceDeID<T>(id, archivo);
        if (indice == -1) return false;

        // Leer el registro
        T buffer;
        if (!leerRegistroPorIndice<T>(indice, buffer, archivo)) return false;

        // Marcar como eliminado (asumiendo bool eliminado al final)
        char* ptr = (char*)&buffer;
        ptr[sizeof(T) - 1] = 1;  // Último byte es bool

        // Actualizar
        ofstream file(archivo, ios::binary | ios::in | ios::out);
        if (!file.is_open()) return false;
        long posicion = calcularPosicion(indice, sizeof(T));
        file.seekp(posicion);
        file.write((char*)&buffer, sizeof(T));
        file.close();

        // Actualizar header
        ArchivoHeader header = leerHeader(archivo);
        header.registrosActivos--;
        actualizarHeader(archivo, header);

        return true;
    }

    // Métodos específicos para entidades
    static bool guardarPaciente(const class Paciente& p);
    static bool leerPacientePorID(int id, class Paciente& p);
    static bool guardarDoctor(const class Doctor& d);
    static bool leerDoctorPorID(int id, class Doctor& d);
    static bool guardarCita(const class Cita& c);
    static bool leerCitaPorID(int id, class Cita& c);
    static bool guardarHistorial(const class HistorialMedico& h);
    static bool leerHistorialPorID(int id, class HistorialMedico& h);
    
    // Funciones de utilidades del sistema
    static bool verificarIntegridadReferencial();
    static bool hacerRespaldo();
    static bool restaurarRespaldo();
    static bool compactarArchivos();
};

#endif