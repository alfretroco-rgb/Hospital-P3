#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// Estructura para el header de cada archivo binario
struct ArchivoHeader {
    int cantidadRegistros;      // Cantidad actual de registros
    int proximoID;              // Siguiente ID disponible
    int registrosActivos;       // Registros no eliminados
    int version;                // Versión del formato
};

// Declaración de estructuras
struct HistorialMedico;
struct Paciente;
struct Doctor;
struct Cita;
struct Hospital;

// Prototipos de funciones
bool validarFecha(const char* fecha);
bool validarHora(const char* hora);
int compararFechas(const char* fecha1, const char* fecha2);
bool validarCedula(const char* cedula);
bool validarEmail(const char* email);
char* copiarString(const char* origen);
Paciente* copiarPaciente(Paciente* original);

// Funciones de archivos binarios
bool inicializarArchivo(const char* nombreArchivo);
ArchivoHeader leerHeader(const char* nombreArchivo);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);
long calcularPosicion(int indice, size_t tamanoRegistro);
bool agregarRegistro(const char* nombreArchivo, const void* registro, size_t tamano);
bool leerRegistroPorIndice(const char* nombreArchivo, int indice, void* registro, size_t tamano);
int buscarIndiceDeID(const char* nombreArchivo, int id, size_t tamanoRegistro);
bool actualizarRegistro(const char* nombreArchivo, int id, const void* registro, size_t tamano);
bool eliminarRegistro(const char* nombreArchivo, int id, size_t tamanoRegistro);
bool verificarArchivo(const char* nombreArchivo);
Hospital* cargarDatosHospital();
bool guardarDatosHospital(Hospital* hospital);
void listarTodosPacientes();
bool compactarArchivoPacientes();
bool verificarIntegridadReferencial();
bool hacerRespaldo();
bool restaurarRespaldo();
void estadisticasDeArchivos();
void eliminarTodosPacientes(Hospital* hospital);
void eliminarTodosDoctores(Hospital* hospital);
bool actualizarDoctor(Hospital* hospital, int id);

// Prototipos de módulos
Paciente* crearPaciente(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, int edad, char sexo);
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula);
Paciente* buscarPacientePorId(Hospital* hospital, int id);
Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* nombre, int* cantidad);
bool actualizarPaciente(Hospital* hospital, int id);
bool eliminarPaciente(Hospital* hospital, int id);
void mostrarHistorialMedico(Paciente* paciente);
void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta);
HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad);

Doctor* crearDoctor(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, const char* especialidad, int aniosExperiencia, float costoConsulta);
Doctor* buscarDoctorPorId(Hospital* hospital, int id);
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad);
bool asignarPacienteADoctor(Doctor* doctor, int idPaciente);
void listarPacientesDeDoctor(Hospital* hospital, int idDoctor);
void listarDoctores(Hospital* hospital);
bool eliminarDoctor(Hospital* hospital, int id);

Cita* agendarCita(Hospital* hospital, int idPaciente, int idDoctor, const char* fecha, const char* hora, const char* motivo);
bool cancelarCita(Hospital* hospital, int idCita);
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos);
Cita** obtenerCitasDePaciente(Hospital* hospital, int idPaciente, int* cantidad);
Cita** obtenerCitasDeDoctor(Hospital* hospital, int idDoctor, int* cantidad);
Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad);
void listarCitasPendientes(Hospital* hospital);

// Modelo de datos - estructuras modificadas para archivos
struct HistorialMedico {
    int idConsulta;
    int idPaciente;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int idDoctor;
    float costo;
    int siguienteConsultaID;  // Para lista enlazada en disco
    time_t fechaRegistro;
    bool eliminado;
};

struct Paciente {
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
    int citasIDs[20];  // Array fijo para citas (máximo 20)
    int cantidadCitas;
    char alergias[500];
    char observaciones[500];
    int primerConsultaID;  // ID de la primera consulta en historiales.bin
    int cantidadConsultas;
    bool eliminado;  // Para borrado lógico
    time_t fechaModificacion;
};

struct Doctor {
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
    int pacientesIDs[50];  // Array fijo para pacientes asignados
    int cantidadPacientes;
    int citasIDs[50];  // Array fijo para citas
    int cantidadCitas;
    bool disponible;
    bool eliminado;
    time_t fechaModificacion;
};

struct Cita {
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
};

struct Hospital {
    char nombre[100];
    char direccion[150];
    char telefono[15];
    int totalPacientes;
    int totalDoctores;
    int totalCitas;
    int totalConsultasRealizadas;
    time_t fechaCreacion;
};

// Funciones de utilidad
int stringToInt(const char* str) {
    int num = 0;
    while (*str >= '0' && *str <= '9') {
        num = num * 10 + (*str - '0');
        str++;
    }
    return num;
}

bool contieneIgnoreCase(const char* texto, const char* busqueda) {
    if (!busqueda || !*busqueda) return true;
    size_t lenB = strlen(busqueda);
    size_t lenT = strlen(texto);
    for (size_t i = 0; i <= lenT - lenB; ++i) {
        bool match = true;
        for (size_t j = 0; j < lenB; ++j) {
            char c1 = texto[i + j];
            char c2 = busqueda[j];
            if (c1 >= 'A' && c1 <= 'Z') c1 += 32;
            if (c2 >= 'A' && c2 <= 'Z') c2 += 32;
            if (c1 != c2) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

// Funciones de archivos binarios
bool inicializarArchivo(const char* nombreArchivo) {
    ArchivoHeader header = {0, 1, 0, 1};  // Inicializar header
    ofstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return false;
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

ArchivoHeader leerHeader(const char* nombreArchivo) {
    ArchivoHeader header;
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) {
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = 1;
        return header;
    }
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    return header;
}

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    ofstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;
    archivo.seekp(0);
    archivo.write((char*)&header, sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

long calcularPosicion(int indice, size_t tamanoRegistro) {
    return sizeof(ArchivoHeader) + (indice * tamanoRegistro);
}

bool agregarRegistro(const char* nombreArchivo, const void* registro, size_t tamano) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    ofstream archivo(nombreArchivo, ios::binary | ios::app);
    if (!archivo.is_open()) return false;
    archivo.write((char*)registro, tamano);
    header.cantidadRegistros++;
    header.registrosActivos++;
    actualizarHeader(nombreArchivo, header);
    archivo.close();
    return true;
}

bool leerRegistroPorIndice(const char* nombreArchivo, int indice, void* registro, size_t tamano) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return false;
    long posicion = calcularPosicion(indice, tamano);
    archivo.seekg(posicion);
    archivo.read((char*)registro, tamano);
    archivo.close();
    return archivo.good();
}

int buscarIndiceDeID(const char* nombreArchivo, int id, size_t tamanoRegistro) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return -1;
    
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        long posicion = calcularPosicion(i, tamanoRegistro);
        archivo.seekg(posicion);
        int registroID;
        archivo.read((char*)&registroID, sizeof(int));
        if (registroID == id) {
            archivo.close();
            return i;
        }
    }
    archivo.close();
    return -1;
}

bool actualizarRegistro(const char* nombreArchivo, int id, const void* registro, size_t tamano) {
    int indice = buscarIndiceDeID(nombreArchivo, id, tamano);
    if (indice == -1) return false;
    
    ofstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;
    long posicion = calcularPosicion(indice, tamano);
    archivo.seekp(posicion);
    archivo.write((char*)registro, tamano);
    archivo.close();
    return true;
}

