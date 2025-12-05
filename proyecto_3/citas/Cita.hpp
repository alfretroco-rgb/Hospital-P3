#ifndef CITA_HPP
#define CITA_HPP

#include <cstring>
#include <ctime>
#include <iostream>
using namespace std;

class Cita {
private:
    int id;
    int idPaciente;
    int idDoctor;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
    bool eliminado;
    time_t fechaModificacion;

public:
    // Constructores
    Cita();
    Cita(int id, int idPaciente, int idDoctor, const char* fecha, const char* hora, const char* motivo);
    Cita(const Cita& other);

    // Destructor
    ~Cita();

    // Getters
    int getId() const;
    int getIdPaciente() const;
    int getIdDoctor() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getMotivo() const;
    const char* getEstado() const;
    const char* getObservaciones() const;
    bool isAtendida() const;
    bool isEliminado() const;
    time_t getFechaModificacion() const;

    // Setters
    void setId(int id);
    void setIdPaciente(int idPaciente);
    void setIdDoctor(int idDoctor);
    void setFecha(const char* fecha);
    void setHora(const char* hora);
    void setMotivo(const char* motivo);
    void setEstado(const char* estado);
    void setObservaciones(const char* observaciones);
    void setAtendida(bool atendida);
    void setEliminado(bool eliminado);
    void setFechaModificacion(time_t fecha);

    // Métodos de validación específicos
    bool validarDatos() const;
    bool fechaHoraValidas() const;

    // Métodos de gestión
    void marcarComoAtendida();
    void cancelar();

    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    // Método estático
    static size_t obtenerTamano();
};

#endif