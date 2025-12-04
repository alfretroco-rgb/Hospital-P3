#include "Paciente.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Paciente::Paciente() {
    id = 0;
    strcpy(nombre, "");
    strcpy(apellido, "");
    strcpy(cedula, "");
    edad = 0;
    sexo = ' ';
    strcpy(tipoSangre, "");
    strcpy(telefono, "");
    strcpy(direccion, "");
    strcpy(email, "");
    cantidadCitas = 0;
    strcpy(alergias, "");
    strcpy(observaciones, "");
    primerConsultaID = -1;
    cantidadConsultas = 0;
    eliminado = false;
    fechaModificacion = time(NULL);
}

Paciente::Paciente(int id, const char* nombre, const char* apellido, const char* cedula) {
    this->id = id;
    strncpy(this->nombre, nombre, 49);
    this->nombre[49] = '\0';
    strncpy(this->apellido, apellido, 49);
    this->apellido[49] = '\0';
    strncpy(this->cedula, cedula, 19);
    this->cedula[19] = '\0';
    edad = 0;
    sexo = ' ';
    strcpy(tipoSangre, "");
    strcpy(telefono, "");
    strcpy(direccion, "");
    strcpy(email, "");
    cantidadCitas = 0;
    strcpy(alergias, "");
    strcpy(observaciones, "");
    primerConsultaID = -1;
    cantidadConsultas = 0;
    eliminado = false;
    fechaModificacion = time(NULL);
}

Paciente::Paciente(const Paciente& other) {
    id = other.id;
    strcpy(nombre, other.nombre);
    strcpy(apellido, other.apellido);
    strcpy(cedula, other.cedula);
    edad = other.edad;
    sexo = other.sexo;
    strcpy(tipoSangre, other.tipoSangre);
    strcpy(telefono, other.telefono);
    strcpy(direccion, other.direccion);
    strcpy(email, other.email);
    cantidadCitas = other.cantidadCitas;
    for (int i = 0; i < cantidadCitas; ++i) {
        citasIDs[i] = other.citasIDs[i];
    }
    strcpy(alergias, other.alergias);
    strcpy(observaciones, other.observaciones);
    primerConsultaID = other.primerConsultaID;
    cantidadConsultas = other.cantidadConsultas;
    eliminado = other.eliminado;
    fechaModificacion = other.fechaModificacion;
}

Paciente::~Paciente() {
    // No hay punteros dinámicos, destructor vacío
}

int Paciente::getId() const { return id; }
const char* Paciente::getNombre() const { return nombre; }
const char* Paciente::getApellido() const { return apellido; }
const char* Paciente::getCedula() const { return cedula; }
int Paciente::getEdad() const { return edad; }
char Paciente::getSexo() const { return sexo; }
const char* Paciente::getTipoSangre() const { return tipoSangre; }
const char* Paciente::getTelefono() const { return telefono; }
const char* Paciente::getDireccion() const { return direccion; }
const char* Paciente::getEmail() const { return email; }
const int* Paciente::getCitasIDs() const { return citasIDs; }
int Paciente::getCantidadCitas() const { return cantidadCitas; }
const char* Paciente::getAlergias() const { return alergias; }
const char* Paciente::getObservaciones() const { return observaciones; }
int Paciente::getPrimerConsultaID() const { return primerConsultaID; }
int Paciente::getCantidadConsultas() const { return cantidadConsultas; }
bool Paciente::isEliminado() const { return eliminado; }
time_t Paciente::getFechaModificacion() const { return fechaModificacion; }

void Paciente::setId(int id) { this->id = id; fechaModificacion = time(NULL); }
void Paciente::setNombre(const char* nombre) { strncpy(this->nombre, nombre, 49); this->nombre[49] = '\0'; fechaModificacion = time(NULL); }
void Paciente::setApellido(const char* apellido) { strncpy(this->apellido, apellido, 49); this->apellido[49] = '\0'; fechaModificacion = time(NULL); }
void Paciente::setCedula(const char* cedula) { if (Validaciones::validarCedula(cedula)) { strncpy(this->cedula, cedula, 19); this->cedula[19] = '\0'; fechaModificacion = time(NULL); } }
void Paciente::setEdad(int edad) { if (edad >= 0 && edad <= 120) { this->edad = edad; fechaModificacion = time(NULL); } }
void Paciente::setSexo(char sexo) { if (sexo == 'M' || sexo == 'F') { this->sexo = sexo; fechaModificacion = time(NULL); } }
void Paciente::setTipoSangre(const char* tipoSangre) { strncpy(this->tipoSangre, tipoSangre, 4); this->tipoSangre[4] = '\0'; fechaModificacion = time(NULL); }
void Paciente::setTelefono(const char* telefono) { if (Validaciones::validarTelefono(telefono)) { strncpy(this->telefono, telefono, 14); this->telefono[14] = '\0'; fechaModificacion = time(NULL); } }
void Paciente::setDireccion(const char* direccion) { strncpy(this->direccion, direccion, 99); this->direccion[99] = '\0'; fechaModificacion = time(NULL); }
void Paciente::setEmail(const char* email) { if (Validaciones::validarEmail(email)) { strncpy(this->email, email, 49); this->email[49] = '\0'; fechaModificacion = time(NULL); } }
void Paciente::setAlergias(const char* alergias) { strncpy(this->alergias, alergias, 499); this->alergias[499] = '\0'; fechaModificacion = time(NULL); }
void Paciente::setObservaciones(const char* observaciones) { strncpy(this->observaciones, observaciones, 499); this->observaciones[499] = '\0'; fechaModificacion = time(NULL); }
void Paciente::setPrimerConsultaID(int id) { primerConsultaID = id; fechaModificacion = time(NULL); }
void Paciente::setCantidadConsultas(int cantidad) { cantidadConsultas = cantidad; fechaModificacion = time(NULL); }
void Paciente::setEliminado(bool eliminado) { this->eliminado = eliminado; fechaModificacion = time(NULL); }
void Paciente::setFechaModificacion(time_t fecha) { fechaModificacion = fecha; }

bool Paciente::validarDatos() const {
    return strlen(nombre) > 0 && strlen(apellido) > 0 && cedulaEsValida() && Validaciones::validarRango(edad, 0, 120) && (sexo == 'M' || sexo == 'F');
}

bool Paciente::esMayorDeEdad() const {
    return edad >= 18;
}

bool Paciente::cedulaEsValida() const {
    return Validaciones::validarCedula(cedula);
}

bool Paciente::agregarCitaID(int citaID) {
    if (cantidadCitas < 20) {
        citasIDs[cantidadCitas++] = citaID;
        fechaModificacion = time(NULL);
        return true;
    }
    return false;
}

bool Paciente::eliminarCitaID(int citaID) {
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

bool Paciente::tieneCitas() const {
    return cantidadCitas > 0;
}

void Paciente::mostrarInformacionBasica() const {
    std::cout << "ID: " << id << ", Nombre: " << nombre << " " << apellido << ", Cedula: " << cedula << std::endl;
}

void Paciente::mostrarInformacionCompleta() const {
    mostrarInformacionBasica();
    std::cout << "Edad: " << edad << ", Sexo: " << sexo << ", Telefono: " << telefono << std::endl;
    std::cout << "Direccion: " << direccion << ", Email: " << email << std::endl;
    std::cout << "Alergias: " << alergias << ", Observaciones: " << observaciones << std::endl;
}

size_t Paciente::obtenerTamano() {
    return sizeof(Paciente);
}