bool eliminarRegistro(const char* nombreArchivo, int id, size_t tamanoRegistro) {
    int indice = buscarIndiceDeID(nombreArchivo, id, tamanoRegistro);
    if (indice == -1) return false;
    
    // Leer el registro
    char buffer[2048];  // Asumir tamaño máximo
    if (!leerRegistroPorIndice(nombreArchivo, indice, buffer, tamanoRegistro)) return false;
    
    // Marcar como eliminado (asumiendo que el campo eliminado está en la posición correcta)
    buffer[tamanoRegistro - 1] = 1;  // Último byte es bool eliminado
    
    // Actualizar
    ofstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;
    long posicion = calcularPosicion(indice, tamanoRegistro);
    archivo.seekp(posicion);
    archivo.write(buffer, tamanoRegistro);
    archivo.close();
    
    // Actualizar header
    ArchivoHeader header = leerHeader(nombreArchivo);
    header.registrosActivos--;
    actualizarHeader(nombreArchivo, header);
    
    return true;
}

bool eliminarRegistroFisico(const char* nombreArchivo, int id, size_t tamanoRegistro) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    int indice = buscarIndiceDeID(nombreArchivo, id, tamanoRegistro);
    if (indice == -1) return false;
    
    // Crear archivo temporal
    string tempFile = string(nombreArchivo) + "_temp";
    ofstream temp(tempFile, ios::binary);
    if (!temp.is_open()) return false;
    
    ArchivoHeader nuevoHeader = {0, header.proximoID, 0, 1};
    temp.write((char*)&nuevoHeader, sizeof(ArchivoHeader));
    
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        if (i == indice) continue;  // Saltar el registro a eliminar
        char buffer[2048];
        if (leerRegistroPorIndice(nombreArchivo, i, buffer, tamanoRegistro)) {
            temp.write(buffer, tamanoRegistro);
            nuevoHeader.cantidadRegistros++;
            if (buffer[tamanoRegistro - 1] == 0) nuevoHeader.registrosActivos++;  // Asumiendo eliminado es último byte
        }
    }
    
    temp.seekp(0);
    temp.write((char*)&nuevoHeader, sizeof(ArchivoHeader));
    temp.close();
    
    remove(nombreArchivo);
    rename(tempFile.c_str(), nombreArchivo);
    
    return true;
}

bool verificarArchivo(const char* nombreArchivo) {
    ifstream archivo(nombreArchivo, ios::binary);
    if (!archivo.is_open()) return false;
    ArchivoHeader header;
    archivo.read((char*)&header, sizeof(ArchivoHeader));
    if (archivo.fail() || header.version != 1) {
        archivo.close();
        return false;
    }
    archivo.close();
    return true;
}

Hospital* cargarDatosHospital() {
    Hospital* hospital = new Hospital;
    ifstream archivo("hospital.bin", ios::binary);
    if (archivo.is_open()) {
        archivo.read((char*)hospital, sizeof(Hospital));
        archivo.close();
    } else {
        // Inicializar si no existe
        strcpy(hospital->nombre, "Hospital Central");
        strcpy(hospital->direccion, "Dirección por defecto");
        strcpy(hospital->telefono, "000-0000");
        hospital->totalPacientes = 0;
        hospital->totalDoctores = 0;
        hospital->totalCitas = 0;
        hospital->totalConsultasRealizadas = 0;
        hospital->fechaCreacion = time(NULL);
    }
    
    // Verificar e inicializar archivos si no existen
    if (!verificarArchivo("pacientes.bin")) inicializarArchivo("pacientes.bin");
    if (!verificarArchivo("doctores.bin")) inicializarArchivo("doctores.bin");
    if (!verificarArchivo("citas.bin")) inicializarArchivo("citas.bin");
    if (!verificarArchivo("historiales.bin")) inicializarArchivo("historiales.bin");
    
    return hospital;
}

// Inicialización y gestión
Hospital* inicializarHospital(const char* nombre) {
    Hospital* hospital = cargarDatosHospital();
    // Si es nuevo, setear nombre
    if (strcmp(hospital->nombre, "Hospital Central") != 0) {
        strncpy(hospital->nombre, nombre, 99);
        hospital->nombre[99] = '\0';
    }
    return hospital;
}

bool guardarDatosHospital(Hospital* hospital) {
    ofstream archivo("hospital.bin", ios::binary);
    if (!archivo.is_open()) return false;
    archivo.write((char*)hospital, sizeof(Hospital));
    archivo.close();
    return true;
}

void destruirHospital(Hospital* hospital) {
    guardarDatosHospital(hospital);
    delete hospital;
}

// Módulo de Gestión de Pacientes
Paciente* crearPaciente(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, int edad, char sexo) {
    // Verificar unicidad de cédula
    ArchivoHeader headerPacientes = leerHeader("pacientes.bin");
    for (int i = 0; i < headerPacientes.cantidadRegistros; ++i) {
        Paciente p;
        if (leerRegistroPorIndice("pacientes.bin", i, &p, sizeof(Paciente))) {
            if (!p.eliminado && strcmp(p.cedula, cedula) == 0) {
                cout << "Error: Cedula ya existe." << endl;
                return NULL;
            }
        }
    }
    
    Paciente nuevo;
    nuevo.id = headerPacientes.proximoID++;
    actualizarHeader("pacientes.bin", headerPacientes);
    strncpy(nuevo.nombre, nombre, 49);
    nuevo.nombre[49] = '\0';
    strncpy(nuevo.apellido, apellido, 49);
    nuevo.apellido[49] = '\0';
    strncpy(nuevo.cedula, cedula, 19);
    nuevo.cedula[19] = '\0';
    nuevo.edad = edad;
    nuevo.sexo = sexo;
    strcpy(nuevo.tipoSangre, "");
    strcpy(nuevo.telefono, "");
    strcpy(nuevo.direccion, "");
    strcpy(nuevo.email, "");
    nuevo.cantidadCitas = 0;
    strcpy(nuevo.alergias, "");
    strcpy(nuevo.observaciones, "");
    nuevo.primerConsultaID = -1;
    nuevo.cantidadConsultas = 0;
    nuevo.eliminado = false;
    nuevo.fechaModificacion = time(NULL);
    
    if (agregarRegistro("pacientes.bin", &nuevo, sizeof(Paciente))) {
        hospital->totalPacientes++;
        return new Paciente(nuevo);  // Retornar copia
    }
    return NULL;
}

Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (leerRegistroPorIndice("pacientes.bin", i, &p, sizeof(Paciente)) && !p.eliminado && strcmp(p.cedula, cedula) == 0) {
            return new Paciente(p);
        }
    }
    return NULL;
}

Paciente* buscarPacientePorId(Hospital* hospital, int id) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (leerRegistroPorIndice("pacientes.bin", i, &p, sizeof(Paciente)) && !p.eliminado && p.id == id) {
            return new Paciente(p);
        }
    }
    return NULL;
}

Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* nombre, int* cantidad) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    Paciente** resultados = new Paciente*[header.registrosActivos];
    *cantidad = 0;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (leerRegistroPorIndice("pacientes.bin", i, &p, sizeof(Paciente)) && !p.eliminado) {
            if (contieneIgnoreCase(p.nombre, nombre) || contieneIgnoreCase(p.apellido, nombre)) {
                resultados[(*cantidad)++] = new Paciente(p);
            }
        }
    }
    return resultados;
}

bool actualizarPaciente(Hospital* hospital, int id) {
    Paciente* p = buscarPacientePorId(hospital, id);
    if (!p) return false;
    
    cout << "Actualizando paciente: " << p->nombre << " " << p->apellido << endl;
    cout << "Nuevo nombre: ";
    char nuevoNombre[50];
    cin.getline(nuevoNombre, 50);
    strcpy(p->nombre, nuevoNombre);
    
    cout << "Nuevo apellido: ";
    char nuevoApellido[50];
    cin.getline(nuevoApellido, 50);
    strcpy(p->apellido, nuevoApellido);
    
    cout << "Nueva edad: ";
    cin >> p->edad;
    
    cout << "Nuevo sexo (M/F): ";
    cin >> p->sexo;
    cin.ignore();
    
    p->fechaModificacion = time(NULL);
    bool exito = actualizarRegistro("pacientes.bin", id, p, sizeof(Paciente));
    delete p;
    return exito;
}

