#include "HistorialMedico.hpp"
#include <iostream>
#include <cstring>
using namespace std;

HistorialMedico::HistorialMedico() {
    idConsulta = 0;
    idPaciente = 0;
    strcpy(fecha, "");
    strcpy(hora, "");
    strcpy(diagnostico, "");
    strcpy(tratamiento, "");
    strcpy(medicamentos, "");
    idDoctor = 0;
    costo = 0.0f;
    siguienteConsultaID = -1;
    fechaRegistro = time(NULL);
    eliminado = false;
}

HistorialMedico::HistorialMedico(int idConsulta, int idPaciente, const char* diagnostico, const char* tratamiento, int idDoctor, float costo) {
    this->idConsulta = idConsulta;
    this->idPaciente = idPaciente;
    strcpy(fecha, "");  // Set current date
    strcpy(hora, "");  // Set current time
    strncpy(this->diagnostico, diagnostico, 199);
    this->diagnostico[199] = '\0';
    strncpy(this->tratamiento, tratamiento, 199);
    this->tratamiento[199] = '\0';
    strcpy(medicamentos, "");
    this->idDoctor = idDoctor;
    this->costo = costo;
    siguienteConsultaID = -1;
    fechaRegistro = time(NULL);
    eliminado = false;
}

HistorialMedico::HistorialMedico(const HistorialMedico& other) {
    idConsulta = other.idConsulta;
    idPaciente = other.idPaciente;
    strcpy(fecha, other.fecha);
    strcpy(hora, other.hora);
    strcpy(diagnostico, other.diagnostico);
    strcpy(tratamiento, other.tratamiento);
    strcpy(medicamentos, other.medicamentos);
    idDoctor = other.idDoctor;
    costo = other.costo;
    siguienteConsultaID = other.siguienteConsultaID;
    fechaRegistro = other.fechaRegistro;
    eliminado = other.eliminado;
}

HistorialMedico::~HistorialMedico() {}

int HistorialMedico::getIdConsulta() const { return idConsulta; }
int HistorialMedico::getIdPaciente() const { return idPaciente; }
const char* HistorialMedico::getFecha() const { return fecha; }
const char* HistorialMedico::getHora() const { return hora; }
const char* HistorialMedico::getDiagnostico() const { return diagnostico; }
const char* HistorialMedico::getTratamiento() const { return tratamiento; }
const char* HistorialMedico::getMedicamentos() const { return medicamentos; }
int HistorialMedico::getIdDoctor() const { return idDoctor; }
float HistorialMedico::getCosto() const { return costo; }
int HistorialMedico::getSiguienteConsultaID() const { return siguienteConsultaID; }
time_t HistorialMedico::getFechaRegistro() const { return fechaRegistro; }
bool HistorialMedico::isEliminado() const { return eliminado; }

void HistorialMedico::setIdConsulta(int id) { idConsulta = id; }
void HistorialMedico::setIdPaciente(int id) { idPaciente = id; }
void HistorialMedico::setFecha(const char* fecha) { strncpy(this->fecha, fecha, 10); this->fecha[10] = '\0'; }
void HistorialMedico::setHora(const char* hora) { strncpy(this->hora, hora, 5); this->hora[5] = '\0'; }
void HistorialMedico::setDiagnostico(const char* diagnostico) { strncpy(this->diagnostico, diagnostico, 199); this->diagnostico[199] = '\0'; }
void HistorialMedico::setTratamiento(const char* tratamiento) { strncpy(this->tratamiento, tratamiento, 199); this->tratamiento[199] = '\0'; }
void HistorialMedico::setMedicamentos(const char* medicamentos) { strncpy(this->medicamentos, medicamentos, 149); this->medicamentos[149] = '\0'; }
void HistorialMedico::setIdDoctor(int id) { idDoctor = id; }
void HistorialMedico::setCosto(float costo) { this->costo = costo; }
void HistorialMedico::setSiguienteConsultaID(int id) { siguienteConsultaID = id; }
void HistorialMedico::setFechaRegistro(time_t fecha) { fechaRegistro = fecha; }
void HistorialMedico::setEliminado(bool eliminado) { this->eliminado = eliminado; }

bool HistorialMedico::validarDatos() const {
    return idPaciente > 0 && idDoctor > 0 && strlen(diagnostico) > 0 && costo >= 0;
}

void HistorialMedico::setSiguiente(HistorialMedico* siguiente) {
    if (siguiente) {
        siguienteConsultaID = siguiente->idConsulta;
    } else {
        siguienteConsultaID = -1;
    }
}

void HistorialMedico::mostrarInformacion() const {
    cout << "Consulta ID: " << idConsulta << ", Paciente: " << idPaciente << ", Doctor: " << idDoctor << endl;
    cout << "Diagnostico: " << diagnostico << endl;
    cout << "Tratamiento: " << tratamiento << ", Costo: " << costo << endl;
}

size_t HistorialMedico::obtenerTamano() {
    return sizeof(HistorialMedico);
}