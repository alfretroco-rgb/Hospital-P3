# Sistema de Gestión Hospitalaria - Proyecto P3

## 📋 Descripción

Este proyecto es una implementación completa de un **sistema de gestión hospitalaria** desarrollado en **C++** utilizando principios de **Programación Orientada a Objetos (POO)** y arquitectura modular. El sistema permite administrar de manera eficiente pacientes, doctores, citas médicas e historiales médicos, con persistencia de datos en archivos binarios.

El proyecto evoluciona desde una implementación monolítica a una estructura modular organizada por dominios/servicios, facilitando el mantenimiento, escalabilidad y reutilización del código.

## 🎯 Objetivos del Proyecto

- Aplicar POO en C++ con encapsulamiento, herencia y polimorfismo.
- Modularizar el código en componentes independientes por responsabilidad.
- Implementar operaciones CRUD completas con persistencia binaria.
- Validar datos y mantener integridad referencial.
- Proporcionar una interfaz de usuario basada en menús para todas las operaciones.

## ✨ Características Principales

### 👥 Gestión de Pacientes
- **Registro completo**: Nombre, apellido, cédula, edad, sexo, tipo de sangre, contacto.
- **Búsqueda avanzada**: Por ID, cédula o nombre.
- **Validaciones**: Unicidad de cédula, formato de email, edad válida.
- **Relaciones**: Asociación con citas y consultas médicas.
- **Operaciones**: Crear, leer, actualizar, eliminar (CRUD) con borrado lógico.

### 👨‍⚕️ Gestión de Doctores
- **Perfil profesional**: Nombre, especialidad, años de experiencia, costo de consulta.
- **Búsqueda**: Por ID o especialidad.
- **Validaciones**: Datos obligatorios y formatos correctos.
- **Asociaciones**: Vinculación con pacientes a través de citas.

### 📅 Gestión de Citas Médicas
- **Agendamiento**: Fecha, hora, motivo, paciente y doctor asignado.
- **Estados**: Pendiente, Atendida, Cancelada.
- **Operaciones**: Agendar, buscar, atender, cancelar.
- **Validaciones**: Disponibilidad de horarios, existencia de paciente/doctor.

### 📋 Historial Médico
- **Registro de consultas**: Diagnóstico, tratamiento, medicamentos, costo.
- **Seguimiento**: Historial completo por paciente.
- **Enlace de consultas**: Sistema de referencias entre consultas.
- **Búsqueda**: Por paciente o consulta específica.

### 💾 Persistencia de Datos
- **Archivos binarios**: Almacenamiento eficiente con headers de metadata.
- **Operaciones avanzadas**: Compactación, respaldo, restauración.
- **Integridad**: Verificación de consistencia de datos.
- **Acceso aleatorio**: Lectura/escritura directa por posición.

### 🛠️ Utilidades y Validaciones
- **Validaciones robustas**: Fechas, horas, emails, cédulas venezolanas.
- **Estadísticas**: Reportes de archivos y uso del sistema.
- **Búsqueda inteligente**: Case-insensitive y por múltiples criterios.

## 🏗️ Arquitectura y Diseño POO

### Principios Aplicados
- **Encapsulamiento**: Atributos privados, acceso controlado vía getters/setters.
- **Modularización**: Separación por dominios (pacientes, doctores, etc.).
- **Separación de responsabilidades**: .hpp para declaraciones, .cpp para implementaciones.
- **Abstracción**: Clases representan entidades del mundo real.
- **Reutilización**: Componentes independientes y combinables.

### Estructura Modular

```
ProyectoHospital/
│
├── hospital/                   # Módulo principal del hospital
│   ├── Hospital.hpp
│   └── Hospital.cpp
│
├── pacientes/                  # Dominio de pacientes
│   ├── Paciente.hpp           # Declaración de clase Paciente
│   ├── Paciente.cpp           # Implementación de métodos
│   ├── operacionesPacientes.hpp  # Declaraciones de operaciones
│   └── operacionesPacientes.cpp  # Lógica de negocio pacientes
│
├── doctores/                   # Dominio de doctores
│   ├── Doctor.hpp
│   ├── Doctor.cpp
│   ├── operacionesDoctores.hpp
│   └── operacionesDoctores.cpp
│
├── citas/                      # Dominio de citas médicas
│   ├── Cita.hpp
│   ├── Cita.cpp
│   ├── operacionesCitas.hpp
│   └── operacionesCitas.cpp
│
├── historial/                  # Dominio de historial médico
│   ├── HistorialMedico.hpp
│   ├── HistorialMedico.cpp
│   ├── operacionesHistorial.hpp
│   └── operacionesHistorial.cpp
│
├── persistencia/               # Capa de persistencia
│   ├── GestorArchivos.hpp     # Gestión de archivos binarios
│   ├── GestorArchivos.cpp
│   └── Constantes.hpp         # Definiciones de rutas
│
├── utilidades/                 # Utilidades transversales
│   ├── Validaciones.hpp       # Validaciones de datos
│   ├── Validaciones.cpp
│   ├── Formatos.hpp           # (Opcional) Formateo de salida
│   └── Formatos.cpp
│
├── main_temp.cpp               # Archivos temporales de prueba
├── main_temp2.cpp
├── hospital.cpp                # Archivo principal monolítico (legacy)
│
├── datos/                      # Directorio de datos (generado)
│   ├── hospital.bin
│   ├── pacientes.bin
│   ├── doctores.bin
│   ├── citas.bin
│   └── historiales.bin
│
├── Makefile                    # Automatización de compilación
├── CMakeLists.txt              # Configuración CMake
├── compilar.ps1                # Script PowerShell
└── README.md                   # Esta documentación
```

### Clases Principales

#### Paciente
```cpp
class Paciente {
private:
    int id;
    char nombre[50], apellido[50], cedula[20];
    int edad;
    char sexo, tipoSangre[5];
    // ... otros atributos

public:
    // Constructores
    Paciente();
    Paciente(int id, const char* nombre, const char* apellido, const char* cedula);
    Paciente(const Paciente& other);
    ~Paciente();

    // Getters y Setters
    int getId() const;
    void setNombre(const char* nombre);
    // ... más getters/setters

    // Métodos específicos
    bool validarDatos() const;
    bool esMayorDeEdad() const;
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static size_t obtenerTamano();
};
```

#### GestorArchivos (Persistencia)
- **Inicialización**: Crea archivos binarios con headers.
- **Templates genéricos**: Para operaciones CRUD en cualquier entidad.
- **Búsqueda eficiente**: Por ID o índice.
- **Mantenimiento**: Compactación y respaldo de datos.

## 🚀 Instalación y Compilación

### Prerrequisitos
- **Compilador C++**: g++ (MinGW en Windows), clang++, o MSVC.
- **Sistema operativo**: Windows, Linux, macOS.
- **Espacio**: ~50MB para código + datos.

### Instalación del Compilador

#### Windows (Chocolatey)
```powershell
choco install mingw -y
```

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install g++
```

#### macOS
```bash
xcode-select --install
```

### Compilación

#### Opción 1: Comando Manual
```bash
g++ hospital.cpp \
    pacientes/Paciente.cpp pacientes/operacionesPacientes.cpp \
    doctores/Doctor.cpp doctores/operacionesDoctores.cpp \
    citas/Cita.cpp citas/operacionesCitas.cpp \
    historial/HistorialMedico.cpp historial/operacionesHistorial.cpp \
    persistencia/GestorArchivos.cpp utilidades/Validaciones.cpp \
    -o hospital
```

#### Opción 2: Makefile