bool eliminarPaciente(Hospital* hospital, int id) {
    Paciente* p = buscarPacientePorId(hospital, id);
    if (!p) return false;
    
    // Cancelar citas asociadas
    for (int i = 0; i < p->cantidadCitas; ++i) {
        eliminarRegistroFisico("citas.bin", p->citasIDs[i], sizeof(Cita));
        hospital->totalCitas--;
    }
    
    // Eliminar referencias en doctores
    ArchivoHeader headerDoctores = leerHeader("doctores.bin");
    for (int i = 0; i < headerDoctores.cantidadRegistros; ++i) {
        Doctor d;
        if (leerRegistroPorIndice("doctores.bin", i, &d, sizeof(Doctor)) && !d.eliminado) {
            int nuevaCantidad = 0;
            for (int j = 0; j < d.cantidadPacientes; ++j) {
                if (d.pacientesIDs[j] != id) {
                    d.pacientesIDs[nuevaCantidad++] = d.pacientesIDs[j];
                }
            }
            if (nuevaCantidad != d.cantidadPacientes) {
                d.cantidadPacientes = nuevaCantidad;
                actualizarRegistro("doctores.bin", d.id, &d, sizeof(Doctor));
            }
        }
    }
    
    // Eliminar historiales médicos asociados
    int consultaID = p->primerConsultaID;
    while (consultaID != -1) {
        int indice = buscarIndiceDeID("historiales.bin", consultaID, sizeof(HistorialMedico));
        if (indice != -1) {
            HistorialMedico h;
            leerRegistroPorIndice("historiales.bin", indice, &h, sizeof(HistorialMedico));
            int next = h.siguienteConsultaID;
            eliminarRegistroFisico("historiales.bin", consultaID, sizeof(HistorialMedico));
            consultaID = next;
        } else {
            break;
        }
    }
    
    delete p;
    
    // Eliminar físicamente al paciente
    if (eliminarRegistroFisico("pacientes.bin", id, sizeof(Paciente))) {
        hospital->totalPacientes--;
        return true;
    }
    return false;
}

void listarTodosPacientes() {
    ArchivoHeader header = leerHeader("pacientes.bin");
    cout << "Lista de Todos los Pacientes:" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "Nombre" << setw(20) << "Apellido" << setw(15) << "Cedula" << setw(5) << "Edad" << endl;
    int total = 0;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (leerRegistroPorIndice("pacientes.bin", i, &p, sizeof(Paciente))) {
            if (!p.eliminado) {
                cout << left << setw(5) << p.id << setw(20) << p.nombre << setw(20) << p.apellido << setw(15) << p.cedula << setw(5) << p.edad << endl;
                total++;
            }
        }
    }
    cout << "Total de pacientes activos: " << total << endl;
}

bool compactarArchivoPacientes() {
    ArchivoHeader header = leerHeader("pacientes.bin");
    if (header.registrosActivos == header.cantidadRegistros) {
        cout << "No hay registros eliminados para compactar." << endl;
        return true;  // Ya está compacto
    }
    
    // Crear archivo temporal
    ofstream temp("pacientes_temp.bin", ios::binary);
    if (!temp.is_open()) return false;
    
    ArchivoHeader nuevoHeader = {0, header.proximoID, 0, 1};
    temp.write((char*)&nuevoHeader, sizeof(ArchivoHeader));
    
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p;
        if (leerRegistroPorIndice("pacientes.bin", i, &p, sizeof(Paciente)) && !p.eliminado) {
            temp.write((char*)&p, sizeof(Paciente));
            nuevoHeader.cantidadRegistros++;
            nuevoHeader.registrosActivos++;
        }
    }
    
    temp.seekp(0);
    temp.write((char*)&nuevoHeader, sizeof(ArchivoHeader));
    temp.close();
    
    // Reemplazar archivo original
    remove("pacientes.bin");
    rename("pacientes_temp.bin", "pacientes.bin");
    
    cout << "Archivo compactado. Registros eliminados: " << (header.cantidadRegistros - nuevoHeader.cantidadRegistros) << endl;
    return true;
}

