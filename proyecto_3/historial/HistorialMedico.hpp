#ifndef HISTORIAL_MEDICO_HPP
#define HISTORIAL_MEDICO_HPP

#include <cstring>
#include <ctime>
#include <iostream>

class HistorialMedico {
private:
    int idConsulta;
    int idPaciente;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int idDoctor;
    float costo;
    int siguienteConsultaID;
    time_t fechaRegistro;
    bool eliminado;

public:
    // Constructores
    HistorialMedico();
    HistorialMedico(int idConsulta, int idPaciente, const char* diagnostico, const char* tratamiento, int idDoctor, float costo);
    HistorialMedico(const HistorialMedico& other);

    // Destructor
    ~HistorialMedico();

    // Getters
    int getIdConsulta() const;
    int getIdPaciente() const;
    const char* getFecha() const;
    const char* getHora() const;
    const char* getDiagnostico() const;
    const char* getTratamiento() const;
    const char* getMedicamentos() const;
    int getIdDoctor() const;
    float getCosto() const;
    int getSiguienteConsultaID() const;
    time_t getFechaRegistro() const;
    bool isEliminado() const;

    // Setters
    void setIdConsulta(int id);
    void setIdPaciente(int id);
    void setFecha(const char* fecha);
    void setHora(const char* hora);
    void setDiagnostico(const char* diagnostico);
    void setTratamiento(const char* tratamiento);
    void setMedicamentos(const char* medicamentos);
    void setIdDoctor(int id);
    void setCosto(float costo);
    void setSiguienteConsultaID(int id);
    void setFechaRegistro(time_t fecha);
    void setEliminado(bool eliminado);

    // Métodos de validación específicos
    bool validarDatos() const;

    // Métodos de navegación
    void setSiguiente(HistorialMedico* siguiente);

    // Métodos de presentación
    void mostrarInformacion() const;

    // Método estático
    static size_t obtenerTamano();
};

#endif