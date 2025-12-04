#include "Cita.hpp"
#include "../utilidades/Validaciones.hpp"
#include <iostream>
#include <cstring>

Cita::Cita() {
    id = 0;
    idPaciente = 0;
    idDoctor = 0;
    strcpy(fecha, "");
    strcpy(hora, "");
    strcpy(motivo, "");
    strcpy(estado, "Pendiente");
    strcpy(observaciones, "");
    atendida = false;
    eliminado = false;
    fechaModificacion = time(NULL);
}

Cita::Cita(int id, int idPaciente, int idDoctor, const char* fecha, const char* hora, const char* motivo) {
    this->id = id;
    this->idPaciente = idPaciente;
    this->idDoctor = idDoctor;
    strncpy(this->fecha, fecha, 10);
    this->fecha[10] = '\0';
    strncpy(this->hora, hora, 5);
    this->hora[5] = '\0';
    strncpy(this->motivo, motivo, 149);
    this->motivo[149] = '\0';
    strcpy(estado, "Pendiente");
    strcpy(observaciones, "");
    atendida = false;
    eliminado = false;
    fechaModificacion = time(NULL);
}

Cita::Cita(const Cita& other) {
    id = other.id;
    idPaciente = other.idPaciente;
    idDoctor = other.idDoctor;
    strcpy(fecha, other.fecha);
    strcpy(hora, other.hora);
    strcpy(motivo, other.motivo);
    strcpy(estado, other.estado);
    strcpy(observaciones, other.observaciones);
    atendida = other.atendida;
    eliminado = other.eliminado;
    fechaModificacion = other.fechaModificacion;
}

Cita::~Cita() {}

int Cita::getId() const { return id; }
int Cita::getIdPaciente() const { return idPaciente; }
int Cita::getIdDoctor() const { return idDoctor; }
const char* Cita::getFecha() const { return fecha; }
const char* Cita::getHora() const { return hora; }
const char* Cita::getMotivo() const { return motivo; }
const char* Cita::getEstado() const { return estado; }
const char* Cita::getObservaciones() const { return observaciones; }
bool Cita::isAtendida() const { return atendida; }
bool Cita::isEliminado() const { return eliminado; }
time_t Cita::getFechaModificacion() const { return fechaModificacion; }

void Cita::setId(int id) { this->id = id; fechaModificacion = time(NULL); }
void Cita::setIdPaciente(int idPaciente) { this->idPaciente = idPaciente; fechaModificacion = time(NULL); }
void Cita::setIdDoctor(int idDoctor) { this->idDoctor = idDoctor; fechaModificacion = time(NULL); }
void Cita::setFecha(const char* fecha) { if (Validaciones::validarFecha(fecha)) { strncpy(this->fecha, fecha, 10); this->fecha[10] = '\0'; fechaModificacion = time(NULL); } }
void Cita::setHora(const char* hora) { strncpy(this->hora, hora, 5); this->hora[5] = '\0'; fechaModificacion = time(NULL); }
void Cita::setMotivo(const char* motivo) { strncpy(this->motivo, motivo, 149); this->motivo[149] = '\0'; fechaModificacion = time(NULL); }
void Cita::setEstado(const char* estado) { strncpy(this->estado, estado, 19); this->estado[19] = '\0'; fechaModificacion = time(NULL); }
void Cita::setObservaciones(const char* observaciones) { strncpy(this->observaciones, observaciones, 199); this->observaciones[199] = '\0'; fechaModificacion = time(NULL); }
void Cita::setAtendida(bool atendida) { this->atendida = atendida; fechaModificacion = time(NULL); }
void Cita::setEliminado(bool eliminado) { this->eliminado = eliminado; fechaModificacion = time(NULL); }
void Cita::setFechaModificacion(time_t fecha) { fechaModificacion = fecha; }

bool Cita::validarDatos() const {
    return idPaciente > 0 && idDoctor > 0 && Validaciones::validarFecha(fecha) && strlen(motivo) > 0;
}

bool Cita::fechaHoraValidas() const {
    // Simple: fecha válida y hora en formato HH:MM
    return Validaciones::validarFecha(fecha) && strlen(hora) == 5 && hora[2] == ':';
}

void Cita::marcarComoAtendida() {
    setEstado("Atendida");
    setAtendida(true);
}

void Cita::cancelar() {
    setEstado("Cancelada");
}

void Cita::mostrarInformacionBasica() const {
    std::cout << "ID: " << id << ", Paciente: " << idPaciente << ", Doctor: " << idDoctor << ", Fecha: " << fecha << " " << hora << std::endl;
}

void Cita::mostrarInformacionCompleta() const {
    mostrarInformacionBasica();
    std::cout << "Motivo: " << motivo << ", Estado: " << estado << ", Atendida: " << (atendida ? "Si" : "No") << std::endl;
    std::cout << "Observaciones: " << observaciones << std::endl;
}

size_t Cita::obtenerTamano() {
    return sizeof(Cita);
}