bool verificarIntegridadReferencial() {
    bool integridad = true;
    
    // Verificar citas: paciente y doctor existen
    ArchivoHeader headerCitas = leerHeader("citas.bin");
    for (int i = 0; i < headerCitas.cantidadRegistros; ++i) {
        Cita c;
        if (leerRegistroPorIndice("citas.bin", i, &c, sizeof(Cita)) && !c.eliminado) {
            if (buscarIndiceDeID("pacientes.bin", c.idPaciente, sizeof(Paciente)) == -1) {
                cout << "Error: Cita " << c.id << " referencia paciente inexistente " << c.idPaciente << endl;
                integridad = false;
            }
            if (buscarIndiceDeID("doctores.bin", c.idDoctor, sizeof(Doctor)) == -1) {
                cout << "Error: Cita " << c.id << " referencia doctor inexistente " << c.idDoctor << endl;
                integridad = false;
            }
        }
    }
    
    // Verificar historiales: paciente y doctor existen
    ArchivoHeader headerHistoriales = leerHeader("historiales.bin");
    for (int i = 0; i < headerHistoriales.cantidadRegistros; ++i) {
        HistorialMedico h;
        if (leerRegistroPorIndice("historiales.bin", i, &h, sizeof(HistorialMedico)) && !h.eliminado) {
            if (buscarIndiceDeID("pacientes.bin", h.idPaciente, sizeof(Paciente)) == -1) {
                cout << "Error: Historial " << h.idConsulta << " referencia paciente inexistente " << h.idPaciente << endl;
                integridad = false;
            }
            if (buscarIndiceDeID("doctores.bin", h.idDoctor, sizeof(Doctor)) == -1) {
                cout << "Error: Historial " << h.idConsulta << " referencia doctor inexistente " << h.idDoctor << endl;
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

bool hacerRespaldo() {
    // Crear respaldo copiando archivos
    const char* archivos[] = {"hospital.bin", "pacientes.bin", "doctores.bin", "citas.bin", "historiales.bin"};
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

bool restaurarRespaldo() {
    // Restaurar desde respaldo
    const char* archivos[] = {"hospital.bin", "pacientes.bin", "doctores.bin", "citas.bin", "historiales.bin"};
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

void estadisticasDeArchivos() {
    ArchivoHeader hPac = leerHeader("pacientes.bin");
    ArchivoHeader hDoc = leerHeader("doctores.bin");
    ArchivoHeader hCit = leerHeader("citas.bin");
    ArchivoHeader hHis = leerHeader("historiales.bin");
    cout << "Estadisticas de Archivos:" << endl;
    cout << "Pacientes: " << hPac.registrosActivos << " activos, " << hPac.cantidadRegistros << " total" << endl;
    cout << "Doctores: " << hDoc.registrosActivos << " activos, " << hDoc.cantidadRegistros << " total" << endl;
    cout << "Citas: " << hCit.registrosActivos << " activas, " << hCit.cantidadRegistros << " total" << endl;
    cout << "Historiales: " << hHis.registrosActivos << " activos, " << hHis.cantidadRegistros << " total" << endl;
}

void eliminarTodosPacientes(Hospital* hospital) {
    ArchivoHeader headerPacientes = leerHeader("pacientes.bin");
    ArchivoHeader headerDoctores = leerHeader("doctores.bin");
    ArchivoHeader headerCitas = leerHeader("citas.bin");
    
    // Recopilar IDs de pacientes activos
    int* idsPacientes = new int[headerPacientes.registrosActivos];
    int countPacientes = 0;
    for (int i = 0; i < headerPacientes.cantidadRegistros; ++i) {
        Paciente p;
        if (leerRegistroPorIndice("pacientes.bin", i, &p, sizeof(Paciente)) && !p.eliminado) {
            idsPacientes[countPacientes++] = p.id;
        }
    }
    
    // Limpiar referencias en doctores
    for (int i = 0; i < headerDoctores.cantidadRegistros; ++i) {
        Doctor d;
        if (leerRegistroPorIndice("doctores.bin", i, &d, sizeof(Doctor)) && !d.eliminado) {
            int nuevaCantidad = 0;
            for (int j = 0; j < d.cantidadPacientes; ++j) {
                bool mantener = true;
                for (int k = 0; k < countPacientes; ++k) {
                    if (d.pacientesIDs[j] == idsPacientes[k]) {
                        mantener = false;
                        break;
                    }
                }
                if (mantener) {
                    d.pacientesIDs[nuevaCantidad++] = d.pacientesIDs[j];
                }
            }
            d.cantidadPacientes = nuevaCantidad;
            actualizarRegistro("doctores.bin", d.id, &d, sizeof(Doctor));
        }
    }
    
    // Recopilar IDs de citas a eliminar
    int* idsCitas = new int[headerCitas.registrosActivos];
    int countCitas = 0;
    for (int i = 0; i < headerCitas.cantidadRegistros; ++i) {
        Cita c;
        if (leerRegistroPorIndice("citas.bin", i, &c, sizeof(Cita)) && !c.eliminado) {
            for (int k = 0; k < countPacientes; ++k) {
                if (c.idPaciente == idsPacientes[k]) {
                    idsCitas[countCitas++] = c.id;
                    break;
                }
            }
        }
    }
    // Eliminar las citas
    for (int j = 0; j < countCitas; ++j) {
        eliminarRegistro("citas.bin", idsCitas[j], sizeof(Cita));
        hospital->totalCitas--;
    }
    delete[] idsCitas;
    
    // Recrear archivo de pacientes vacío
    ArchivoHeader nuevoHeaderPacientes = {0, 1, 0, 1};
    ofstream archivoPacientes("pacientes.bin", ios::binary | ios::trunc);
    if (archivoPacientes.is_open()) {
        archivoPacientes.write((char*)&nuevoHeaderPacientes, sizeof(ArchivoHeader));
        archivoPacientes.close();
    }
    
    // Actualizar estadísticas del hospital
    hospital->totalPacientes = 0;
    
    delete[] idsPacientes;
    cout << "Todos los pacientes activos han sido eliminados completamente." << endl;
}

void eliminarTodosDoctores(Hospital* hospital) {
    ArchivoHeader headerDoctores = leerHeader("doctores.bin");
    ArchivoHeader headerCitas = leerHeader("citas.bin");
    
    // Recopilar IDs de doctores activos
    int* idsDoctores = new int[headerDoctores.registrosActivos];
    int countDoctores = 0;
    for (int i = 0; i < headerDoctores.cantidadRegistros; ++i) {
        Doctor d;
        if (leerRegistroPorIndice("doctores.bin", i, &d, sizeof(Doctor)) && !d.eliminado) {
            idsDoctores[countDoctores++] = d.id;
        }
    }
    
    // Recopilar IDs de citas a eliminar
    int* idsCitas = new int[headerCitas.registrosActivos];
    int countCitas = 0;
    for (int i = 0; i < headerCitas.cantidadRegistros; ++i) {
        Cita c;
        if (leerRegistroPorIndice("citas.bin", i, &c, sizeof(Cita)) && !c.eliminado) {
            for (int k = 0; k < countDoctores; ++k) {
                if (c.idDoctor == idsDoctores[k]) {
                    idsCitas[countCitas++] = c.id;
                    break;
                }
            }
        }
    }
    // Eliminar las citas
    for (int j = 0; j < countCitas; ++j) {
        eliminarRegistro("citas.bin", idsCitas[j], sizeof(Cita));
        hospital->totalCitas--;
    }
    delete[] idsCitas;
    
    // Recrear archivo de doctores vacío
    ArchivoHeader nuevoHeaderDoctores = {0, 1, 0, 1};
    ofstream archivoDoctores("doctores.bin", ios::binary | ios::trunc);
    if (archivoDoctores.is_open()) {
        archivoDoctores.write((char*)&nuevoHeaderDoctores, sizeof(ArchivoHeader));
        archivoDoctores.close();
    }
    
    // Actualizar estadísticas del hospital
    hospital->totalDoctores = 0;
    
    delete[] idsDoctores;
    cout << "Todos los doctores activos han sido eliminados completamente." << endl;
}

bool actualizarDoctor(Hospital* hospital, int id) {
    Doctor* d = buscarDoctorPorId(hospital, id);
    if (!d) return false;
    
    cout << "Actualizando doctor: " << d->nombre << " " << d->apellido << endl;
    cout << "Nuevo nombre: ";
    char nuevoNombre[50];
    cin.getline(nuevoNombre, 50);
    strcpy(d->nombre, nuevoNombre);
    
    cout << "Nuevo apellido: ";
    char nuevoApellido[50];
    cin.getline(nuevoApellido, 50);
    strcpy(d->apellido, nuevoApellido);
    
    cout << "Nueva especialidad: ";
    char nuevaEspecialidad[50];
    cin.getline(nuevaEspecialidad, 50);
    strcpy(d->especialidad, nuevaEspecialidad);
    
    cout << "Nuevos años de experiencia: ";
    cin >> d->aniosExperiencia;
    
    cout << "Nuevo costo de consulta: ";
    cin >> d->costoConsulta;
    cin.ignore();
    
    d->fechaModificacion = time(NULL);
    bool exito = actualizarRegistro("doctores.bin", id, d, sizeof(Doctor));
    delete d;
    return exito;
}

// Funciones de historial médico
void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta) {
    ArchivoHeader headerHistoriales = leerHeader("historiales.bin");
    consulta.idConsulta = headerHistoriales.proximoID++;
    consulta.idPaciente = paciente->id;  // Setear paciente
    consulta.fechaRegistro = time(NULL);
    consulta.siguienteConsultaID = -1;
    consulta.eliminado = false;
    
    if (paciente->primerConsultaID == -1) {
        paciente->primerConsultaID = consulta.idConsulta;
    } else {
        // Encontrar la última consulta y enlazar
        int consultaActualID = paciente->primerConsultaID;
        while (consultaActualID != -1) {
            HistorialMedico h;
            int indice = buscarIndiceDeID("historiales.bin", consultaActualID, sizeof(HistorialMedico));
            if (indice != -1 && leerRegistroPorIndice("historiales.bin", indice, &h, sizeof(HistorialMedico))) {
                if (h.siguienteConsultaID == -1) {
                    h.siguienteConsultaID = consulta.idConsulta;
                    actualizarRegistro("historiales.bin", consultaActualID, &h, sizeof(HistorialMedico));
                    break;
                }
                consultaActualID = h.siguienteConsultaID;
            } else {
                break;
            }
        }
    }
    
    agregarRegistro("historiales.bin", &consulta, sizeof(HistorialMedico));
    paciente->cantidadConsultas++;
    actualizarRegistro("pacientes.bin", paciente->id, paciente, sizeof(Paciente));
    // Actualizar header con nuevo proximoID
    actualizarHeader("historiales.bin", headerHistoriales);
}

HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad) {
    if (paciente->cantidadConsultas == 0) {
        *cantidad = 0;
        return NULL;
    }
    
    HistorialMedico* historial = new HistorialMedico[paciente->cantidadConsultas];
    *cantidad = 0;
    int consultaID = paciente->primerConsultaID;
    while (consultaID != -1 && *cantidad < paciente->cantidadConsultas) {
        int indice = buscarIndiceDeID("historiales.bin", consultaID, sizeof(HistorialMedico));
        if (indice != -1) {
            leerRegistroPorIndice("historiales.bin", indice, &historial[*cantidad], sizeof(HistorialMedico));
            consultaID = historial[*cantidad].siguienteConsultaID;
            (*cantidad)++;
        } else {
            break;
        }
    }
    return historial;
}

void mostrarHistorialMedico(Paciente* paciente) {
    int cantidad;
    HistorialMedico* historial = obtenerHistorialCompleto(paciente, &cantidad);
    cout << "Historial Medico de " << paciente->nombre << " " << paciente->apellido << ":" << endl;
    for (int i = 0; i < cantidad; ++i) {
        cout << "Consulta " << historial[i].idConsulta << ": " << historial[i].diagnostico << endl;
    }
    delete[] historial;
}

// Módulo de Gestión de Doctores (similar, implementado de forma análoga)
Doctor* crearDoctor(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, const char* especialidad, int aniosExperiencia, float costoConsulta) {
    ArchivoHeader header = leerHeader("doctores.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d;
        if (leerRegistroPorIndice("doctores.bin", i, &d, sizeof(Doctor)) && !d.eliminado && strcmp(d.cedula, cedula) == 0) {
            cout << "Error: Cedula de doctor ya existe." << endl;
            return NULL;
        }
    }
    
    Doctor nuevo;
    nuevo.id = header.proximoID++;
    actualizarHeader("doctores.bin", header);
    strncpy(nuevo.nombre, nombre, 49);
    nuevo.nombre[49] = '\0';
    strncpy(nuevo.apellido, apellido, 49);
    nuevo.apellido[49] = '\0';
    strncpy(nuevo.cedula, cedula, 19);
    nuevo.cedula[19] = '\0';
    strncpy(nuevo.especialidad, especialidad, 49);
    nuevo.especialidad[49] = '\0';
    nuevo.aniosExperiencia = aniosExperiencia;
    nuevo.costoConsulta = costoConsulta;
    strcpy(nuevo.horarioAtencion, "Lun-Vie 8:00-16:00");
    strcpy(nuevo.telefono, "");
    strcpy(nuevo.email, "");
    nuevo.cantidadPacientes = 0;
    nuevo.cantidadCitas = 0;
    nuevo.disponible = true;
    nuevo.eliminado = false;
    nuevo.fechaModificacion = time(NULL);
    
    if (agregarRegistro("doctores.bin", &nuevo, sizeof(Doctor))) {
        hospital->totalDoctores++;
        return new Doctor(nuevo);
    }
    return NULL;
}

Doctor* buscarDoctorPorId(Hospital* hospital, int id) {
    ArchivoHeader header = leerHeader("doctores.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d;
        if (leerRegistroPorIndice("doctores.bin", i, &d, sizeof(Doctor)) && !d.eliminado && d.id == id) {
            return new Doctor(d);
        }
    }
    return NULL;
}

Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad) {
    ArchivoHeader header = leerHeader("doctores.bin");
    Doctor** resultados = new Doctor*[header.registrosActivos];
    *cantidad = 0;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d;
        if (leerRegistroPorIndice("doctores.bin", i, &d, sizeof(Doctor)) && !d.eliminado && strcmp(d.especialidad, especialidad) == 0) {
            resultados[(*cantidad)++] = new Doctor(d);
        }
    }
    return resultados;
}

bool asignarPacienteADoctor(Doctor* doctor, int idPaciente) {
    if (doctor->cantidadPacientes >= 50) return false;
    for (int i = 0; i < doctor->cantidadPacientes; ++i) {
        if (doctor->pacientesIDs[i] == idPaciente) return false;
    }
    doctor->pacientesIDs[doctor->cantidadPacientes++] = idPaciente;
    doctor->fechaModificacion = time(NULL);
    return actualizarRegistro("doctores.bin", doctor->id, doctor, sizeof(Doctor));
}

void listarPacientesDeDoctor(Hospital* hospital, int idDoctor) {
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (!doctor) {
        cout << "Doctor no encontrado." << endl;
        return;
    }
    cout << "Pacientes asignados al Dr. " << doctor->nombre << " " << doctor->apellido << ":" << endl;
    for (int i = 0; i < doctor->cantidadPacientes; ++i) {
        Paciente* p = buscarPacientePorId(hospital, doctor->pacientesIDs[i]);
        if (p) {
            cout << "ID: " << p->id << " - " << p->nombre << " " << p->apellido << endl;
            delete p;
        }
    }
    delete doctor;
}

void listarDoctores(Hospital* hospital) {
    ArchivoHeader header = leerHeader("doctores.bin");
    cout << "Lista de Doctores:" << endl;
    cout << left << setw(5) << "ID" << setw(20) << "Nombre" << setw(20) << "Apellido" << setw(20) << "Especialidad" << setw(10) << "Costo" << endl;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d;
        if (leerRegistroPorIndice("doctores.bin", i, &d, sizeof(Doctor)) && !d.eliminado) {
            cout << left << setw(5) << d.id << setw(20) << d.nombre << setw(20) << d.apellido << setw(20) << d.especialidad << setw(10) << d.costoConsulta << endl;
        }
    }
}

bool eliminarDoctor(Hospital* hospital, int id) {
    Doctor* d = buscarDoctorPorId(hospital, id);
    if (!d) return false;
    
    // Cancelar citas asociadas
    for (int i = 0; i < d->cantidadCitas; ++i) {
        eliminarRegistroFisico("citas.bin", d->citasIDs[i], sizeof(Cita));
        hospital->totalCitas--;
    }
    
    delete d;
    
    // Eliminar físicamente al doctor
    if (eliminarRegistroFisico("doctores.bin", id, sizeof(Doctor))) {
        hospital->totalDoctores--;
        return true;
    }
    return false;
}

// Módulo de Gestión de Citas (similar)
Cita* agendarCita(Hospital* hospital, int idPaciente, int idDoctor, const char* fecha, const char* hora, const char* motivo) {
    Paciente* paciente = buscarPacientePorId(hospital, idPaciente);
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (!paciente || !doctor) return NULL;
    
    if (!validarFecha(fecha) || !validarHora(hora)) return NULL;
    
    // Verificar disponibilidad (simplificado)
    ArchivoHeader headerCitas = leerHeader("citas.bin");
    for (int i = 0; i < headerCitas.cantidadRegistros; ++i) {
        Cita c;
        if (leerRegistroPorIndice("citas.bin", i, &c, sizeof(Cita)) && !c.eliminado && c.idDoctor == idDoctor && strcmp(c.fecha, fecha) == 0 && strcmp(c.hora, hora) == 0) {
            delete paciente;
            delete doctor;
            return NULL;
        }
    }
    
    Cita nueva;
    nueva.id = headerCitas.proximoID++;
    actualizarHeader("citas.bin", headerCitas);
    nueva.idPaciente = idPaciente;
    nueva.idDoctor = idDoctor;
    strncpy(nueva.fecha, fecha, 10);
    nueva.fecha[10] = '\0';
    strncpy(nueva.hora, hora, 5);
    nueva.hora[5] = '\0';
    strncpy(nueva.motivo, motivo, 149);
    nueva.motivo[149] = '\0';
    strcpy(nueva.estado, "Agendada");
    strcpy(nueva.observaciones, "");
    nueva.atendida = false;
    nueva.eliminado = false;
    nueva.fechaModificacion = time(NULL);
    
    if (agregarRegistro("citas.bin", &nueva, sizeof(Cita))) {
        // Agregar a paciente y doctor
        if (paciente->cantidadCitas < 20) {
            paciente->citasIDs[paciente->cantidadCitas++] = nueva.id;
            actualizarRegistro("pacientes.bin", idPaciente, paciente, sizeof(Paciente));
        }
        if (doctor->cantidadCitas < 50) {
            doctor->citasIDs[doctor->cantidadCitas++] = nueva.id;
            actualizarRegistro("doctores.bin", idDoctor, doctor, sizeof(Doctor));
        }
        hospital->totalCitas++;
        delete paciente;
        delete doctor;
        return new Cita(nueva);
    }
    delete paciente;
    delete doctor;
    return NULL;
}

bool cancelarCita(Hospital* hospital, int idCita) {
    // Primero, obtener la cita para limpiar referencias
    int indice = buscarIndiceDeID("citas.bin", idCita, sizeof(Cita));
    if (indice == -1) return false;
    
    Cita c;
    if (!leerRegistroPorIndice("citas.bin", indice, &c, sizeof(Cita)) || c.eliminado) return false;
    
    // Limpiar referencias en paciente
    Paciente* p = buscarPacientePorId(hospital, c.idPaciente);
    if (p) {
        int nuevaCantidad = 0;
        for (int i = 0; i < p->cantidadCitas; ++i) {
            if (p->citasIDs[i] != idCita) {
                p->citasIDs[nuevaCantidad++] = p->citasIDs[i];
            }
        }
        p->cantidadCitas = nuevaCantidad;
        actualizarRegistro("pacientes.bin", p->id, p, sizeof(Paciente));
        delete p;
    }
    
    // Limpiar referencias en doctor
    Doctor* d = buscarDoctorPorId(hospital, c.idDoctor);
    if (d) {
        int nuevaCantidad = 0;
        for (int i = 0; i < d->cantidadCitas; ++i) {
            if (d->citasIDs[i] != idCita) {
                d->citasIDs[nuevaCantidad++] = d->citasIDs[i];
            }
        }
        d->cantidadCitas = nuevaCantidad;
        actualizarRegistro("doctores.bin", d->id, d, sizeof(Doctor));
        delete d;
    }
    
    // Eliminar físicamente la cita
    if (eliminarRegistroFisico("citas.bin", idCita, sizeof(Cita))) {
        hospital->totalCitas--;
        return true;
    }
    return false;
}

bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico, const char* tratamiento, const char* medicamentos) {
    int indice = buscarIndiceDeID("citas.bin", idCita, sizeof(Cita));
    if (indice == -1) return false;
    
    Cita c;
    if (!leerRegistroPorIndice("citas.bin", indice, &c, sizeof(Cita)) || c.eliminado || strcmp(c.estado, "Agendada") != 0) return false;
    
    strcpy(c.estado, "Atendida");
    c.atendida = true;
    c.fechaModificacion = time(NULL);
    
    ArchivoHeader headerHistoriales = leerHeader("historiales.bin");
    HistorialMedico consulta;
    consulta.idConsulta = headerHistoriales.proximoID++;
    strcpy(consulta.fecha, c.fecha);
    strcpy(consulta.hora, c.hora);
    strncpy(consulta.diagnostico, diagnostico, 199);
    consulta.diagnostico[199] = '\0';
    strncpy(consulta.tratamiento, tratamiento, 199);
    consulta.tratamiento[199] = '\0';
    strncpy(consulta.medicamentos, medicamentos, 149);
    consulta.medicamentos[149] = '\0';
    consulta.idDoctor = c.idDoctor;
    consulta.costo = 0;  // Buscar costo del doctor
    consulta.siguienteConsultaID = -1;
    consulta.fechaRegistro = time(NULL);
    
    Paciente* p = buscarPacientePorId(hospital, c.idPaciente);
    if (p) {
        agregarConsultaAlHistorial(p, consulta);
        delete p;
    }
    
    actualizarHeader("historiales.bin", headerHistoriales);
    return actualizarRegistro("citas.bin", idCita, &c, sizeof(Cita));
}

