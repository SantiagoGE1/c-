#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <iomanip>
#include <limits>
#include <sstream> 

using namespace std;

//  ENUMERACION
enum class Bando { POLICIA_HONESTO, LADRON, CORRUPTO };
enum class Estado { ACTIVO, CAPTURADO, INHABILITADO };
enum class TipoPersonaje {
    INSPECTOR_JEFE, PERITO_FORENSE, OFICIAL_ASALTO,
    ANALISTA_DATOS, NEGOCIADOR, POLICIA_CORRUPTO,
    CEREBRO, FANTASMA, MULO, SABOTEADOR, HACKER, LOBERO
};

// Enum de la función 'inicializarItemsBase'
enum class TipoItem {
    SCANNER_METALES, KIT_HUELLAS, ORDEN_ALLOGRAMIENTO,
    CHALECO_ANTIBALAS, GRABADORA_ENCUBIERTA, GANAS_MANOS,
    SENUELO_COMUNICACIONES, LLAVES_MANIPULADAS, FONDO_NEGRO,
    OTRO10, OTRO11, OTRO12, OTRO13, OTRO14, OTRO15,
    OTRO16, OTRO17, OTRO18, OTRO19, OTRO20
};
// Estructura para 'Item'
struct Item {
    int id;
    string nombre;
    string descripcion;
    TipoItem tipo;
    int durabilidad; 
    int usosRestantes;
    string efecto;
    Item(int i, string n, string d, TipoItem t, int dur, string e)
        : id(i), nombre(n), descripcion(d), tipo(t), durabilidad(dur),
        usosRestantes(dur), efecto(e) {}
};

// Nodo para la lista de Items
struct NodoItem {
    Item* item;
    NodoItem* siguiente;
    NodoItem(Item* it) : item(it), siguiente(nullptr) {}
};

// Estructura para habilidades
struct Habilidad {
    string nombre;
    string descripcion;
    int cooldown;
    int cooldownActual;
    bool disponible;

    Habilidad(string n, string d, int cd)
        : nombre(n), descripcion(d), cooldown(cd),
        cooldownActual(0), disponible(true) {}
};

// Estructura para 'Personaje'
struct Personaje {
    int id;
    string nombre;
    string descripcion;
    TipoPersonaje tipo;
    Bando bando;
    Estado estado;
    int puntosVidaMax;
    int puntosVida;
    int capacidadOro;
    int cargaOro;
    int posicion;
    bool reveladoComoCorrupto;

    vector<Habilidad> habilidades;
    NodoItem* inventario;
    Personaje* siguiente;

    Personaje(int i, string n, string d, TipoPersonaje t, Bando b, int cap)
        : id(i), nombre(n), descripcion(d), tipo(t), bando(b),
        estado(Estado::ACTIVO), puntosVidaMax(5), puntosVida(5),
        capacidadOro(cap), cargaOro(0), posicion(0),
        reveladoComoCorrupto(false), inventario(nullptr), siguiente(nullptr) {
        
        
        if (t == TipoPersonaje::MULO) puntosVidaMax = 15;
        else if (t == TipoPersonaje::CEREBRO) puntosVidaMax = 8;
        puntosVida = puntosVidaMax;
    }

    // killer para limpiar el inventario del personaje
    ~Personaje() {
        NodoItem* actual = inventario;
        while (actual != nullptr) {
            NodoItem* siguienteNodo = actual->siguiente;
            delete actual->item;
            delete actual;
            actual = siguienteNodo;
        }
    }
};

// Estructura para Equipo (Necesaria para juego)
struct Equipo {
    string nombre;
    Personaje* miembros;
    int cantidadMiembros;
    Equipo(string n) : nombre(n), miembros(nullptr), cantidadMiembros(0) {}
};

// carga y guardado del juego
struct Juego {
    Equipo* equipos[3]; // 0: Honestos, 1: Ladrones, 2: Corruptos
    int oroTotal;
    int oroRecuperado;
    int ladronesCapturados;
    int corruptosDescubiertos;
    int rondaActual;
    bool juegoTerminado;

    Juego()
        : oroTotal(0), oroRecuperado(0),
        ladronesCapturados(0), corruptosDescubiertos(0),
        rondaActual(0), juegoTerminado(false) {
        equipos[0] = new Equipo("Policías Honestos");
        equipos[1] = new Equipo("Ladrones");
        equipos[2] = new Equipo("Policías Corruptos");
    }

    // killer para limpiar los equipos
    ~Juego() {
        for (int i = 0; i < 3; ++i) {
            if (equipos[i]) delete equipos[i];
        }
    }
};

//  CLASE LISTA PERSONAJES 

class ListaPersonajes {
private:
    Personaje* cabeza;
    int contadorId;

public:
    ListaPersonajes() : cabeza(nullptr), contadorId(1) {}

    ~ListaPersonajes() {
        Personaje* actual = cabeza;
        while (actual != nullptr) {
            Personaje* temp = actual;
            actual = actual->siguiente;
            delete temp; 
        }
    }

    void agregarPersonaje(Personaje* p) {
        p->id = contadorId++;
        p->siguiente = cabeza;
        cabeza = p;
    }

