#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

// Enumeraciones
enum class Bando { POLICIA_HONESTO, LADRON, CORRUPTO };
enum class Estado { ACTIVO, CAPTURADO, INHABILITADO };
enum class TipoPersonaje {
    INSPECTOR_JEFE, PERITO_FORENSE, OFICIAL_ASALTO, 
    ANALISTA_DATOS, NEGOCIADOR, POLICIA_CORRUPTO,
    CEREBRO, FANTASMA, MULO, SABOTEADOR, HACKER, LOBERO
};
enum class TipoItem {
    SCANNER_METALES, KIT_HUELLAS, GANAS_MANOS, ORDEN_ALLOGRAMIENTO,
    SENUELO_COMUNICACIONES, CHALECO_ANTIBALAS, LLAVES_MANIPULADAS,
    INTERFAZ_SEGURIDAD, FONDO_NEGRO, GRABADORA_ENCUBIERTA
};

// Estructura para habilidades
struct Habilidad {
    string nombre;
    bool disponible;
    int cooldown;
    int usosRestantes;
};

// Estructura para ítems/implementos
struct Item {
    int id;
    string nombre;
    TipoItem tipo;
    int durabilidadMax;
    int usosRestantes;
    string efecto;
    Item* siguiente;
};

// Nodo para lista de items
struct NodoItem {
    Item* item;
    NodoItem* siguiente;
};

// Estructura para personaje
struct Personaje {
    int id;
    string nombre;
    TipoPersonaje tipo;
    Bando bando;
    int posicion; // ID de la estación actual
    Estado estado;
    int puntosVida;
    int cargaOro; // Cantidad de oro que lleva
    int capacidadOro; // Capacidad máxima de oro
    
    // Habilidades
    Habilidad habilidades[3];
    
    // Inventario (lista de items)
    NodoItem* inventario;
    
    Personaje* siguiente;
};

// Estructura para estación del metro
struct Estacion {
    int id;
    string nombre;
    bool tieneOro;
    bool tienePista;
    bool pistaSaboteada;
    vector<int> conexiones; // IDs de estaciones conectadas
    vector<int> personajesEnEstacion; // IDs de personajes presentes
    Estacion* siguiente;
};

// Estructura para el mapa (grafo)
struct Mapa {
    Estacion* estaciones;
    int cantidadEstaciones;
};

// Estructura para equipo
struct Equipo {
    string nombre;
    Bando bando;
    Personaje* miembros;
    int cantidadMiembros;
};

// Estructura para el juego
struct Juego {
    Mapa* mapa;
    Equipo* equipos[3]; // 0: Policías honestos, 1: Ladrones, 2: Policías corruptos
    int rondaActual;
    stack<string> bitacora; // Historial de acciones
    queue<int> ordenTurnos; // Cola de IDs de personajes para el orden de turnos
    int oroTotal;
    int oroRecuperado;
    int ladronesCapturados;
};

// Prototipos de funciones
void inicializarJuego(Juego* juego);
void menuPrincipal(Juego* juego);
void gestionPersonajes(Juego* juego);
void gestionImplementos(Juego* juego);
void gestionEquipos(Juego* juego);
void gestionMochilas(Juego* juego);
void gestionMapas(Juego* juego);
void consultarEquipo(Juego* juego);
void simularJuego(Juego* juego);
void guardarBitacora(Juego* juego);

// Implementación de listas enlazadas
class ListaPersonajes {
private:
    Personaje* cabeza;
public:
    ListaPersonajes() : cabeza(nullptr) {}
    
    void agregarPersonaje(Personaje* nuevo) {
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }
    