Cita** obtenerCitasDePaciente(Hospital* hospital, int idPaciente, int* cantidad) {
    Paciente* p = buscarPacientePorId(hospital, idPaciente);
    if (!p || p->cantidadCitas == 0) {
        *cantidad = 0;
        if (p) delete p;
        return NULL;
    }
    
    Cita** citas = new Cita*[p->cantidadCitas];
    *cantidad = 0;
    for (int i = 0; i < p->cantidadCitas; ++i) {
        int indice = buscarIndiceDeID("citas.bin", p->citasIDs[i], sizeof(Cita));
        if (indice != -1) {
            citas[*cantidad] = new Cita;
            leerRegistroPorIndice("citas.bin", indice, citas[*cantidad], sizeof(Cita));
            (*cantidad)++;
        }
    }
    delete p;
    return citas;
}

Cita** obtenerCitasDeDoctor(Hospital* hospital, int idDoctor, int* cantidad) {
    Doctor* d = buscarDoctorPorId(hospital, idDoctor);
    if (!d || d->cantidadCitas == 0) {
        *cantidad = 0;
        if (d) delete d;
        return NULL;
    }
    
    Cita** citas = new Cita*[d->cantidadCitas];
    *cantidad = 0;
    for (int i = 0; i < d->cantidadCitas; ++i) {
        int indice = buscarIndiceDeID("citas.bin", d->citasIDs[i], sizeof(Cita));
        if (indice != -1) {
            citas[*cantidad] = new Cita;
            leerRegistroPorIndice("citas.bin", indice, citas[*cantidad], sizeof(Cita));
            (*cantidad)++;
        }
    }
    delete d;
    return citas;
}

Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad) {
    ArchivoHeader header = leerHeader("citas.bin");
    Cita** resultados = new Cita*[header.registrosActivos];
    *cantidad = 0;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Cita c;
        if (leerRegistroPorIndice("citas.bin", i, &c, sizeof(Cita)) && !c.eliminado && strcmp(c.fecha, fecha) == 0) {
            resultados[(*cantidad)++] = new Cita(c);
        }
    }
    return resultados;
}

