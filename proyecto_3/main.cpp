#include "persistencia/GestorArchivos.hpp"
#include "pacientes/operacionesPacientes.hpp"
#include "doctores/operacionesDoctores.hpp"
#include "citas/operacionesCitas.hpp"
#include "historial/operacionesHistorial.hpp"
#include "utilidades/Validaciones.hpp"
#include <iostream>
using namespace std;

void menuPacientes() {
    int op;
    do {
        cout << "\n=== GESTION DE PACIENTES ===" << endl;
        cout << "1. Registrar paciente" << endl;
        cout << "2. Buscar por ID" << endl;
        cout << "3. Buscar por cedula" << endl;
        cout << "4. Actualizar paciente" << endl;
        cout << "5. Eliminar paciente" << endl;
        cout << "6. Listar todos" << endl;
        cout << "0. Volver" << endl;
        cout << "Opcion: ";
        cin >> op;
        cin.ignore();
        switch (op) {
            case 1: registrarPaciente(); break;
            case 2: buscarPacientePorID(); break;
            case 3: buscarPacientePorCedula(); break;
            case 4: actualizarPaciente(); break;
            case 5: eliminarPaciente(); break;
            case 6: listarTodosPacientes(); break;
            case 0: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (op != 0);
}

void menuDoctores() {
    int op;
    do {
        cout << "\n=== GESTION DE DOCTORES ===" << endl;
        cout << "1. Registrar doctor" << endl;
        cout << "2. Buscar por ID" << endl;
        cout << "3. Listar por especialidad" << endl;
        cout << "4. Actualizar doctor" << endl;
        cout << "5. Eliminar doctor" << endl;
        cout << "6. Listar todos" << endl;
        cout << "0. Volver" << endl;
        cout << "Opcion: ";
        cin >> op;
        cin.ignore();
        switch (op) {
            case 1: registrarDoctor(); break;
            case 2: buscarDoctorPorID(); break;
            case 3: listarDoctoresPorEspecialidad(); break;
            case 4: actualizarDoctor(); break;
            case 5: eliminarDoctor(); break;
            case 6: listarDoctores(); break;
            case 0: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (op != 0);
}

void menuCitas() {
    int op;
    do {
        cout << "\n=== GESTION DE CITAS ===" << endl;
        cout << "1. Agendar cita" << endl;
        cout << "2. Buscar por ID" << endl;
        cout << "3. Listar pendientes" << endl;
        cout << "4. Atender cita" << endl;
        cout << "5. Cancelar cita" << endl;
        cout << "0. Volver" << endl;
        cout << "Opcion: ";
        cin >> op;
        cin.ignore();
        switch (op) {
            case 1: agendarCita(); break;
            case 2: buscarCitaPorID(); break;
            case 3: listarCitasPendientes(); break;
            case 4: atenderCita(); break;
            case 5: cancelarCita(); break;
            case 0: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (op != 0);
}

void menuHistorial() {
    int op;
    do {
        cout << "\n=== GESTION DE HISTORIAL MEDICO ===" << endl;
        cout << "1. Agregar consulta" << endl;
        cout << "2. Mostrar historial de paciente" << endl;
        cout << "0. Volver" << endl;
        cout << "Opcion: ";
        cin >> op;
        cin.ignore();
        switch (op) {
            case 1: agregarConsulta(); break;
            case 2: mostrarHistorialPaciente(); break;
            case 0: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (op != 0);
}

void menuUtilidades() {
    int op;
    do {
        cout << "\n=== UTILIDADES DEL SISTEMA ===" << endl;
        cout << "1. Estadisticas de archivos" << endl;
        cout << "2. Verificar integridad" << endl;
        cout << "3. Hacer respaldo" << endl;
        cout << "4. Restaurar respaldo" << endl;
        cout << "5. Compactar archivos" << endl;
        cout << "0. Volver" << endl;
        cout << "Opcion: ";
        cin >> op;
        cin.ignore();
        switch (op) {
            case 1: Validaciones::estadisticasDeArchivos(); break;
            case 2: GestorArchivos::verificarIntegridadReferencial(); break;
            case 3: GestorArchivos::hacerRespaldo(); break;
            case 4: GestorArchivos::restaurarRespaldo(); break;
            case 5: GestorArchivos::compactarArchivos(); break;
            case 0: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (op != 0);
}

int main() {
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        cerr << "Error al inicializar archivos." << endl;
        return 1;
    }

    int opcion;
    do {
        cout << "\n=== SISTEMA DE GESTION HOSPITALARIA ===" << endl;
        cout << "1. Gestion de Pacientes" << endl;
        cout << "2. Gestion de Doctores" << endl;
        cout << "3. Gestion de Citas" << endl;
        cout << "4. Gestion de Historial Medico" << endl;
        cout << "5. Utilidades del Sistema" << endl;
        cout << "0. Salir" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();
        switch (opcion) {
            case 1: menuPacientes(); break;
            case 2: menuDoctores(); break;
            case 3: menuCitas(); break;
            case 4: menuHistorial(); break;
            case 5: menuUtilidades(); break;
            case 0: break;
            default: cout << "Opcion invalida." << endl;
        }
    } while (opcion != 0);

    return 0;
}