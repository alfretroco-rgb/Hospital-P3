#include "Doctor.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Doctor::Doctor() {
    id = 0;
    strcpy(nombre, "");
    strcpy(apellido, "");
    strcpy(cedula, "");
    strcpy(especialidad, "");
    aniosExperiencia = 0;
    costoConsulta = 0.0f;
    strcpy(horarioAtencion, "Lun-Vie 8:00-16:00");
    strcpy(telefono, "");
    strcpy(email, "");
    cantidadPacientes = 0;
    cantidadCitas = 0;
    disponible = true;
    eliminado = false;
    fechaModificacion = time(NULL);
}

Doctor::Doctor(int id, const char* nombre, const char* apellido, const char* cedula, const char* especialidad, int aniosExperiencia, float costoConsulta) {
    this->id = id;
    strncpy(this->nombre, nombre, 49);
    this->nombre[49] = '\0';
    strncpy(this->apellido, apellido, 49);
    this->apellido[49] = '\0';
    strncpy(this->cedula, cedula, 19);
    this->cedula[19] = '\0';
    strncpy(this->especialidad, especialidad, 49);
    this->especialidad[49] = '\0';
    this->aniosExperiencia = aniosExperiencia;
    this->costoConsulta = costoConsulta;
    strcpy(horarioAtencion, "Lun-Vie 8:00-16:00");
    strcpy(telefono, "");
    strcpy(email, "");
    cantidadPacientes = 0;
    cantidadCitas = 0;
    disponible = true;
    eliminado = false;
    fechaModificacion = time(NULL);
}

Doctor::Doctor(const Doctor& other) {
    id = other.id;
    strcpy(nombre, other.nombre);
    strcpy(apellido, other.apellido);
    strcpy(cedula, other.cedula);
    strcpy(especialidad, other.especialidad);
    aniosExperiencia = other.aniosExperiencia;
    costoConsulta = other.costoConsulta;
    strcpy(horarioAtencion, other.horarioAtencion);
    strcpy(telefono, other.telefono);
    strcpy(email, other.email);
    cantidadPacientes = other.cantidadPacientes;
    for (int i = 0; i < cantidadPacientes; ++i) {
        pacientesIDs[i] = other.pacientesIDs[i];
    }
    cantidadCitas = other.cantidadCitas;
    for (int i = 0; i < cantidadCitas; ++i) {
        citasIDs[i] = other.citasIDs[i];
    }
    disponible = other.disponible;
    eliminado = other.eliminado;
    fechaModificacion = other.fechaModificacion;
}

Doctor::~Doctor() {
    // No hay punteros dinámicos
}

int Doctor::getId() const { return id; }
const char* Doctor::getNombre() const { return nombre; }
const char* Doctor::getApellido() const { return apellido; }
const char* Doctor::getCedula() const { return cedula; }
const char* Doctor::getEspecialidad() const { return especialidad; }
int Doctor::getAniosExperiencia() const { return aniosExperiencia; }
float Doctor::getCostoConsulta() const { return costoConsulta; }
const char* Doctor::getHorarioAtencion() const { return horarioAtencion; }
const char* Doctor::getTelefono() const { return telefono; }
const char* Doctor::getEmail() const { return email; }
const int* Doctor::getPacientesIDs() const { return pacientesIDs; }
int Doctor::getCantidadPacientes() const { return cantidadPacientes; }
const int* Doctor::getCitasIDs() const { return citasIDs; }
int Doctor::getCantidadCitas() const { return cantidadCitas; }
bool Doctor::isDisponible() const { return disponible; }
bool Doctor::isEliminado() const { return eliminado; }
time_t Doctor::getFechaModificacion() const { return fechaModificacion; }

