#ifndef PACIENTE_HPP
#define PACIENTE_HPP

#include <cstring>
#include <ctime>
#include <iostream>

class Paciente {
private:
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    int citasIDs[20];
    int cantidadCitas;
    char alergias[500];
    char observaciones[500];
    int primerConsultaID;
    int cantidadConsultas;
    bool eliminado;
    time_t fechaModificacion;

public:
    // Constructores
    Paciente();
    Paciente(int id, const char* nombre, const char* apellido, const char* cedula);
    Paciente(const Paciente& other);

    // Destructor
    ~Paciente();

    // Getters
    int getId() const;
    const char* getNombre() const;
    const char* getApellido() const;
    const char* getCedula() const;
    int getEdad() const;
    char getSexo() const;
    const char* getTipoSangre() const;
    const char* getTelefono() const;
    const char* getDireccion() const;
    const char* getEmail() const;
    const int* getCitasIDs() const;
    int getCantidadCitas() const;
    const char* getAlergias() const;
    const char* getObservaciones() const;
    int getPrimerConsultaID() const;
    int getCantidadConsultas() const;
    bool isEliminado() const;
    time_t getFechaModificacion() const;

    // Setters
    void setId(int id);
    void setNombre(const char* nombre);
    void setApellido(const char* apellido);
    void setCedula(const char* cedula);
    void setEdad(int edad);
    void setSexo(char sexo);
    void setTipoSangre(const char* tipoSangre);
    void setTelefono(const char* telefono);
    void setDireccion(const char* direccion);
    void setEmail(const char* email);
    void setAlergias(const char* alergias);
    void setObservaciones(const char* observaciones);
    void setPrimerConsultaID(int id);
    void setCantidadConsultas(int cantidad);
    void setEliminado(bool eliminado);
    void setFechaModificacion(time_t fecha);

    // Métodos de validación específicos
    bool validarDatos() const;
    bool esMayorDeEdad() const;
    bool cedulaEsValida() const;

    // Métodos de gestión de relaciones
    bool agregarCitaID(int citaID);
    bool eliminarCitaID(int citaID);
    bool tieneCitas() const;

    // Métodos de presentación
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    // Método estático
    static size_t obtenerTamano();
};

#endif