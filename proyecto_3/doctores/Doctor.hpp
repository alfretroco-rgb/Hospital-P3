#ifndef DOCTOR_HPP
#define DOCTOR_HPP

#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;

class Doctor {
private:
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    int pacientesIDs[50];
    int cantidadPacientes;
    int citasIDs[50];
    int cantidadCitas;
    bool disponible;
    bool eliminado;
    time_t fechaModificacion;

public:
    // Constructores
    Doctor();
    Doctor(int id, const char* nombre, const char* apellido, const char* cedula, const char* especialidad, int aniosExperiencia, float costoConsulta);
    Doctor(const Doctor& other);

    // Destructor
    ~Doctor();

    // Getters
    int getId() const;
    const char* getNombre() const;
    const char* getApellido() const;
    const char* getCedula() const;
    const char* getEspecialidad() const;
    int getAniosExperiencia() const;
    float getCostoConsulta() const;
    const char* getHorarioAtencion() const;
    const char* getTelefono() const;
    const char* getEmail() const;
    const int* getPacientesIDs() const;
    int getCantidadPacientes() const;
    const int* getCitasIDs() const;
    int getCantidadCitas() const;
    bool isDisponible() const;
    bool isEliminado() const;
    time_t getFechaModificacion() const;

    // Setters
    void setId(int id);
    void setNombre(const char* nombre);
    void setApellido(const char* apellido);
    void setCedula(const char* cedula);
    void setEspecialidad(const char* especialidad);
    void setAniosExperiencia(int anios);
    void setCostoConsulta(float costo);
    void setHorarioAtencion(const char* horario);
    void setTelefono(const char* telefono);
    void setEmail(const char* email);
    void setDisponible(bool disponible);
    void setEliminado(bool eliminado);
    void setFechaModificacion(time_t fecha);

    // Métodos de validación específicos
    bool validarDatos() const;
    bool especialidadEsValida() const;
    bool costoConsultaValido() const;

    // Métodos de gestión de relaciones
    bool agregarPacienteID(int pacienteID);
    bool eliminarPacienteID(int pacienteID);
    bool agregarCitaID(int citaID);
    bool eliminarCitaID(int citaID);
    bool tienePacientes() const;
    bool tieneCitas() const;

    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    // Método estático
    static size_t obtenerTamano();
};

#endif