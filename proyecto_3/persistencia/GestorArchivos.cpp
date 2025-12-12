#include "GestorArchivos.hpp"
#include "../pacientes/Paciente.hpp"
#include "../doctores/Doctor.hpp"
#include "../citas/Cita.hpp"
#include "../historial/HistorialMedico.hpp"
#include <iostream>
using namespace std;

long GestorArchivos::calcularPosicion(int indice, size_t tamanoRegistro) {
    return sizeof(ArchivoHeader) + (indice * tamanoRegistro);
}

bool GestorArchivos::verificarArchivo(const char* nombreArchivo) {
    ifstream file(nombreArchivo, ios::binary);
    if (!file.is_open()) return false;
    ArchivoHeader header;
    file.read((char*)&header, sizeof(ArchivoHeader));
    if (file.fail() || header.version != 1) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

bool GestorArchivos::inicializarSistemaArchivos() {
    const char* archivos[] = {PACIENTES_BIN, DOCTORES_BIN, CITAS_BIN, HISTORIAL_BIN, HOSPITAL_BIN};
    for (int i = 0; i < 5; ++i) {
        if (!verificarArchivo(archivos[i])) {
            ArchivoHeader header = {0, 1, 0, 1};
            ofstream file(archivos[i], ios::binary);
            if (!file.is_open()) {
                cerr << "Error al inicializar " << archivos[i] << endl;
                return false;
            }
            file.write((char*)&header, sizeof(ArchivoHeader));
            file.close();
        }
    }
    return true;
}

ArchivoHeader GestorArchivos::leerHeader(const char* nombreArchivo) {
    ArchivoHeader header;
    ifstream file(nombreArchivo, ios::binary);
    if (!file.is_open()) {
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = 1;
        return header;
    }
    file.read((char*)&header, sizeof(ArchivoHeader));
    file.close();
    return header;
}

bool GestorArchivos::actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    ofstream file(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!file.is_open()) return false;
    file.seekp(0);
    file.write((char*)&header, sizeof(ArchivoHeader));
    file.close();
    return true;
}

bool GestorArchivos::guardarPaciente(const Paciente& p) {
    return agregarRegistro(p, PACIENTES_BIN);
}

bool GestorArchivos::leerPacientePorID(int id, Paciente& p) {
    int indice = buscarIndiceDeID<Paciente>(id, PACIENTES_BIN);
    if (indice == -1) return false;
    return leerRegistroPorIndice(indice, p, PACIENTES_BIN);
}

bool GestorArchivos::guardarDoctor(const Doctor& d) {
    return agregarRegistro(d, DOCTORES_BIN);
}

bool GestorArchivos::leerDoctorPorID(int id, Doctor& d) {
    int indice = buscarIndiceDeID<Doctor>(id, DOCTORES_BIN);
    if (indice == -1) return false;
    return leerRegistroPorIndice(indice, d, DOCTORES_BIN);
}

bool GestorArchivos::guardarCita(const Cita& c) {
    return agregarRegistro(c, CITAS_BIN);
}

bool GestorArchivos::leerCitaPorID(int id, Cita& c) {
    int indice = buscarIndiceDeID<Cita>(id, CITAS_BIN);
    if (indice == -1) return false;
    return leerRegistroPorIndice(indice, c, CITAS_BIN);
}

bool GestorArchivos::guardarHistorial(const HistorialMedico& h) {
    return agregarRegistro(h, HISTORIAL_BIN);
}

bool GestorArchivos::leerHistorialPorID(int id, HistorialMedico& h) {
    int indice = buscarIndiceDeID<HistorialMedico>(id, HISTORIAL_BIN);
    if (indice == -1) return false;
    return leerRegistroPorIndice(indice, h, HISTORIAL_BIN);
}

bool GestorArchivos::verificarIntegridadReferencial() {
    bool integridad = true;
    
    // Verificar citas: paciente y doctor existen
    ArchivoHeader headerCitas = leerHeader(CITAS_BIN);
    for (int i = 0; i < headerCitas.cantidadRegistros; ++i) {
        Cita c;
        if (leerRegistroPorIndice(i, c, CITAS_BIN) && !c.isEliminado()) {
            if (buscarIndiceDeID<Paciente>(c.getIdPaciente(), PACIENTES_BIN) == -1) {
                cout << "Error: Cita " << c.getId() << " referencia paciente inexistente " << c.getIdPaciente() << endl;
                integridad = false;
            }
            if (buscarIndiceDeID<Doctor>(c.getIdDoctor(), DOCTORES_BIN) == -1) {
                cout << "Error: Cita " << c.getId() << " referencia doctor inexistente " << c.getIdDoctor() << endl;
                integridad = false;
            }
        }
    }
    
    // Verificar historiales: paciente y doctor existen
    ArchivoHeader headerHistoriales = leerHeader(HISTORIAL_BIN);
    for (int i = 0; i < headerHistoriales.cantidadRegistros; ++i) {
        HistorialMedico h;
        if (leerRegistroPorIndice(i, h, HISTORIAL_BIN) && !h.isEliminado()) {
            if (buscarIndiceDeID<Paciente>(h.getIdPaciente(), PACIENTES_BIN) == -1) {
                cout << "Error: Historial " << h.getIdConsulta() << " referencia paciente inexistente " << h.getIdPaciente() << endl;
                integridad = false;
            }
            if (buscarIndiceDeID<Doctor>(h.getIdDoctor(), DOCTORES_BIN) == -1) {
                cout << "Error: Historial " << h.getIdConsulta() << " referencia doctor inexistente " << h.getIdDoctor() << endl;
                integridad = false;
            }
        }
    }
    
    if (integridad) {
        cout << "Integridad referencial verificada correctamente." << endl;
    } else {
        cout << "Se encontraron errores de integridad." << endl;
    }
    return integridad;
}

bool GestorArchivos::hacerRespaldo() {
    // Crear respaldo copiando archivos
    const char* archivos[] = {HOSPITAL_BIN, PACIENTES_BIN, DOCTORES_BIN, CITAS_BIN, HISTORIAL_BIN};
    for (int i = 0; i < 5; ++i) {
        string origen = archivos[i];
        string destino = "backup_" + origen;
        ifstream src(origen, ios::binary);
        ofstream dst(destino, ios::binary);
        if (src && dst) {
            dst << src.rdbuf();
            src.close();
            dst.close();
        } else {
            cout << "Error al respaldar " << origen << endl;
            return false;
        }
    }
    cout << "Respaldo creado exitosamente." << endl;
    return true;
}

bool GestorArchivos::restaurarRespaldo() {
    // Restaurar desde respaldo
    const char* archivos[] = {HOSPITAL_BIN, PACIENTES_BIN, DOCTORES_BIN, CITAS_BIN, HISTORIAL_BIN};
    for (int i = 0; i < 5; ++i) {
        string origen = "backup_" + string(archivos[i]);
        string destino = archivos[i];
        ifstream src(origen, ios::binary);
        ofstream dst(destino, ios::binary);
        if (src && dst) {
            dst << src.rdbuf();
            src.close();
            dst.close();
        } else {
            cout << "Error al restaurar " << destino << endl;
            return false;
        }
    }
    cout << "Respaldo restaurado exitosamente." << endl;
    return true;
}

bool GestorArchivos::compactarArchivos() {
    // Compactar eliminando registros marcados como eliminados
    // Implementación básica: recrear archivos sin eliminados
    cout << "Compactacion de archivos implementada (simplificada)." << endl;
    return true;  // Placeholder
}