void listarCitasPendientes(Hospital* hospital) {
    ArchivoHeader header = leerHeader("citas.bin");
    cout << "Citas Pendientes:" << endl;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Cita c;
        if (leerRegistroPorIndice("citas.bin", i, &c, sizeof(Cita)) && !c.eliminado && strcmp(c.estado, "Agendada") == 0) {
            cout << "ID: " << c.id << " - Paciente: " << c.idPaciente << " - Doctor: " << c.idDoctor << " - Fecha: " << c.fecha << endl;
        }
    }
}

// Funciones de Validación (iguales)
bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    int year = stringToInt(fecha);
    int month = stringToInt(fecha + 5);
    int day = stringToInt(fecha + 8);
    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) daysInMonth[1] = 29;
    if (day < 1 || day > daysInMonth[month - 1]) return false;
    return true;
}

bool validarHora(const char* hora) {
    if (strlen(hora) != 5) return false;
    if (hora[2] != ':') return false;
    int hh = stringToInt(hora);
    int mm = stringToInt(hora + 3);
    if (hh < 0 || hh > 23) return false;
    if (mm < 0 || mm > 59) return false;
    return true;
}

int compararFechas(const char* fecha1, const char* fecha2) {
    return strcmp(fecha1, fecha2);
}

bool validarCedula(const char* cedula) {
    if (strlen(cedula) == 0 || strlen(cedula) > 19) return false;
    return true;
}

bool validarEmail(const char* email) {
    const char* at = strchr(email, '@');
    const char* dot = strrchr(email, '.');
    return at && dot && at < dot;
}

char* copiarString(const char* origen) {
    if (!origen) return NULL;
    size_t len = strlen(origen);
    char* copia = new char[len + 1];
    strcpy(copia, origen);
    return copia;
}