    Personaje* buscarPorId(int id) {
        Personaje* actual = cabeza;
        while (actual != nullptr) {
            if (actual->id == id) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void eliminarPersonaje(int id) {
        Personaje* actual = cabeza;
        Personaje* anterior = nullptr;
        
        while (actual != nullptr && actual->id != id) {
            anterior = actual;
            actual = actual->siguiente;
        }
        
        if (actual == nullptr) return;
        
        if (anterior == nullptr) {
            cabeza = actual->siguiente;
        } else {
            anterior->siguiente = actual->siguiente;
        }
        
        delete actual;
    }
    
    void mostrarLista() {
        Personaje* actual = cabeza;
        while (actual != nullptr) {
            cout << "ID: " << actual->id << " - " << actual->nombre 
                 << " (" << (actual->bando == Bando::POLICIA_HONESTO ? "Policia Honesto" : 
                            actual->bando == Bando::LADRON ? "Ladron" : "Corrupto") << ")" << endl;
            actual = actual->siguiente;
        }
    }
};

// Implementación del grafo para el mapa
class GrafoMapa {
private:
    Mapa* mapa;
public:
    GrafoMapa() {
        mapa = new Mapa;
        mapa->estaciones = nullptr;
        mapa->cantidadEstaciones = 0;
    }
    
    void agregarEstacion(Estacion* nueva) {
        nueva->siguiente = mapa->estaciones;
        mapa->estaciones = nueva;
        mapa->cantidadEstaciones++;
    }
    
    Estacion* buscarEstacion(int id) {
        Estacion* actual = mapa->estaciones;
        while (actual != nullptr) {
            if (actual->id == id) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void conectarEstaciones(int id1, int id2) {
        Estacion* est1 = buscarEstacion(id1);
        Estacion* est2 = buscarEstacion(id2);
        
        if (est1 && est2) {
            est1->conexiones.push_back(id2);
            est2->conexiones.push_back(id1);
        }
    }
    
    void mostrarMapa() {
        Estacion* actual = mapa->estaciones;
        while (actual != nullptr) {
            cout << "Estacion " << actual->id << ": " << actual->nombre << endl;
            cout << "Conexiones: ";
            for (int conexion : actual->conexiones) {
                cout << conexion << " ";
            }
            cout << endl << "Personajes presentes: ";
            for (int personajeId : actual->personajesEnEstacion) {
                cout << personajeId << " ";
            }
            cout << endl << "-------------------" << endl;
            actual = actual->siguiente;
        }
    }
};

// Implementación de la pila para bitácora
class PilaBitacora {
private:
    stack<string> bitacora;
public:
    void agregarEntrada(string accion) {
        bitacora.push(accion);
    }
    
    void mostrarBitacora() {
        stack<string> temp = bitacora;
        while (!temp.empty()) {
            cout << temp.top() << endl;
            temp.pop();
        }
    }
    
    void guardarEnArchivo() {
        ofstream archivo("bitacora.txt");
        stack<string> temp = bitacora;
        
        while (!temp.empty()) {
            archivo << temp.top() << endl;
            temp.pop();
        }
        archivo.close();
    }
};

// Implementación de la cola para turnos
class ColaTurnos {
private:
    queue<int> turnos;
public:
    void agregarTurno(int personajeId) {
        turnos.push(personajeId);
    }
    
    int siguienteTurno() {
        if (turnos.empty()) return -1;
        int siguiente = turnos.front();
        turnos.pop();
        return siguiente;
    }
    
    bool vacia() {
        return turnos.empty();
    }
};

// Funciones principales del juego
void inicializarJuego(Juego* juego) {
    // Inicializar mapa básico
    juego->mapa = new Mapa;
    juego->mapa->estaciones = nullptr;
    juego->mapa->cantidadEstaciones = 0;
    
    // Inicializar equipos
    for (int i = 0; i < 3; i++) {
        juego->equipos[i] = new Equipo;
        juego->equipos[i]->miembros = nullptr;
        juego->equipos[i]->cantidadMiembros = 0;
    }
    
    juego->equipos[0]->nombre = "Policias Honestos";
    juego->equipos[0]->bando = Bando::POLICIA_HONESTO;
    
    juego->equipos[1]->nombre = "Los Sombras";
    juego->equipos[1]->bando = Bando::LADRON;
    
    juego->equipos[2]->nombre = "Policias Corruptos";
    juego->equipos[2]->bando = Bando::CORRUPTO;
    
    juego->rondaActual = 0;
    juego->oroTotal = 100; // Ejemplo: 100 lingotes
    juego->oroRecuperado = 0;
    juego->ladronesCapturados = 0;
}

void menuPrincipal(Juego* juego) {
    int opcion;
    do {
        cout << "===== EL ROBO AL TREN DEL ORO =====" << endl;
        cout << "1. Gestion de Personajes" << endl;
        cout << "2. Gestion de Implementos" << endl;
        cout << "3. Gestion de Equipos" << endl;
        cout << "4. Gestion de Mochilas" << endl;
        cout << "5. Gestion de Mapas" << endl;
        cout << "6. Consultar Equipo" << endl;
        cout << "7. Simular Juego" << endl;
        cout << "8. Ver Bitacora" << endl;
        cout << "9. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        switch (opcion) {
            case 1: gestionPersonajes(juego); break;
            case 2: gestionImplementos(juego); break;
            case 3: gestionEquipos(juego); break;
            case 4: gestionMochilas(juego); break;
            case 5: gestionMapas(juego); break;
            case 6: consultarEquipo(juego); break;
            case 7: simularJuego(juego); break;
            case 8: 
                cout << "=== BITACORA ===" << endl;
                // Mostrar bitácora
                break;
            case 9: cout << "Saliendo..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 9);
}

void gestionPersonajes(Juego* juego) {
    int opcion;
    do {
        cout << "=== GESTION DE PERSONAJES ===" << endl;
        cout << "1. Crear Personaje" << endl;
        cout << "2. Modificar Personaje" << endl;
        cout << "3. Eliminar Personaje" << endl;
        cout << "4. Listar Personajes" << endl;
        cout << "5. Buscar Personaje" << endl;
        cout << "6. Volver" << endl;
        cout << "Seleccione: ";
        cin >> opcion;
        
        // Implementar las operaciones CRUD para personajes
        switch (opcion) {
            case 1:
                // Crear nuevo personaje
                break;
            case 2:
                // Modificar personaje existente
                break;
            case 3:
                // Eliminar personaje
                break;
            case 4:
                // Listar todos los personajes
                break;
            case 5:
                // Buscar personaje por ID
                break;
            case 6: break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 6);
}

void gestionImplementos(Juego* juego) {
    int opcion;
    do {
        cout << "=== GESTION DE IMPLEMENTOS ===" << endl;
        cout << "1. Agregar Implemento" << endl;
        cout << "2. Modificar Implemento" << endl;
        cout << "3. Eliminar Implemento" << endl;
        cout << "4. Listar Implementos" << endl;
        cout << "5. Buscar Implemento" << endl;
        cout << "6. Volver" << endl;
        cout << "Seleccione: ";
        cin >> opcion;
        
        // Implementar operaciones CRUD para implementos
    } while (opcion != 6);
}

void gestionEquipos(Juego* juego) {
    // Implementar gestión de equipos
    cout << "=== GESTION DE EQUIPOS ===" << endl;
    for (int i = 0; i < 3; i++) {
        cout << i+1 << ". " << juego->equipos[i]->nombre << endl;
    }
}

void gestionMochilas(Juego* juego) {
    // Implementar gestión de mochilas/inventario
    cout << "=== GESTION DE MOCHILAS ===" << endl;
}

void gestionMapas(Juego* juego) {
    int opcion;
    do {
        cout << "=== GESTION DE MAPAS ===" << endl;
        cout << "1. Agregar Estacion" << endl;
        cout << "2. Modificar Estacion" << endl;
        cout << "3. Eliminar Estacion" << endl;
        cout << "4. Conectar Estaciones" << endl;
        cout << "5. Mostrar Mapa" << endl;
        cout << "6. Volver" << endl;
        cout << "Seleccione: ";
        cin >> opcion;
        
        // Implementar operaciones para el mapa
    } while (opcion != 6);
}

void consultarEquipo(Juego* juego) {
    cout << "=== CONSULTAR EQUIPO ===" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "Equipo: " << juego->equipos[i]->nombre << endl;
        cout << "Miembros: " << juego->equipos[i]->cantidadMiembros << endl;
        // Mostrar detalles de cada miembro
    }
}

void simularJuego(Juego* juego) {
    cout << "=== SIMULACION DEL JUEGO ===" << endl;
    cout << "Iniciando simulacion de rondas..." << endl;
    
    // Implementar la lógica del juego por rondas
    for (int ronda = 1; ronda <= 10; ronda++) {
        cout << "--- Ronda " << ronda << " ---" << endl;
        juego->rondaActual = ronda;
        
        // Simular acciones de cada personaje
        // Movimiento, acciones, interacciones
        
        // Verificar condiciones de victoria
        if (juego->oroRecuperado >= 90) {
            cout << "Los policias honestos han ganado!" << endl;
            break;
        }
        
        if (juego->ladronesCapturados >= 80) {
            cout << "Los policias honestos han ganado!" << endl;
            break;
        }
    }
    
    guardarBitacora(juego);
}

void guardarBitacora(Juego* juego) {
    ofstream archivo("resultados.txt");
    archivo << "=== RESULTADOS DEL JUEGO ===" << endl;
    archivo << "Rondas completadas: " << juego->rondaActual << endl;
    archivo << "Oro total: " << juego->oroTotal << endl;
    archivo << "Oro recuperado: " << juego->oroRecuperado << endl;
    archivo << "Ladrones capturados: " << juego->ladronesCapturados << endl;
    archivo.close();
}

// Función principal
int main() {
    srand(time(0)); // Para generación de números aleatorios
    
    Juego juego;
    inicializarJuego(&juego);
    menuPrincipal(&juego);
    
    return 0;
}