void Doctor::setId(int id) { this->id = id; fechaModificacion = time(NULL); }
void Doctor::setNombre(const char* nombre) { strncpy(this->nombre, nombre, 49); this->nombre[49] = '\0'; fechaModificacion = time(NULL); }
void Doctor::setApellido(const char* apellido) { strncpy(this->apellido, apellido, 49); this->apellido[49] = '\0'; fechaModificacion = time(NULL); }
void Doctor::setCedula(const char* cedula) { if (Validaciones::validarCedula(cedula)) { strncpy(this->cedula, cedula, 19); this->cedula[19] = '\0'; fechaModificacion = time(NULL); } }
void Doctor::setEspecialidad(const char* especialidad) { strncpy(this->especialidad, especialidad, 49); this->especialidad[49] = '\0'; fechaModificacion = time(NULL); }
void Doctor::setAniosExperiencia(int anios) { if (anios >= 0) { aniosExperiencia = anios; fechaModificacion = time(NULL); } }
void Doctor::setCostoConsulta(float costo) { if (costo >= 0) { costoConsulta = costo; fechaModificacion = time(NULL); } }
void Doctor::setHorarioAtencion(const char* horario) { strncpy(horarioAtencion, horario, 49); horarioAtencion[49] = '\0'; fechaModificacion = time(NULL); }
void Doctor::setTelefono(const char* telefono) { if (Validaciones::validarTelefono(telefono)) { strncpy(this->telefono, telefono, 14); this->telefono[14] = '\0'; fechaModificacion = time(NULL); } }
void Doctor::setEmail(const char* email) { if (Validaciones::validarEmail(email)) { strncpy(this->email, email, 49); this->email[49] = '\0'; fechaModificacion = time(NULL); } }
void Doctor::setDisponible(bool disponible) { this->disponible = disponible; fechaModificacion = time(NULL); }
void Doctor::setEliminado(bool eliminado) { this->eliminado = eliminado; fechaModificacion = time(NULL); }
void Doctor::setFechaModificacion(time_t fecha) { fechaModificacion = fecha; }

bool Doctor::validarDatos() const {
    return strlen(nombre) > 0 && strlen(apellido) > 0 && Validaciones::validarCedula(cedula) && especialidadEsValida() && costoConsultaValido();
}

bool Doctor::especialidadEsValida() const {
    // Lista simple de especialidades válidas
    const char* especialidades[] = {"Medicina General", "Cardiologia", "Pediatria", "Dermatologia", "Ginecologia"};
    for (const char* esp : especialidades) {
        if (strcmp(especialidad, esp) == 0) return true;
    }
    return false;
}

bool Doctor::costoConsultaValido() const {
    return costoConsulta >= 0 && costoConsulta <= 1000;  // Ejemplo de rango
}

bool Doctor::agregarPacienteID(int pacienteID) {
    if (cantidadPacientes < 50) {
        pacientesIDs[cantidadPacientes++] = pacienteID;
        fechaModificacion = time(NULL);
        return true;
    }
    return false;
}

bool Doctor::eliminarPacienteID(int pacienteID) {
    for (int i = 0; i < cantidadPacientes; ++i) {
        if (pacientesIDs[i] == pacienteID) {
            for (int j = i; j < cantidadPacientes - 1; ++j) {
                pacientesIDs[j] = pacientesIDs[j + 1];
            }
            cantidadPacientes--;
            fechaModificacion = time(NULL);
            return true;
        }
    }
    return false;
}

bool Doctor::agregarCitaID(int citaID) {
    if (cantidadCitas < 50) {
        citasIDs[cantidadCitas++] = citaID;
        fechaModificacion = time(NULL);
        return true;
    }
    return false;
}

bool Doctor::eliminarCitaID(int citaID) {
    for (int i = 0; i < cantidadCitas; ++i) {
        if (citasIDs[i] == citaID) {
            for (int j = i; j < cantidadCitas - 1; ++j) {
                citasIDs[j] = citasIDs[j + 1];
            }
            cantidadCitas--;
            fechaModificacion = time(NULL);
            return true;
        }
    }
    return false;
}

bool Doctor::tienePacientes() const {
    return cantidadPacientes > 0;
}

bool Doctor::tieneCitas() const {
    return cantidadCitas > 0;
}

void Doctor::mostrarInformacionBasica() const {
    std::cout << "ID: " << id << ", Nombre: " << nombre << " " << apellido << ", Especialidad: " << especialidad << std::endl;
}

void Doctor::mostrarInformacionCompleta() const {
    mostrarInformacionBasica();
    std::cout << "Cedula: " << cedula << ", Anios Exp: " << aniosExperiencia << ", Costo: " << costoConsulta << std::endl;
    std::cout << "Telefono: " << telefono << ", Email: " << email << ", Disponible: " << (disponible ? "Si" : "No") << std::endl;
}

size_t Doctor::obtenerTamano() {
    return sizeof(Doctor);
}