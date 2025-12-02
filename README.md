# Hospital-P3

## Descripción

Este proyecto es un sistema de gestión hospitalaria desarrollado en C++ que permite administrar pacientes, doctores, citas médicas e historiales médicos. Utiliza archivos binarios para la persistencia de datos, implementando operaciones CRUD (Crear, Leer, Actualizar, Eliminar) con borrado lógico y físico.

## Características Principales

### Gestión de Pacientes

- Crear, buscar, actualizar y eliminar pacientes
- Búsqueda por cédula, ID o nombre
- Validación de unicidad de cédula
- Historial médico completo por paciente

### Gestión de Doctores

- Crear, buscar, actualizar y eliminar doctores
- Búsqueda por especialidad
- Asignación de pacientes a doctores
- Gestión de disponibilidad

### Gestión de Citas

- Agendar citas médicas
- Cancelar y atender citas
- Obtener citas por paciente, doctor o fecha
- Estados de citas (pendiente, atendida)

### Historial Médico

- Registro de consultas médicas
- Diagnósticos, tratamientos y medicamentos
- Lista enlazada de consultas por paciente
- Costos de consultas

### Persistencia de Datos

- Archivos binarios con headers para metadata
- Operaciones de respaldo y restauración
- Compactación de archivos
- Verificación de integridad referencial

### Utilidades

- Estadísticas de archivos
- Validaciones de datos (fechas, horas, emails, cédulas)
- Funciones de búsqueda case-insensitive

## Estructura del Proyecto

- `hospital.cpp`: Archivo principal con toda la implementación
- `hospital.bin`: Datos del hospital
- `pacientes.bin`: Archivo de pacientes
- `doctores.bin`: Archivo de doctores
- `citas.bin`: Archivo de citas
- `historiales.bin`: Archivo de historiales médicos

## Compilación y Ejecución

Para compilar el proyecto:

```bash
g++ hospital.cpp -o hospital
```

Para ejecutar:

```bash
./hospital
```

## Dependencias

- Compilador C++ (g++ recomendado)
- Librerías estándar de C++ (iostream, cstring, ctime, etc.)

## Autor

- **alfretroco-rgb** - Desarrollador del proyecto

## Notas

Este proyecto implementa un sistema completo de gestión hospitalaria con persistencia en archivos binarios, validaciones de datos y operaciones avanzadas como compactación y respaldo de datos.