    Personaje* buscarPorId(int id) {
        Personaje* actual = cabeza;
        while (actual != nullptr) {
            if (actual->id == id) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    Personaje* buscarPorNombre(string nombre) {
        Personaje* actual = cabeza;
        while (actual != nullptr) {
            if (actual->nombre == nombre) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void eliminarPersonaje(int id) {
        if (cabeza == nullptr) return;

        if (cabeza->id == id) {
            Personaje* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return;
        }

        Personaje* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->id != id) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != nullptr) {
            Personaje* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp;
        }
    }


    void mostrarLista() {
        Personaje* actual = cabeza;
        int contador = 1;
        cout << "==============================================================" << endl;
        cout << "                       LISTA DE PERSONAJES                      " << endl;
        cout << "==============================================================" << endl;

        while (actual != nullptr) {
            cout << "[" << contador << "] " << actual->nombre << " (ID: " << actual->id << ")" << endl;
            cout << "  Tipo: ";
            switch (actual->tipo) {
                case TipoPersonaje::INSPECTOR_JEFE: cout << "Inspector Jefe"; break;
                case TipoPersonaje::PERITO_FORENSE: cout << "Perito Forense"; break;
                case TipoPersonaje::OFICIAL_ASALTO: cout << "Oficial de Asalto"; break;
                case TipoPersonaje::ANALISTA_DATOS: cout << "Analista de Datos"; break;
                case TipoPersonaje::NEGOCIADOR: cout << "Negociador"; break;
                case TipoPersonaje::POLICIA_CORRUPTO: cout << "Policia Corrupto"; break;
                case TipoPersonaje::CEREBRO: cout << "Cerebro"; break;
                case TipoPersonaje::FANTASMA: cout << "Fantasma"; break;
                case TipoPersonaje::MULO: cout << "Mulo"; break;
                case TipoPersonaje::SABOTEADOR: cout << "Saboteador"; break;
                case TipoPersonaje::HACKER: cout << "Hacker"; break;
                case TipoPersonaje::LOBERO: cout << "Lobero"; break;
            }
            cout << " | Bando: ";
            switch (actual->bando) {
                case Bando::POLICIA_HONESTO: cout << "Policia Honesto"; break;
                case Bando::LADRON: cout << "Ladron"; break;
                case Bando::CORRUPTO: cout << "Corrupto"; break;
            }
            cout << " | PV: " << actual->puntosVida << "/" << actual->puntosVidaMax;
            cout << " | Oro: " << actual->cargaOro << "/" << actual->capacidadOro << endl;
            cout << "  " << actual->descripcion << endl;
            cout << "  --------------------------------------------------------------" << endl;
            actual = actual->siguiente;
            contador++;
        }

        if (contador == 1) {
            cout << "  No hay personajes creados." << endl;
        }
    }

    Personaje* obtenerCabeza() { return cabeza; }
};


//CLASE LISTA ITEMS 
class ListaItems {
private:
    NodoItem* cabeza;
    int contadorId;

public:
    ListaItems() : cabeza(nullptr), contadorId(1) {}

    ~ListaItems() {
        NodoItem* cur = cabeza;
        while (cur != nullptr) {
            NodoItem* next = cur->siguiente;
            if (cur->item) delete cur->item;
            delete cur;
            cur = next;
        }
        cabeza = nullptr;
    }

    void agregarItem(Item* item) {
        if (item->id == 0) item->id = contadorId++; // Asignar ID si es nuevo
        NodoItem* nuevo = new NodoItem(item);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    Item* buscarPorId(int id) {
        NodoItem* actual = cabeza;
        while (actual != nullptr) {
            if (actual->item->id == id) return actual->item;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    Item* buscarPorNombre(string nombre) {
        NodoItem* actual = cabeza;
        while (actual != nullptr) {
            if (actual->item->nombre == nombre) return actual->item;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Devuelve 'true' si eliminó, 'false' si no
    bool eliminarItem(int id) {
        if (cabeza == nullptr) return false;

        if (cabeza->item->id == id) {
            NodoItem* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp->item;
            delete temp;
            return true;
        }

        NodoItem* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->item->id != id) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != nullptr) {
            NodoItem* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            delete temp->item;
            delete temp;
            return true;
        }
        return false;
    }

    Item* extraerItem(int id) {
        if (cabeza == nullptr) return nullptr;
        NodoItem* prev = nullptr;
        NodoItem* cur = cabeza;
        while (cur != nullptr && cur->item->id != id) {
            prev = cur;
            cur = cur->siguiente;
        }
        if (cur == nullptr) return nullptr;
        
        if (prev != nullptr) prev->siguiente = cur->siguiente;
        else cabeza = cur->siguiente;
        
        Item* it = cur->item;
        delete cur; // Borra el nodo, pero no el ítem
        return it;
    }

    void mostrarLista() {
        NodoItem* actual = cabeza;
        int contador = 1;
        cout << "==============================================================" << endl;
        cout << "                         LISTA DE ITEMS                         " << endl;
        cout << "==============================================================" << endl;

        while (actual != nullptr) {
            cout << "[" << contador << "] " << actual->item->nombre << " (ID: " << actual->item->id << ")" << endl;
            cout << "  Tipo: " << actual->item->descripcion << endl;
            cout << "  Usos: " << actual->item->usosRestantes << "/" << actual->item->durabilidad << endl;
            cout << "  Efecto: " << actual->item->efecto << endl;
            cout << "  --------------------------------------------------------------" << endl;
            actual = actual->siguiente;
            contador++;
        }

        if (contador == 1) {
            cout << "  No hay items creados." << endl;
        }
    }

    NodoItem* obtenerCabeza() { return cabeza; }
};


// FUNCIÓN AUXILIAR

// ayuda a la lectura segura de enteros con rango
int leerEntero(int minVal, int maxVal) {
    int val;
    while (true) {
        if (!(cin >> val)) {
            cout << "Entrada inválida. Intenta de nuevo: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (val < minVal || val > maxVal) {
            cout << "Valor fuera de rango (" << minVal << "-" << maxVal << "). Intenta de nuevo: ";
            continue;
        }
        // consumir resto de línea
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return val;
    }
}

// Personajes Items

void inicializarHabilidadesPersonaje(Personaje* personaje) {
    switch (personaje->tipo) {
        case TipoPersonaje::INSPECTOR_JEFE:
            personaje->habilidades.push_back(Habilidad("Investigacion Avanzada",
                "Revela 2 pistas en lugar de 1", 2));
            personaje->habilidades.push_back(Habilidad("Intuicion Incorruptible",
                "Detecta si un policia en la misma estacion es corrupto", 3));
            break;
        case TipoPersonaje::PERITO_FORENSE:
            personaje->habilidades.push_back(Habilidad("Analisis Forense",
                "Revela informacion adicional de las pistas", 2));
            break;
        case TipoPersonaje::OFICIAL_ASALTO:
            personaje->habilidades.push_back(Habilidad("Captura Eficaz",
                "Mayor probabilidad de éxito en capturas", 1));
            break;
        case TipoPersonaje::ANALISTA_DATOS:
            personaje->habilidades.push_back(Habilidad("Rastreo de Ladrones",
                "Revela la ultima ubicacion conocida de un ladron", 2));
            break;
        case TipoPersonaje::NEGOCIADOR:
            personaje->habilidades.push_back(Habilidad("Interrogatorio Eficaz",
                "Obtiene informacion adicional de civiles y ladrones capturados", 2));
            break;
        case TipoPersonaje::POLICIA_CORRUPTO:
            personaje->habilidades.push_back(Habilidad("Sabotaje",
                "Borra evidencias criticas permanentemente", 2));
            personaje->habilidades.push_back(Habilidad("Soborno",
                "Bloquea el movimiento de un policia honesto", 3));
            break;
        case TipoPersonaje::CEREBRO:
            personaje->habilidades.push_back(Habilidad("Mover Fichas",
                "Da una acción adicional a un ladrón en el mismo radio", 2));
            personaje->habilidades.push_back(Habilidad("Sembrar Duda",
                "Crea falsas pistas que señalan policías honestos como corruptos", 3));
            break;
        case TipoPersonaje::FANTASMA:
            personaje->habilidades.push_back(Habilidad("Sigilo Total",
                "Invisible para habilidades de rastreo por 2 turnos", 3));
            break;
        case TipoPersonaje::MULO:
            personaje->habilidades.push_back(Habilidad("Transporte Pesado",
                "Puede cargar el triple de oro por 3 turnos", 4));
            break;
        case TipoPersonaje::SABOTEADOR:
            personaje->habilidades.push_back(Habilidad("Pistas Falsas",
                "Coloca pistas que desvían a los policías", 2));
            personaje->habilidades.push_back(Habilidad("Trampas",
                "Retrasa el avance de policías en una estación", 2));
            break;
        case TipoPersonaje::HACKER:
            personaje->habilidades.push_back(Habilidad("Interrumpir Comunicaciones",
                "Bloquea habilidades especiales de policías por 1 turno", 3));
            break;
        case TipoPersonaje::LOBERO:
            personaje->habilidades.push_back(Habilidad("Distracción",
                "Mueve un policía contra su voluntad a una estación adyacente", 2));
            break;
    }
}

void inicializarItemsBase(ListaItems& listaItems) {
    // Items para Policías
    listaItems.agregarItem(new Item(0, "Scanner de Metales",
        "Permite rastrear oro en un radio de 2 estaciones",
        TipoItem::SCANNER_METALES, 3, "Rastrear oro en área amplia"));
    listaItems.agregarItem(new Item(0, "Kit de Huellas Dactilares",
        "Mejora la acción Investigar, garantizando encontrar pista crucial",
        TipoItem::KIT_HUELLAS, 2, "Investigación garantizada"));
    listaItems.agregarItem(new Item(0, "Orden de Allanamiento",
        "Permite Investigar o Capturar en estación adyacente sin moverse",
        TipoItem::ORDEN_ALLOGRAMIENTO, 2, "Acción a distancia"));
    listaItems.agregarItem(new Item(0, "Chaleco Antibalas",
        "Segunda oportunidad al fallar desafío de captura/escape",
        TipoItem::CHALECO_ANTIBALAS, 1, "Protección en combate"));
    listaItems.agregarItem(new Item(0, "Grabadora Encubierta",
        "Si graba a un corrupto usando habilidad ilegal, lo captura al instante",
        TipoItem::GRABADORA_ENCUBIERTA, 1, "Contra corrupción"));
    // Items para Ladrones
    listaItems.agregarItem(new Item(0, "Ganas de Manos",
        "Aumenta temporalmente la capacidad de carga de oro",
        TipoItem::GANAS_MANOS, 2, "Más capacidad de oro"));
    listaItems.agregarItem(new Item(0, "Señuelo de Comunicaciones",
        "Crea falsa alerta de ladrón detectado en mapa de policías",
        TipoItem::SENUELO_COMUNICACIONES, 2, "Distracción"));
    listaItems.agregarItem(new Item(0, "Llaves Manipuladas",
        "Abrir pasaje secreto, teletransportándose a cualquier estación conectada",
        TipoItem::LLAVES_MANIPULADAS, 1, "Teletransporte"));
    // Items para Policías Corruptos
    listaItems.agregarItem(new Item(0, "Fondo Negro",
        "Soborno potente que permite Sabotear de forma remota",
        TipoItem::FONDO_NEGRO, 1, "Soborno avanzado"));
}

void crearPersonajesBase(ListaPersonajes& listaPersonajes) {
    // Policías Honestos
    listaPersonajes.agregarPersonaje(new Personaje(0, "Inspector Kaine",
        "La ley no es una guia flexible, es el faro que nos impide perdernos en la oscuridad",
        TipoPersonaje::INSPECTOR_JEFE, Bando::POLICIA_HONESTO, 5));
    listaPersonajes.agregarPersonaje(new Personaje(0, "Perito Rodriguez",
        "Especialista en analisis forense y recoleccion de evidencias",
        TipoPersonaje::PERITO_FORENSE, Bando::POLICIA_HONESTO, 3));
    listaPersonajes.agregarPersonaje(new Personaje(0, "Oficial Martinez",
        "Experto en operativos de captura y enfrentamientos",
        TipoPersonaje::OFICIAL_ASALTO, Bando::POLICIA_HONESTO, 3));
    // Ladrones
    listaPersonajes.agregarPersonaje(new Personaje(0, "Silas Vance",
        "El oro no es el botin, es la llave para comprar la libertad",
        TipoPersonaje::CEREBRO, Bando::LADRON, 8));
    listaPersonajes.agregarPersonaje(new Personaje(0, "El Fantasma",
        "Se desvanece en las sombras, dejando solo recuerdos",
        TipoPersonaje::FANTASMA, Bando::LADRON, 4));
    listaPersonajes.agregarPersonaje(new Personaje(0, "El Mulo",
        "Carga con el peso del botin, silencioso y eficiente",
        TipoPersonaje::MULO, Bando::LADRON, 15));
    // Policías Corruptos (inicialmente ocultos)
    listaPersonajes.agregarPersonaje(new Personaje(0, "Agente Smith",
        "Siempre en el lugar correcto en el momento correcto... ¿demasiado correcto?",
        TipoPersonaje::POLICIA_CORRUPTO, Bando::CORRUPTO, 4));

    // habilidades para cada personaje
    Personaje* actual = listaPersonajes.obtenerCabeza();
    while (actual != nullptr) {
        inicializarHabilidadesPersonaje(actual);
        actual = actual->siguiente;
    }
}

// Esta es la versión correcta de 'menuGestionPersonajes'
void menuGestionPersonajes(ListaPersonajes& listaPersonajes) {
    int opcion;
    do {
        cout << "\n┌─────────────────────────────────────────────────────────────────────────┐" << endl;
        cout << "│                       GESTIÓN DE PERSONAJES                         │" << endl;
        cout << "└─────────────────────────────────────────────────────────────────────────┘" << endl;
        cout << "1. Crear Personaje" << endl;
        cout << "2. Modificar Personaje" << endl;
        cout << "3. Eliminar Personaje" << endl;
        cout << "4. Listar Personajes" << endl;
        cout << "5. Buscar Personaje" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero(1, 6);

        switch (opcion) {
            case 1: {
                string nombre, descripcion;
                int tipo, bando, capacidad;
                
                cout << "\n--- CREAR NUEVO PERSONAJE ---" << endl;
                cout << "Nombre: ";
                getline(cin, nombre);
                cout << "Descripcion: ";
                getline(cin, descripcion);
                
                cout << "Tipo (1-12): " << endl;
                cout << "1. Inspector Jefe  2. Perito Forense  3. Oficial Asalto" << endl;
                cout << "4. Analista Datos  5. Negociador      6. Policia Corrupto" << endl;
                cout << "7. Cerebro         8. Fantasma        9. Mulo" << endl;
                cout << "10. Saboteador     11. Hacker         12. Lobero" << endl;
                tipo = leerEntero(1, 12);
                
                cout << "Bando (1-3): " << endl;
                cout << "1. Policia Honesto  2. Ladron  3. Corrupto" << endl;
                bando = leerEntero(1, 3);
                
                cout << "Capacidad de oro: ";
                capacidad = leerEntero(1, 1000);
                
                TipoPersonaje tipoEnum = static_cast<TipoPersonaje>(tipo - 1);
                Bando bandoEnum = static_cast<Bando>(bando - 1);
                
                Personaje* nuevo = new Personaje(0, nombre, descripcion, tipoEnum, bandoEnum, capacidad);
                inicializarHabilidadesPersonaje(nuevo);
                listaPersonajes.agregarPersonaje(nuevo);
                
                cout << "Personaje creado exitosamente!" << endl;
                break;
            }
            
            case 2: {
                cout << "\n--- MODIFICAR PERSONAJE ---" << endl;
                listaPersonajes.mostrarLista();
                int id;
                cout << "ID del personaje a modificar: ";
                id = leerEntero(1, 10000);
                
                Personaje* personaje = listaPersonajes.buscarPorId(id);
                if (personaje) {
                    string nuevoNombre, nuevaDesc;
                    cout << "Nuevo nombre (actual: " << personaje->nombre << "): ";
                    getline(cin, nuevoNombre);
                    cout << "Nueva descripcion: ";
                    getline(cin, nuevaDesc);
                    
                    if (!nuevoNombre.empty()) personaje->nombre = nuevoNombre;
                    if (!nuevaDesc.empty()) personaje->descripcion = nuevaDesc;
                    
                    cout << "Personaje modificado exitosamente!" << endl;
                } else {
                    cout << "Personaje no encontrado." << endl;
                }
                break;
            }
            
            case 3: {
                cout << "\n--- ELIMINAR PERSONAJE ---" << endl;
                listaPersonajes.mostrarLista();
                int id;
                cout << "ID del personaje a eliminar: ";
                id = leerEntero(1, 10000);
                
                Personaje* personaje = listaPersonajes.buscarPorId(id);
                if (personaje) {
                    listaPersonajes.eliminarPersonaje(id);
                    cout << "Personaje eliminado exitosamente!" << endl;
                } else {
                    cout << " Personaje no encontrado." << endl;
                }
                break;
            }
            
            case 4:
                listaPersonajes.mostrarLista();
                break;
                
            case 5: {
                string nombre;
                cout << "Nombre del personaje a buscar: ";
                getline(cin, nombre);
                
                Personaje* personaje = listaPersonajes.buscarPorNombre(nombre);
                if (personaje) {
                    cout << "Personaje encontrado:" << endl;
                    cout << "ID: " << personaje->id << " | Nombre: " << personaje->nombre << endl;
                    cout << "Descripcion: " << personaje->descripcion << endl;
                } else {
                    cout << "Personaje no encontrado." << endl;
                }
                break;
            }
            
            case 6:
                cout << "Volviendo al menú principal..." << endl;
                break;
                
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 6);
}

// Esta es la versión correcta de 'menuGestionItems'
void menuGestionItems(ListaItems& listaItems) {
    // Helpers locales para acciones del menú
    auto crearItem = [&]() {
        string nombre, descripcion, efecto;
        int tipo, durabilidad;
        cout << "\n--- CREAR NUEVO ITEM ---" << endl;
        cout << "Nombre: ";
        getline(cin, nombre);
        cout << "Descripcion: ";
        getline(cin, descripcion);
        cout << "Efecto: ";
        getline(cin, efecto);
        cout << "Durabilidad: ";
        durabilidad = leerEntero(1, 1000);
        cout << "Tipo (1-20): ";
        tipo = leerEntero(1, 20);
        TipoItem tipoEnum = static_cast<TipoItem>(tipo - 1);
        listaItems.agregarItem(new Item(0, nombre, descripcion, tipoEnum, durabilidad, efecto));
        cout << "Item creado exitosamente!" << endl;
    };

    auto modificarItem = [&]() {
        cout << "\n--- MODIFICAR ITEM ---" << endl;
        listaItems.mostrarLista();
        cout << "ID del item a modificar: ";
        int id = leerEntero(1, 10000);
        Item* it = listaItems.buscarPorId(id);
        if (!it) { cout << "Item no encontrado." << endl; return; }
        string nombre, descripcion, efecto;
        int durabilidad;
        cout << "Nuevo nombre (actual: " << it->nombre << "): ";
        getline(cin, nombre);
        cout << "Nueva descripcion: ";
        getline(cin, descripcion);
        cout << "Nuevo efecto: ";
        getline(cin, efecto);
        cout << "Nueva durabilidad: ";
        durabilidad = leerEntero(1, 1000);
        if (!nombre.empty()) it->nombre = nombre;
        if (!descripcion.empty()) it->descripcion = descripcion;
        if (!efecto.empty()) it->efecto = efecto;
        it->durabilidad = durabilidad;
        it->usosRestantes = durabilidad; // Reiniciar usos al modificar
        cout << "Item modificado exitosamente." << endl;
    };

    auto eliminarItem = [&]() {
        cout << "\n--- ELIMINAR ITEM ---" << endl;
        listaItems.mostrarLista();
        cout << "ID del item a eliminar: ";
        int id = leerEntero(1, 10000);
        if (listaItems.eliminarItem(id)) {
            cout << "Item eliminado." << endl;
        } else {
            cout << "Item no encontrado." << endl;
        }
    };

    auto listarItems = [&]() { listaItems.mostrarLista(); };

    auto buscarItem = [&]() {
        string nombre;
        cout << "Nombre del item a buscar: ";
        getline(cin, nombre);
        Item* it = listaItems.buscarPorNombre(nombre);
        if (it) {
            cout << "Item encontrado: ID=" << it->id << " Nombre=" << it->nombre << " Efecto=" << it->efecto << endl;
        } else {
            cout << "Item no encontrado." << endl;
        }
    };

    int opcion;
    do {
        cout << "\n==============================================================" << endl;
        cout << "                       GESTION DE ITEMS                         " << endl;
        cout << "==============================================================" << endl;
        cout << "1. Crear Item" << endl;
        cout << "2. Modificar Item" << endl;
        cout << "3. Eliminar Item" << endl;
        cout << "4. Listar Items" << endl;
        cout << "5. Buscar Item" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero(1, 6);

        switch (opcion) {
            case 1: crearItem(); break;
            case 2: modificarItem(); break;
            case 3: eliminarItem(); break;
            case 4: listarItems(); break;
            case 5: buscarItem(); break;
            case 6: cout << "Volviendo al menu principal..." << endl; break;
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 6);
}


// clase de juego manager
class JuegoManager {
private:
    ListaPersonajes* listaPersonajes;
    ListaItems* listaItems;
    Juego* juego; // <-- (necesario para guardar/cargar)

public:
    JuegoManager() {
        listaPersonajes = new ListaPersonajes();
        listaItems = new ListaItems();
        juego = new Juego(); 
        
        // Inicializar datos base
        inicializarItemsBase(*listaItems);
        crearPersonajesBase(*listaPersonajes);
    }

    ~JuegoManager() {
        // El killer de ListaPersonajes se encarga de borrar
        // los personajes (ya que no han sido asignados a equipos)
        if (listaPersonajes) { delete listaPersonajes; }
        if (listaItems) { delete listaItems; }
        if (juego) { delete juego; } 
    }
    
    
    // FUNCIÓN DE GUARDADO 
    void guardarEstado(const string& ruta) {
        ofstream out(ruta);
        if (!out.is_open()) {
            cout << "No se pudo abrir el archivo para guardar: " << ruta << endl;
            return;
        }
        
        // Guardar estado del juego (ronda, oro, etc.)
        out << "ronda " << juego->rondaActual << "\n";
        out << "oroTotal " << juego->oroTotal << "\n";
        out << "oroRecuperado " << juego->oroRecuperado << "\n";
        out << "ladronesCapturados " << juego->ladronesCapturados << "\n";
        out << "corruptosDescubiertos " << juego->corruptosDescubiertos << "\n";

        // Guardar personajes (id|nombre|tipo|bando|pos|estado|pv|carga|cap)

        Personaje* p = listaPersonajes->obtenerCabeza();
        while (p) {
            out << "P|" << p->id << "|" << p->nombre << "|" << (int)p->tipo << "|" << (int)p->bando
                << "|" << p->posicion << "|" << (int)p->estado << "|" << p->puntosVida
                << "|" << p->cargaOro << "|" << p->capacidadOro << "|" << p->reveladoComoCorrupto << "\n";
            p = p->siguiente;
        }
        
        // Aquí faltaría guardar el estado del mapa (oro, pistas)
        
        out.close();
        cout << "Estado guardado en: " << ruta << endl;
    }

    // FUNCIÓN DE CARGA
    void cargarEstado(const string& ruta) {
        ifstream in(ruta);
        if (!in.is_open()) {
            cout << "No se pudo abrir el archivo para cargar: " << ruta << endl;
            return;
        }
        
        string linea;
        while (getline(in, linea)) {
            if (linea.rfind("ronda ", 0) == 0) {
                juego->rondaActual = stoi(linea.substr(6));
            }
            else if (linea.rfind("oroTotal ", 0) == 0) {
                juego->oroTotal = stoi(linea.substr(9));
            }
            else if (linea.rfind("oroRecuperado ", 0) == 0) {
                juego->oroRecuperado = stoi(linea.substr(14));
            }
            else if (linea.rfind("ladronesCapturados ", 0) == 0) {
                juego->ladronesCapturados = stoi(linea.substr(20));
            }
            else if (linea.rfind("corruptosDescubiertos ", 0) == 0) {
                juego->corruptosDescubiertos = stoi(linea.substr(24));
            }
            else if (linea.rfind("P|", 0) == 0) {
                // dividir la línea
                stringstream ss(linea.substr(2)); 
                string token;
                vector<string> parts;
                
                while (getline(ss, token, '|')) {
                    parts.push_back(token);
                }
                
                if (parts.size() >= 10) {
                    int id = stoi(parts[0]);
                    Personaje* p = listaPersonajes->buscarPorId(id);
                    if (p) {
                        p->nombre = parts[1];
                        p->tipo = static_cast<TipoPersonaje>(stoi(parts[2]));
                        p->bando = static_cast<Bando>(stoi(parts[3]));
                        p->posicion = stoi(parts[4]);
                        p->estado = static_cast<Estado>(stoi(parts[5]));
                        p->puntosVida = stoi(parts[6]);
                        p->cargaOro = stoi(parts[7]);
                        p->capacidadOro = stoi(parts[8]);
                        p->reveladoComoCorrupto = (parts[9] == "1");
                    }
                }
            }
        }
        in.close();
        cout << "Estado cargado desde: " << ruta << endl;
    }

    // Mochilas para el jugadore
    void menuGestionMochilas() {
        cout << "\n--- GESTION DE MOCHILAS ---" << endl;
        if (!listaPersonajes->obtenerCabeza()) {
            cout << "No hay personajes creados. No se pueden ver mochilas." << endl;
            return;
        }
        
        //  Mostrar personajes para que el usuario elija
        listaPersonajes->mostrarLista();
        cout << "\nSeleccione el ID del personaje para ver su mochila (0 para cancelar): ";
        int id = leerEntero(0, 10000);

        if (id == 0) return;

        //Buscar al personaje
        Personaje* p = listaPersonajes->buscarPorId(id);
        if (!p) {
            cout << "Error: Personaje no encontrado." << endl;
            return;
        }

        // Mostrar inventario (mochila)
        cout << "\n Inventario de " << p->nombre << ":" << endl;
        cout << "------------------------------------------" << endl;
        if (p->inventario == nullptr) {
            cout << "  (Mochila vacia)" << endl;
        } else {
            NodoItem* actual = p->inventario;
            int contador = 1;
            while (actual != nullptr) {
                cout << " " << contador << ". " << actual->item->nombre << endl;
                cout << "    Usos: " << actual->item->usosRestantes << "/" << actual->item->durabilidad << endl;
                cout << "    Efecto: " << actual->item->efecto << endl;
                actual = actual->siguiente;
                contador++;
            }
        }
        cout << "------------------------------------------" << endl;
        cout << "Presione Enter para continuar...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    // Aqui los equipos
    void menuGestionEquipos() {
        cout << "\n--- GESTION DE EQUIPOS ---" << endl;
        cout << "Mostrando personajes agrupados por su bando asignado:" << endl;

        Personaje* p = listaPersonajes->obtenerCabeza();
        if (!p) {
            cout << "No hay personajes creados para mostrar." << endl;
            return;
        }

        // Bucle 1) Policías Honestos
        cout << "\n== EQUIPO: POLICIAS HONESTOS ==" << endl;
        int countHonestos = 0;
        p = listaPersonajes->obtenerCabeza(); // Reiniciar puntero
        while (p != nullptr) {
            if (p->bando == Bando::POLICIA_HONESTO) {
                cout << "  - (ID: " << p->id << ") " << p->nombre << " [" << p->puntosVida << " PV]" << endl;
                countHonestos++;
            }
            p = p->siguiente;
        }
        if (countHonestos == 0) cout << "  (No hay personajes en este bando)" << endl;


        // Bucle 2) Ladrones
        cout << "\n== EQUIPO: LADRONES ==" << endl;
        int countLadrones = 0;
        p = listaPersonajes->obtenerCabeza(); // Reiniciar puntero
        while (p != nullptr) {
            if (p->bando == Bando::LADRON) {
                cout << "  - (ID: " << p->id << ") " << p->nombre << " [" << p->puntosVida << " PV, Cap: " << p->capacidadOro << "]" << endl;
                countLadrones++;
            }
            p = p->siguiente;
        }
        if (countLadrones == 0) cout << "  (No hay personajes en este bando)" << endl;


        // Bucle 3) Policías Corruptos
        cout << "\n== EQUIPO: POLICIAS CORRUPTOS ==" << endl;
        int countCorruptos = 0;
        p = listaPersonajes->obtenerCabeza(); // Reiniciar puntero
        while (p != nullptr) {
            if (p->bando == Bando::CORRUPTO) {
                cout << "  - (ID: " << p->id << ") " << p->nombre << " [" << p->puntosVida << " PV]" << endl;
                countCorruptos++;
            }
            p = p->siguiente;
        }
        if (countCorruptos == 0) cout << "  (No hay personajes en este bando)" << endl;

        cout << "\nPresione Enter para continuar..."; 
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }


    // Menu de items
    void menuGestionItemsInteractivo() {
        int opcion;
        do {
            cout << "\n--- GESTION DE IMPLEMENTOS (Interactivo) ---" << endl;
            cout << "1. Crear Item" << endl;
            cout << "2. Asignar Item a Personaje" << endl;
            cout << "3. Listar Items (Globales)" << endl;
            cout << "4. Guardar Estado" << endl; 
            cout << "5. Cargar Estado" << endl;    
            cout << "6. Volver" << endl;
            cout << "Seleccione una opcion: ";
            opcion = leerEntero(1, 6);

            switch (opcion) {
                case 1: {
                    string nombre, descripcion, efecto;
                    int tipo, durabilidad;
                    cout << "Nombre: ";
                    getline(cin, nombre);
                    cout << "Descripción: ";
                    getline(cin, descripcion);
                    cout << "Efecto: ";
                    getline(cin, efecto);
                    cout << "Durabilidad: ";
                    durabilidad = leerEntero(1, 1000);
                    cout << "Tipo (1-20): ";
                    tipo = leerEntero(1, 20);
                    TipoItem tipoEnum = static_cast<TipoItem>(tipo - 1);
                    listaItems->agregarItem(new Item(0, nombre, descripcion, tipoEnum, durabilidad, efecto));
                    cout << "Item creado." << endl;
                    break;
                }
                case 2: {
                    cout << "--- Asignar Item a Personaje ---" << endl;
                    listaItems->mostrarLista();
                    cout << "ID del item a asignar: ";
                    int itemId = leerEntero(1, 10000);
                    Item* item = listaItems->extraerItem(itemId);
                    if (!item) {
                        cout << "Item no encontrado." << endl;
                        break;
                    }
                    listaPersonajes->mostrarLista();
                    cout << "ID del personaje receptor: ";
                    int pid = leerEntero(1, 10000);
                    Personaje* p = listaPersonajes->buscarPorId(pid);
                    if (!p) {
                        cout << "Personaje no encontrado. Devolviendo item a la lista global." << endl;
                        listaItems->agregarItem(item);
                        break;
                    }
                    NodoItem* nodo = new NodoItem(item);
                    nodo->siguiente = p->inventario;
                    p->inventario = nodo;
                    cout << "Item " << item->nombre << " asignado a " << p->nombre << "." << endl;
                    break;
                }
                case 3:
                    listaItems->mostrarLista();
                    break;
                case 4: {
                    cout << "Guardando estado del juego en 'save.txt'..." << endl;
                    guardarEstado("save.txt");
                    break;
                }
                case 5: {
                    cout << "Cargando estado del juego desde 'save.txt'..." << endl;
                    cargarEstado("save.txt");
                    break;
                }
                case 6:
                    cout << "Volviendo..." << endl;
                    break;
            }
        } while (opcion != 6);
    }
    
    // getters para los handlerss
    ListaPersonajes* getListaPersonajes() { return listaPersonajes; }
    ListaItems* getListaItems() { return listaItems; }
};
//Menu principal

void mostrarMenuPrincipal() {
    cout << "\n";
    cout << "==============================================================" << endl;
    cout << "           EL ROBO AL TREN DEL ORO - MENU           " << endl;
    cout << "==============================================================" << endl;
    cout << "\n";
    cout << "Bienvenido al sistema de gestion del robo al tren del oro" << endl;
    cout << "\n";
    cout << " 1. Gestion de Personajes       4. Gestion de Mochilas" << endl;
    cout << " 2. Gestion de Implementos      5. Gestion de Mapas   " << endl;
    cout << " 3. Gestion de Equipos          6. Consultar Equipo   " << endl;
    cout << "\n";
    cout << " 7. Simular Juego Completo      8. Ver Bitacora       " << endl;
    cout << " 9. Salir del Sistema" << endl;
    cout << "\n";
    cout << "Seleccione una opcion: ";
}

// Los handlers para cada opción del menú principal

void handleGestionPersonajes(JuegoManager& jm) {
    
    menuGestionPersonajes(*jm.getListaPersonajes());
}

void handleGestionItems(JuegoManager& jm) {
    
    jm.menuGestionItemsInteractivo();
}

//stubbets para los demás handlers

void handleGestionEquipos(JuegoManager& jm) {
    jm.menuGestionEquipos(); 
}

void handleGestionMochilas(JuegoManager& jm) {
    jm.menuGestionMochilas(); 
}

void handleGestionMapas(JuegoManager& jm) {
    cout << "Gestion de Mapas - Proximamente..." << endl;
}

void handleConsultarEquipo(JuegoManager& jm) {
    cout << "Consultar Equipo - Proximamente..." << endl;
}

void handleSimularJuego(JuegoManager& jm) {
    cout << "Simular Juego - Lógica no incluida en este fragmento." << endl;
}

void handleVerBitacora(JuegoManager& jm) {
    cout << "Ver Bitacora - Lógica no incluida en este fragmento." << endl;
}

void handleSalir(JuegoManager& jm) {
    cout << "Gracias por jugar a El Robo al Tren del Oro!" << endl;
}


// Punto de entrada principal
int main() {
    srand(time(0)); 
    JuegoManager juegoManager;
    int opcion;

    cout << "==============================================================" << endl;
    cout << "           EL ROBO AL TREN DEL ORO v1.0           " << endl;
    cout << "   Un juego de estrategia, traicion y toma de decisiones   " << endl;
    cout << "==============================================================" << endl;

    this_thread::sleep_for(chrono::seconds(1));

    do {
        mostrarMenuPrincipal();
        opcion = leerEntero(1, 9);

        switch (opcion) {
            case 1: handleGestionPersonajes(juegoManager); break;
            case 2: handleGestionItems(juegoManager); break;
            case 3: handleGestionEquipos(juegoManager); break;
            case 4: handleGestionMochilas(juegoManager); break;
            case 5: handleGestionMapas(juegoManager); break;
            case 6: handleConsultarEquipo(juegoManager); break;
            case 7: handleSimularJuego(juegoManager); break;
            case 8: handleVerBitacora(juegoManager); break;
            case 9: handleSalir(juegoManager); break;
            default:
                cout << " Opcion no valida. Por favor, seleccione 1-9." << endl;
        }

    } while (opcion != 9);

    return 0;
}