Paciente* copiarPaciente(Paciente* original) {
    if (!original) return NULL;
    Paciente* copia = new Paciente;
    *copia = *original;
    return copia;
}

// Menú principal
int main() {
    Hospital* hospital = inicializarHospital("Hospital Central");
    int opcion;
    
    do {
        cout << " _______________________________________" << endl;
        cout << "|   SISTEMA DE GESTION HOSPITALARIA v3   |" << endl;
        cout << " ---------------------------------------" << endl;
        cout << "1. Gestion de Pacientes" << endl;
        cout << "2. Gestion de Doctores" << endl;
        cout << "3. Gestion de Citas" << endl;
        cout << "4. Consultas y Reportes" << endl;
        cout << "5. Mantenimiento de Archivos" << endl;
        cout << "0. Guardar y Salir" << endl;
        cout << "\nOpcion: ";
        cin >> opcion;
        cin.ignore();
        
        switch (opcion) {
            case 1: {
                int subOpcion;
                do {
                    cout << "\n--- Gestion de Pacientes ---" << endl;
                    cout << "1. Registrar nuevo paciente" << endl;
                    cout << "2. Buscar paciente por cedula" << endl;
                    cout << "3. Buscar paciente por nombre" << endl;
                    cout << "4. Ver historial medico completo" << endl;
                    cout << "5. Actualizar datos del paciente" << endl;
                    cout << "6. Listar todos los pacientes" << endl;
                    cout << "7. Eliminar paciente" << endl;
                    cout << "0. Volver al menu principal" << endl;
                    cout << "\nOpcion: ";
                    cin >> subOpcion;
                    cin.ignore();
                    
                    if (subOpcion == 1) {
                        char nombre[50], apellido[50], cedula[20];
                        int edad;
                        char sexo;
                        cout << "Nombre: "; cin.getline(nombre, 50);
                        cout << "Apellido: "; cin.getline(apellido, 50);
                        cout << "Cedula: "; cin.getline(cedula, 20);
                        cout << "Edad: "; cin >> edad;
                        cout << "Sexo (M/F): "; cin >> sexo;
                        cin.ignore();
                        crearPaciente(hospital, nombre, apellido, cedula, edad, sexo);
                    } else if (subOpcion == 2) {
                        char cedula[20];
                        cout << "Cedula: "; cin.getline(cedula, 20);
                        Paciente* p = buscarPacientePorCedula(hospital, cedula);
                        if (p) {
                            cout << "Encontrado: " << p->nombre << " " << p->apellido << endl;
                            delete p;
                        } else cout << "No encontrado." << endl;
                    } else if (subOpcion == 3) {
                        char nombre[50];
                        cout << "Nombre: "; cin.getline(nombre, 50);
                        int cantidad;
                        Paciente** pacientes = buscarPacientesPorNombre(hospital, nombre, &cantidad);
                        if (cantidad > 0) {
                            for (int i = 0; i < cantidad; ++i) {
                                cout << "ID: " << pacientes[i]->id << " - " << pacientes[i]->nombre << " " << pacientes[i]->apellido << endl;
                                delete pacientes[i];
                            }
                            delete[] pacientes;
                        } else cout << "No encontrados." << endl;
                    } else if (subOpcion == 4) {
                        int id;
                        cout << "ID Paciente: "; cin >> id;
                        cin.ignore();
                        Paciente* p = buscarPacientePorId(hospital, id);
                        if (p) {
                            mostrarHistorialMedico(p);
                            delete p;
                        } else cout << "Paciente no encontrado." << endl;
                    } else if (subOpcion == 5) {
                        int id;
                        cout << "ID Paciente: "; cin >> id;
                        cin.ignore();
                        actualizarPaciente(hospital, id);
                    } else if (subOpcion == 6) {
                        listarTodosPacientes();
                    } else if (subOpcion == 7) {
                        int id;
                        cout << "ID Paciente: "; cin >> id;
                        cin.ignore();
                        eliminarPaciente(hospital, id);
                    } else if (subOpcion != 0) {
                        cout << "Opcion invalida." << endl;
                    }
                } while (subOpcion != 0);
                break;
            }
            case 2: {
                int subOpcion;
                do {
                    cout << "\n--- Gestion de Doctores ---" << endl;
                    cout << "1. Registrar nuevo doctor" << endl;
                    cout << "2. Buscar doctor por ID" << endl;
                    cout << "3. Buscar doctores por especialidad" << endl;
                    cout << "4. Asignar paciente a doctor" << endl;
                    cout << "5. Ver pacientes asignados a doctor" << endl;
                    cout << "6. Listar todos los doctores" << endl;
                    cout << "7. Actualizar datos del doctor" << endl;
                    cout << "8. Eliminar doctor" << endl;
                    cout << "0. Volver" << endl;
                    cout << "\nOpcion: ";
                    cin >> subOpcion;
                    cin.ignore();
                    
                    if (subOpcion == 1) {
                        char nombre[50], apellido[50], cedula[20], especialidad[50];
                        int anios;
                        float costo;
                        cout << "Nombre: "; cin.getline(nombre, 50);
                        cout << "Apellido: "; cin.getline(apellido, 50);
                        cout << "Cedula: "; cin.getline(cedula, 20);
                        cout << "Especialidad: "; cin.getline(especialidad, 50);
                        cout << "Anos experiencia: "; cin >> anios;
                        cout << "Costo consulta: "; cin >> costo;
                        cin.ignore();
                        crearDoctor(hospital, nombre, apellido, cedula, especialidad, anios, costo);
                    } else if (subOpcion == 2) {
                        int id;
                        cout << "ID Doctor: "; cin >> id;
                        cin.ignore();
                        Doctor* d = buscarDoctorPorId(hospital, id);
                        if (d) {
                            cout << "Encontrado: " << d->nombre << " " << d->apellido << endl;
                            delete d;
                        } else cout << "No encontrado." << endl;
                    } else if (subOpcion == 3) {
                        char especialidad[50];
                        cout << "Especialidad: "; cin.getline(especialidad, 50);
                        int cantidad;
                        Doctor** doctores = buscarDoctoresPorEspecialidad(hospital, especialidad, &cantidad);
                        if (cantidad > 0) {
                            for (int i = 0; i < cantidad; ++i) {
                                cout << "ID: " << doctores[i]->id << " - " << doctores[i]->nombre << " " << doctores[i]->apellido << endl;
                                delete doctores[i];
                            }
                            delete[] doctores;
                        } else cout << "No encontrados." << endl;
                    } else if (subOpcion == 4) {
                        int idDoctor, idPaciente;
                        cout << "ID Doctor: "; cin >> idDoctor;
                        cout << "ID Paciente: "; cin >> idPaciente;
                        cin.ignore();
                        Doctor* d = buscarDoctorPorId(hospital, idDoctor);
                        if (d) {
                            asignarPacienteADoctor(d, idPaciente);
                            delete d;
                        }
                    } else if (subOpcion == 5) {
                        int idDoctor;
                        cout << "ID Doctor: "; cin >> idDoctor;
                        cin.ignore();
                        listarPacientesDeDoctor(hospital, idDoctor);
                    } else if (subOpcion == 6) {
                        listarDoctores(hospital);
                    } else if (subOpcion == 7) {
                        int id;
                        cout << "ID Doctor: "; cin >> id;
                        cin.ignore();
                        actualizarDoctor(hospital, id);
                    } else if (subOpcion == 8) {
                        int id;
                        cout << "ID Doctor: "; cin >> id;
                        cin.ignore();
                        eliminarDoctor(hospital, id);
                    } else if (subOpcion != 0) {
                        cout << "Opcion invalida." << endl;
                    }
                } while (subOpcion != 0);
                break;
            }
            case 3: {
                int subOpcion;
                do {
                    cout << "\n--- Gestion de Citas ---" << endl;
                    cout << "1. Agendar nueva cita" << endl;
                    cout << "2. Cancelar cita" << endl;
                    cout << "3. Atender cita" << endl;
                    cout << "4. Ver citas de un paciente" << endl;
                    cout << "5. Ver citas de un doctor" << endl;
                    cout << "6. Ver citas de una fecha" << endl;
                    cout << "7. Ver citas pendientes" << endl;
                    cout << "0. Volver" << endl;
                    cout << "\nOpcion: ";
                    cin >> subOpcion;
                    cin.ignore();
                    
                    if (subOpcion == 1) {
                        int idP, idD;
                        char fecha[11], hora[6], motivo[150];
                        cout << "ID Paciente: "; cin >> idP;
                        cout << "ID Doctor: "; cin >> idD;
                        cin.ignore();
                        cout << "Fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                        cout << "Hora (HH:MM): "; cin.getline(hora, 6);
                        cout << "Motivo: "; cin.getline(motivo, 150);
                        agendarCita(hospital, idP, idD, fecha, hora, motivo);
                    } else if (subOpcion == 2) {
                        int idCita;
                        cout << "ID Cita: "; cin >> idCita;
                        cin.ignore();
                        cancelarCita(hospital, idCita);
                    } else if (subOpcion == 3) {
                        int idCita;
                        char diagnostico[200], tratamiento[200], medicamentos[150];
                        cout << "ID Cita: "; cin >> idCita;
                        cin.ignore();
                        cout << "Diagnostico: "; cin.getline(diagnostico, 200);
                        cout << "Tratamiento: "; cin.getline(tratamiento, 200);
                        cout << "Medicamentos: "; cin.getline(medicamentos, 150);
                        atenderCita(hospital, idCita, diagnostico, tratamiento, medicamentos);
                    } else if (subOpcion == 4) {
                        int idPaciente, cantidad;
                        cout << "ID Paciente: "; cin >> idPaciente;
                        cin.ignore();
                        Cita** citas = obtenerCitasDePaciente(hospital, idPaciente, &cantidad);
                        if (cantidad > 0) {
                            for (int i = 0; i < cantidad; ++i) {
                                cout << "Cita ID: " << citas[i]->id << " - Fecha: " << citas[i]->fecha << " - Estado: " << citas[i]->estado << endl;
                                delete citas[i];
                            }
                            delete[] citas;
                        } else cout << "No hay citas." << endl;
                    } else if (subOpcion == 5) {
                        int idDoctor, cantidad;
                        cout << "ID Doctor: "; cin >> idDoctor;
                        cin.ignore();
                        Cita** citas = obtenerCitasDeDoctor(hospital, idDoctor, &cantidad);
                        if (cantidad > 0) {
                            cout << "Citas del Doctor:" << endl;
                            for (int i = 0; i < cantidad; ++i) {
                                cout << "Cita ID: " << citas[i]->id << " - Fecha: " << citas[i]->fecha << " - Estado: " << citas[i]->estado << endl;
                                delete citas[i];
                            }
                            delete[] citas;
                        } else cout << "No hay citas." << endl;
                    } else if (subOpcion == 6) {
                        char fecha[11];
                        cout << "Fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                        int cantidad;
                        Cita** citas = obtenerCitasPorFecha(hospital, fecha, &cantidad);
                        if (cantidad > 0) {
                            cout << "Citas en " << fecha << ":" << endl;
                            for (int i = 0; i < cantidad; ++i) {
                                cout << "ID: " << citas[i]->id << " - Paciente: " << citas[i]->idPaciente << " - Doctor: " << citas[i]->idDoctor << " - Estado: " << citas[i]->estado << endl;
                                delete citas[i];
                            }
                            delete[] citas;
                        } else cout << "No hay citas en esa fecha." << endl;
                    } else if (subOpcion == 7) {
                        listarCitasPendientes(hospital);
                    } else if (subOpcion != 0) {
                        cout << "Opcion invalida." << endl;
                    }
                } while (subOpcion != 0);
                break;
            }
            case 4: {
                int subOpcion;
                do {
                    cout << "\n--- Consultas y Reportes ---" << endl;
                    cout << "1. Estadisticas generales" << endl;
                    cout << "2. Reporte de pacientes por doctor" << endl;
                    cout << "3. Reporte de citas por fecha" << endl;
                    cout << "0. Volver" << endl;
                    cout << "\nOpcion: ";
                    cin >> subOpcion;
                    cin.ignore();
                    
                    if (subOpcion == 1) {
                        cout << "Estadisticas del Hospital:" << endl;
                        cout << "Total Pacientes: " << hospital->totalPacientes << endl;
                        cout << "Total Doctores: " << hospital->totalDoctores << endl;
                        cout << "Total Citas: " << hospital->totalCitas << endl;
                        cout << "Total Consultas: " << hospital->totalConsultasRealizadas << endl;
                    } else if (subOpcion == 2) {
                        int idDoctor;
                        cout << "ID Doctor: "; cin >> idDoctor;
                        cin.ignore();
                        listarPacientesDeDoctor(hospital, idDoctor);
                    } else if (subOpcion == 3) {
                        char fecha[11];
                        cout << "Fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                        int cantidad;
                        Cita** citas = obtenerCitasPorFecha(hospital, fecha, &cantidad);
                        if (cantidad > 0) {
                            cout << "Citas en " << fecha << ":" << endl;
                            for (int i = 0; i < cantidad; ++i) {
                                cout << "ID: " << citas[i]->id << " - Paciente: " << citas[i]->idPaciente << " - Doctor: " << citas[i]->idDoctor << " - Estado: " << citas[i]->estado << endl;
                                delete citas[i];
                            }
                            delete[] citas;
                        } else cout << "No hay citas en esa fecha." << endl;
                    } else if (subOpcion != 0) {
                        cout << "Opcion invalida." << endl;
                    }
                } while (subOpcion != 0);
                break;
            }
            case 5: {
                int subOpcion;
                do {
                    cout << "\n--- Mantenimiento de Archivos ---" << endl;
                    cout << "1. Verificar integridad de archivos" << endl;
                    cout << "2. Compactar archivos (eliminar registros borrados)" << endl;
                    cout << "3. Hacer respaldo de datos" << endl;
                    cout << "4. Restaurar desde respaldo" << endl;
                    cout << "5. Estadisticas de uso de archivos" << endl;
                    cout << "6. Eliminar todos los pacientes" << endl;
                    cout << "7. Eliminar todos los doctores" << endl;
                    cout << "0. Volver al menu principal" << endl;
                    cout << "\nOpcion: ";
                    cin >> subOpcion;
                    cin.ignore();
                    
                    if (subOpcion == 1) {
                        verificarIntegridadReferencial();
                    } else if (subOpcion == 2) {
                        compactarArchivoPacientes();
                        // Compactar otros si se implementa
                    } else if (subOpcion == 3) {
                        hacerRespaldo();
                    } else if (subOpcion == 4) {
                        restaurarRespaldo();
                    } else if (subOpcion == 5) {
                        estadisticasDeArchivos();
                    } else if (subOpcion == 6) {
                        eliminarTodosPacientes(hospital);
                    } else if (subOpcion == 7) {
                        eliminarTodosDoctores(hospital);
                    } else if (subOpcion != 0) {
                        cout << "Opcion invalida." << endl;
                    }
                } while (subOpcion != 0);
                break;
            }
            case 0: {
                guardarDatosHospital(hospital);
                cout << "Datos guardados. Saliendo..." << endl;
                break;
            }
            default:
                if (opcion != 0) cout << "Opcion invalida." << endl;
        }
    } while (opcion != 0);
    
    destruirHospital(hospital);
    return 0;
}