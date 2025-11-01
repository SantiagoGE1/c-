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

using namespace std;

// Helper: lectura segura de enteros con rango
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

// ==================== ENUMERACIONES Y ESTRUCTURAS BASE ====================

enum class Bando { POLICIA_HONESTO, LADRON, CORRUPTO };
enum class Estado { ACTIVO, CAPTURADO, INHABILITADO };
enum class TipoPersonaje {
    INSPECTOR_JEFE, PERITO_FORENSE, OFICIAL_ASALTO, 
    ANALISTA_DATOS, NEGOCIADOR, POLICIA_CORRUPTO,
    CEREBRO, FANTASMA, MULO, SABOTEADOR, HACKER, LOBERO
};
void menuGestionItems(ListaItems& listaItems) {
    // Helpers locales
    auto crearItem = [&]() {
        string nombre, descripcion, efecto;
        int tipo, durabilidad;
        cout << "\n--- CREAR NUEVO ITEM ---" << endl;
        cout << "Nombre: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, nombre);
        cout << "Descripcion: ";
        getline(cin, descripcion);
        cout << "Efecto: ";
        getline(cin, efecto);
        cout << "Durabilidad: "; durabilidad = leerEntero(1,1000);
        cout << "Tipo (1-20): "; tipo = leerEntero(1,20);
        TipoItem tipoEnum = static_cast<TipoItem>(tipo - 1);
        listaItems.agregarItem(new Item(0, nombre, descripcion, tipoEnum, durabilidad, efecto));
        cout << "Item creado exitosamente!" << endl;
    };

    auto modificarItem = [&]() {
        cout << "\n--- MODIFICAR ITEM ---" << endl;
        listaItems.mostrarLista();
        cout << "ID del item a modificar: ";
        int id = leerEntero(1,10000);
        Item* it = listaItems.buscarPorId(id);
        if (!it) { cout << "Item no encontrado." << endl; return; }
        string nombre, descripcion, efecto; int durabilidad;
        cout << "Nuevo nombre (actual: " << it->nombre << "): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, nombre);
        cout << "Nueva descripcion: "; getline(cin, descripcion);
        cout << "Nuevo efecto: "; getline(cin, efecto);
        cout << "Nueva durabilidad: "; durabilidad = leerEntero(1,1000);
        if (!nombre.empty()) it->nombre = nombre;
        if (!descripcion.empty()) it->descripcion = descripcion;
        if (!efecto.empty()) it->efecto = efecto;
        it->durabilidad = durabilidad;
        cout << "Item modificado exitosamente." << endl;
    };

    auto eliminarItem = [&]() {
        cout << "\n--- ELIMINAR ITEM ---" << endl;
        listaItems.mostrarLista();
        cout << "ID del item a eliminar: "; int id = leerEntero(1,10000);
        listaItems.eliminarItem(id);
        cout << "Operación finalizada." << endl;
    };

    auto listarItems = [&]() { listaItems.mostrarLista(); };

    auto buscarItem = [&]() {
        cout << "Nombre del item a buscar: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string nombre; getline(cin, nombre);
        Item* it = listaItems.buscarPorNombre(nombre);
        if (it) cout << "Item encontrado: ID=" << it->id << " Nombre=" << it->nombre << " Efecto=" << it->efecto << endl;
        else cout << "Item no encontrado." << endl;
    };

    int opcion;
    do {
        cout << "\n==============================================================" << endl;
        cout << "                      GESTION DE ITEMS                        " << endl;
        cout << "==============================================================" << endl;
        cout << "1. Crear Item" << endl;
        cout << "2. Modificar Item" << endl;
        cout << "3. Eliminar Item" << endl;
        cout << "4. Listar Items" << endl;
        cout << "5. Buscar Item" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero(1,6);
        switch(opcion) {
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
            if (!nuevoNombre.empty()) personaje->nombre = nuevoNombre;
            if (!nuevaDesc.empty()) personaje->descripcion = nuevaDesc;
            cout << "Personaje modificado exitosamente!" << endl;
        };

        auto eliminarPersonaje = [&](void) {
            cout << "\n--- ELIMINAR PERSONAJE ---" << endl;
            listaPersonajes.mostrarLista();
            cout << "ID del personaje a eliminar: ";
            int id = leerEntero(1,10000);
            Personaje* personaje = listaPersonajes.buscarPorId(id);
            if (!personaje) { cout << "Personaje no encontrado." << endl; return; }
            listaPersonajes.eliminarPersonaje(id);
            cout << "Personaje eliminado exitosamente!" << endl;
        };

        auto listarPersonajes = [&]() { listaPersonajes.mostrarLista(); };

        auto buscarPersonaje = [&]() {
            cout << "Nombre del personaje a buscar: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string nombre; getline(cin, nombre);
            Personaje* personaje = listaPersonajes.buscarPorNombre(nombre);
            if (personaje) {
                cout << "Personaje encontrado:" << endl;
                cout << "ID: " << personaje->id << " | Nombre: " << personaje->nombre << endl;
                cout << "Descripcion: " << personaje->descripcion << endl;
            } else {
                cout << "Personaje no encontrado." << endl;
            }
        };

        int opcion;
        do {
            cout << "\n==============================================================" << endl;
            cout << "                      GESTION DE PERSONAJES                        " << endl;
            cout << "==============================================================" << endl;
            cout << "1. Crear Personaje" << endl;
            cout << "2. Modificar Personaje" << endl;
            cout << "3. Eliminar Personaje" << endl;
            cout << "4. Listar Personajes" << endl;
            cout << "5. Buscar Personaje" << endl;
            cout << "6. Volver al Menu Principal" << endl;
            cout << "Seleccione una opcion: ";
            opcion = leerEntero(1,6);
            switch(opcion) {
                case 1: crearPersonaje(); break;
                case 2: modificarPersonaje(); break;
                case 3: eliminarPersonaje(); break;
                case 4: listarPersonajes(); break;
                case 5: buscarPersonaje(); break;
                case 6: cout << "Volviendo al menu principal..." << endl; break;
                default: cout << "Opcion no valida." << endl;
            }
        } while (opcion != 6);
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
        cout << "                      LISTA DE PERSONAJES                       " << endl;
        cout << "==============================================================" << endl;
        
        while (actual != nullptr) {
            cout << "[" << contador << "] " << actual->nombre << " (ID: " << actual->id << ")" << endl;
            cout << "   Tipo: ";
            switch(actual->tipo) {
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
            switch(actual->bando) {
                case Bando::POLICIA_HONESTO: cout << "Policia Honesto"; break;
                case Bando::LADRON: cout << "Ladron"; break;
                case Bando::CORRUPTO: cout << "Corrupto"; break;
            }
            cout << " | PV: " << actual->puntosVida << "/" << actual->puntosVidaMax;
            cout << " | Oro: " << actual->cargaOro << "/" << actual->capacidadOro << endl;
            cout << "   " << actual->descripcion << endl;
            cout << "   --------------------------------------------------------------" << endl;
            actual = actual->siguiente;
            contador++;
        }
        
        if (contador == 1) {
            cout << "   No hay personajes creados." << endl;
        }
    }
    
    Personaje* obtenerCabeza() { return cabeza; }
};

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
        item->id = contadorId++;
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
    
    void eliminarItem(int id) {
        if (cabeza == nullptr) return;
        
        if (cabeza->item->id == id) {
            NodoItem* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp->item;
            delete temp;
            return;
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
        }
    }

    // Extrae (sin eliminar) un Item de la lista por id y lo devuelve.
    // Devuelve nullptr si no existe.
    Item* extraerItem(int id) {
        if (cabeza == nullptr) return nullptr;
        NodoItem* prev = nullptr;
        NodoItem* cur = cabeza;
        while (cur != nullptr && cur->item->id != id) {
            prev = cur;
            cur = cur->siguiente;
        }
        if (cur == nullptr) return nullptr;
        // Desenganchar nodo
        if (prev != nullptr) prev->siguiente = cur->siguiente;
        else cabeza = cur->siguiente;
        Item* it = cur->item;
        // liberar el nodo, pero no el item
        delete cur;
        return it;
    }
    
    void mostrarLista() {
        NodoItem* actual = cabeza;
        int contador = 1;
        cout << "==============================================================" << endl;
        cout << "                        LISTA DE ITEMS                            " << endl;
        cout << "==============================================================" << endl;
        
        while (actual != nullptr) {
            cout << "[" << contador << "] " << actual->item->nombre << " (ID: " << actual->item->id << ")" << endl;
            cout << "   Tipo: " << actual->item->descripcion << endl;
            cout << "   Usos: " << actual->item->usosRestantes << "/" << actual->item->durabilidadMax << endl;
            cout << "   Efecto: " << actual->item->efecto << endl;
            cout << "   --------------------------------------------------------------" << endl;
            actual = actual->siguiente;
            contador++;
        }
        
        if (contador == 1) {
            cout << "   No hay items creados." << endl;
        }
    }
    
    NodoItem* obtenerCabeza() { return cabeza; }
};

class GrafoMapa {
private:
    Mapa* mapa;
    int contadorId;
    
public:
    GrafoMapa() : mapa(new Mapa()), contadorId(1) {}

    ~GrafoMapa() {
        // liberar estaciones y mapa
        if (mapa) {
            Estacion* cur = mapa->estaciones;
            while (cur != nullptr) {
                Estacion* next = cur->siguiente;
                delete cur;
                cur = next;
            }
            delete mapa;
            mapa = nullptr;
        }
    }
    
    void agregarEstacion(string nombre, string descripcion) {
        Estacion* nueva = new Estacion(contadorId++, nombre, descripcion);
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
    
    Estacion* buscarEstacionPorNombre(string nombre) {
        Estacion* actual = mapa->estaciones;
        while (actual != nullptr) {
            if (actual->nombre == nombre) return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void conectarEstaciones(int id1, int id2) {
        Estacion* est1 = buscarEstacion(id1);
        Estacion* est2 = buscarEstacion(id2);
        
        if (est1 && est2) {
            // Verificar que no estén ya conectadas
            if (find(est1->conexiones.begin(), est1->conexiones.end(), id2) == est1->conexiones.end()) {
                est1->conexiones.push_back(id2);
            }
            if (find(est2->conexiones.begin(), est2->conexiones.end(), id1) == est2->conexiones.end()) {
                est2->conexiones.push_back(id1);
            }
        }
    }
    
    void mostrarMapa() {
        cout << "==============================================================" << endl;
        cout << "                         MAPA DEL METRO                       " << endl;
        cout << "==============================================================" << endl;

        Estacion* actual = mapa->estaciones;
        while (actual != nullptr) {
            mostrarEstacion(actual);
            actual = actual->siguiente;
        }
    }

    // Helper: imprimir una sola estacion (separa la logica para facilitar pruebas y reuso)
    void mostrarEstacion(Estacion* actual) {
        if (!actual) return;
        cout << "Estacion " << actual->id << ": " << actual->nombre << endl;
        cout << "   " << actual->descripcion << endl;
        cout << "   Conexiones: ";
        for (size_t i = 0; i < actual->conexiones.size(); i++) {
            Estacion* conexion = buscarEstacion(actual->conexiones[i]);
            if (conexion) {
                cout << conexion->nombre;
                if (i < actual->conexiones.size() - 1) cout << ", ";
            }
        }
        cout << endl;

        cout << "   Estado: ";
        if (actual->tieneOro) cout << actual->cantidadOro << " lingotes | ";
        if (actual->tienePista) {
            if (actual->pistaSaboteada) cout << "Pista saboteada | ";
            else cout << "Tiene pista | ";
        }
        if (actual->tieneCivil) cout << "Civil presente | ";

        cout << "Personajes: ";
        for (int id : actual->personajesEnEstacion) {
            cout << "#" << id << " ";
        }
        cout << endl;
        cout << "   --------------------------------------------------------------" << endl;
    }
    
    Mapa* obtenerMapa() { return mapa; }
};

class SistemaBitacora {
private:
    stack<string> bitacora;
    
public:
    void agregarEntrada(string accion, string personaje = "", string ubicacion = "") {
        string timestamp = obtenerTimestamp();
        string entrada = "[" + timestamp + "] ";
        if (!personaje.empty()) entrada += personaje + " - ";
        entrada += accion;
        if (!ubicacion.empty()) entrada += " en " + ubicacion;
        bitacora.push(entrada);
    }
    
    void mostrarBitacora() {
        stack<string> temp = bitacora;
        vector<string> entradas;
        
        cout << "==============================================================" << endl;
        cout << "                           BITACORA DEL JUEGO                      " << endl;
        cout << "==============================================================" << endl;
        
        while (!temp.empty()) {
            entradas.push_back(temp.top());
            temp.pop();
        }
        
        for (int i = entradas.size() - 1; i >= 0; i--) {
            cout << entradas[i] << endl;
        }
    }
    
    void guardarEnArchivo() {
        ofstream archivo("bitacora_juego.txt");
        stack<string> temp = bitacora;
        vector<string> entradas;
        
        while (!temp.empty()) {
            entradas.push_back(temp.top());
            temp.pop();
        }
        
    archivo << "BITACORA DEL ROBO AL TREN DEL ORO" << endl;
    archivo << "===================================" << endl;
        
        for (int i = entradas.size() - 1; i >= 0; i--) {
            archivo << entradas[i] << endl;
        }
        archivo.close();
    }
    
private:
    string obtenerTimestamp() {
        time_t ahora = time(0);
        tm* tiempo = localtime(&ahora);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", tiempo);
        return string(buffer);
    }
};

// ==================== FUNCIONES DE INICIALIZACIÓN ====================

void inicializarHabilidadesPersonaje(Personaje* personaje) {
    switch(personaje->tipo) {
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

void inicializarMapaBase(GrafoMapa& grafoMapa) {
    grafoMapa.agregarEstacion("Estacion Central", "La estacion principal, corazon del metro");
    grafoMapa.agregarEstacion("Tunel Norte", "Pasajes oscuros y humedos al norte");
    grafoMapa.agregarEstacion("Tunel Sur", "Corredores abandonados hacia el sur");
    grafoMapa.agregarEstacion("Anden 4", "Donde ocurrio el descarrilamiento");
    grafoMapa.agregarEstacion("Sala de Maquinas", "Cuarto de mantenimiento con herramientas");
    grafoMapa.agregarEstacion("Pasaje Secreto", "Tunel oculto conocido solo por iniciados");
    grafoMapa.agregarEstacion("Boveda Antigua", "Deposito seguro ahora abandonado");
    grafoMapa.agregarEstacion("Oficinas", "Área administrativa del metro");
    grafoMapa.agregarEstacion("Salida Este", "Vía de escape hacia el este");
    grafoMapa.agregarEstacion("Salida Oeste", "Vía de escape hacia el oeste");
    
    // Conectar estaciones
    grafoMapa.conectarEstaciones(1, 2);
    grafoMapa.conectarEstaciones(1, 3);
    grafoMapa.conectarEstaciones(1, 4);
    grafoMapa.conectarEstaciones(2, 5);
    grafoMapa.conectarEstaciones(3, 6);
    grafoMapa.conectarEstaciones(4, 7);
    grafoMapa.conectarEstaciones(5, 8);
    grafoMapa.conectarEstaciones(6, 9);
    grafoMapa.conectarEstaciones(7, 10);
    grafoMapa.conectarEstaciones(8, 9);
    grafoMapa.conectarEstaciones(9, 10);
}

void crearPersonajesBase(ListaPersonajes& listaPersonajes) {
    // Policías Honestos
    listaPersonajes.agregarPersonaje(new Personaje(0, "Inspector Kaine", 
        "La ley no es una guía flexible, es el faro que nos impide perdernos en la oscuridad", 
        TipoPersonaje::INSPECTOR_JEFE, Bando::POLICIA_HONESTO, 5));
    
    listaPersonajes.agregarPersonaje(new Personaje(0, "Perito Rodríguez", 
        "Especialista en análisis forense y recolección de evidencias", 
        TipoPersonaje::PERITO_FORENSE, Bando::POLICIA_HONESTO, 3));
    
    listaPersonajes.agregarPersonaje(new Personaje(0, "Oficial Martínez", 
        "Experto en operativos de captura y enfrentamientos", 
        TipoPersonaje::OFICIAL_ASALTO, Bando::POLICIA_HONESTO, 3));
    
    // Ladrones
    listaPersonajes.agregarPersonaje(new Personaje(0, "Silas Vance", 
        "El oro no es el botín, es la llave para comprar la libertad", 
        TipoPersonaje::CEREBRO, Bando::LADRON, 8));
    
    listaPersonajes.agregarPersonaje(new Personaje(0, "El Fantasma", 
        "Se desvanece en las sombras, dejando solo recuerdos", 
        TipoPersonaje::FANTASMA, Bando::LADRON, 4));
    
    listaPersonajes.agregarPersonaje(new Personaje(0, "El Mulo", 
        "Carga con el peso del botín, silencioso y eficiente", 
        TipoPersonaje::MULO, Bando::LADRON, 15));
    
    // Policías Corruptos (inicialmente ocultos)
    listaPersonajes.agregarPersonaje(new Personaje(0, "Agente Smith", 
        "Siempre en el lugar correcto en el momento correcto... ¿demasiado correcto?", 
        TipoPersonaje::POLICIA_CORRUPTO, Bando::CORRUPTO, 4));
    
    // Inicializar habilidades para cada personaje
    Personaje* actual = listaPersonajes.obtenerCabeza();
    while (actual != nullptr) {
        inicializarHabilidadesPersonaje(actual);
        actual = actual->siguiente;
    }
}

// ==================== SISTEMA DE CRUD COMPLETO ====================

void menuGestionPersonajes(ListaPersonajes& listaPersonajes) {
    int opcion;
    do {
        cout << "\n┌─────────────────────────────────────────────────────────────────────────┐" << endl;
        cout << "│                      GESTIÓN DE PERSONAJES                             │" << endl;
        cout << "└─────────────────────────────────────────────────────────────────────────┘" << endl;
        cout << "1. Crear Personaje" << endl;
        cout << "2. Modificar Personaje" << endl;
        cout << "3. Eliminar Personaje" << endl;
        cout << "4. Listar Personajes" << endl;
        cout << "5. Buscar Personaje" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero(1,6);
        
        switch(opcion) {
            case 1: {
                string nombre, descripcion;
                int tipo, bando, capacidad;
                
                cout << "\n--- CREAR NUEVO PERSONAJE ---" << endl;
                cout << "Nombre: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, nombre);
                cout << "Descripcion: ";
                getline(cin, descripcion);
                
                cout << "Tipo (1-12): " << endl;
                cout << "1. Inspector Jefe  2. Perito Forense  3. Oficial Asalto" << endl;
                cout << "4. Analista Datos  5. Negociador      6. Policia Corrupto" << endl;
                cout << "7. Cerebro         8. Fantasma        9. Mulo" << endl;
                cout << "10. Saboteador     11. Hacker         12. Lobero" << endl;
                tipo = leerEntero(1,12);
                
                cout << "Bando (1-3): " << endl;
                cout << "1. Policia Honesto  2. Ladron  3. Corrupto" << endl;
                bando = leerEntero(1,3);
                
                cout << "Capacidad de oro: ";
                capacidad = leerEntero(1,1000);
                
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
                id = leerEntero(1,10000);
                
                Personaje* personaje = listaPersonajes.buscarPorId(id);
                if (personaje) {
                    string nuevoNombre, nuevaDesc;
                    cout << "Nuevo nombre (actual: " << personaje->nombre << "): ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
                id = leerEntero(1,10000);
                
                Personaje* personaje = listaPersonajes.buscarPorId(id);
                if (personaje) {
                    listaPersonajes.eliminarPersonaje(id);
                    cout << "Personaje eliminado exitosamente!" << endl;
                } else {
                    cout << "❌ Personaje no encontrado." << endl;
                }
                break;
            }
            
            case 4:
                listaPersonajes.mostrarLista();
                break;
                
            case 5: {
                string nombre;
                cout << "Nombre del personaje a buscar: ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
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

void menuGestionItems(ListaItems& listaItems) {
    // Helpers locales para acciones del menú
    auto crearItem = [&]() {
        string nombre, descripcion, efecto;
        int tipo, durabilidad;
        cout << "\n--- CREAR NUEVO ITEM ---" << endl;
    cout << "Nombre: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nombre);
        cout << "Descripcion: ";
        getline(cin, descripcion);
        cout << "Efecto: ";
        getline(cin, efecto);
        cout << "Durabilidad: ";
        durabilidad = leerEntero(1,1000);
        cout << "Tipo (1-20): ";
        tipo = leerEntero(1,20);
        TipoItem tipoEnum = static_cast<TipoItem>(tipo - 1);
        listaItems.agregarItem(new Item(0, nombre, descripcion, tipoEnum, durabilidad, efecto));
        cout << "Item creado exitosamente!" << endl;
    };

    auto modificarItem = [&]() {
        cout << "\n--- MODIFICAR ITEM ---" << endl;
        listaItems.mostrarLista();
        cout << "ID del item a modificar: ";
        int id = leerEntero(1,10000);
        Item* it = listaItems.buscarPorId(id);
        if (!it) { cout << "Item no encontrado." << endl; return; }
        string nombre, descripcion, efecto;
        int durabilidad;
    cout << "Nuevo nombre (actual: " << it->nombre << "): ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, nombre);
        cout << "Nueva descripcion: ";
        getline(cin, descripcion);
        cout << "Nuevo efecto: ";
        getline(cin, efecto);
        cout << "Nueva durabilidad: ";
        durabilidad = leerEntero(1,1000);
        if (!nombre.empty()) it->nombre = nombre;
        if (!descripcion.empty()) it->descripcion = descripcion;
        if (!efecto.empty()) it->efecto = efecto;
        it->durabilidad = durabilidad;
        cout << "Item modificado exitosamente." << endl;
    };

    auto eliminarItem = [&]() {
        cout << "\n--- ELIMINAR ITEM ---" << endl;
        listaItems.mostrarLista();
        cout << "ID del item a eliminar: ";
        int id = leerEntero(1,10000);
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
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
        cout << "                      GESTION DE ITEMS                        " << endl;
        cout << "==============================================================" << endl;
        cout << "1. Crear Item" << endl;
        cout << "2. Modificar Item" << endl;
        cout << "3. Eliminar Item" << endl;
        cout << "4. Listar Items" << endl;
        cout << "5. Buscar Item" << endl;
        cout << "6. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        opcion = leerEntero(1,6);

        switch(opcion) {
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

// ==================== SISTEMA DE JUEGO COMPLETO ====================

class JuegoManager {
private:
    Juego* juego;
    ListaPersonajes* listaPersonajes;
    ListaItems* listaItems;
    GrafoMapa* grafoMapa;
    SistemaBitacora bitacora;
    
public:
    JuegoManager() {
        juego = new Juego();
        listaPersonajes = new ListaPersonajes();
        listaItems = new ListaItems();
        grafoMapa = new GrafoMapa();
        
        // Inicializar datos base
        inicializarItemsBase(*listaItems);
        inicializarMapaBase(*grafoMapa);
        crearPersonajesBase(*listaPersonajes);
        juego->mapa = grafoMapa->obtenerMapa();
        
        // Distribuir oro aleatoriamente
        distribuirOroInicial();
        generarCiviles();
    }

    ~JuegoManager() {
        // limpiar equipos creados dentro de Juego
        if (juego) {
            for (int i = 0; i < 3; ++i) {
                if (juego->equipos[i]) {
                    // los miembros (Personaje*) ya son liberados por ListaPersonajes destructor
                    delete juego->equipos[i];
                    juego->equipos[i] = nullptr;
                }
            }
            // No eliminar juego->mapa aquí: grafoMapa es responsable de su Mapa
            delete juego;
            juego = nullptr;
        }

        // liberar estructuras auxiliares
        if (listaPersonajes) { delete listaPersonajes; listaPersonajes = nullptr; }
        if (listaItems) { delete listaItems; listaItems = nullptr; }
        if (grafoMapa) { delete grafoMapa; grafoMapa = nullptr; }
    }

    // Menú interactivo para gestión de items que permite asignar items a personajes
    void menuGestionItemsInteractivo() {
        int opcion;
        do {
            cout << "\n--- GESTIÓN DE IMPLEMENTOS (Interactivo) ---" << endl;
            cout << "1. Crear Item" << endl;
            cout << "2. Asignar Item a Personaje" << endl;
            cout << "3. Listar Items" << endl;
            cout << "4. Volver" << endl;
            cout << "Seleccione una opción: ";
            opcion = leerEntero(1,4);

            switch(opcion) {
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
                    durabilidad = leerEntero(1,1000);
                    cout << "Tipo (1-20): ";
                    tipo = leerEntero(1,20);
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
                    // Mostrar personajes
                    listaPersonajes->mostrarLista();
                    cout << "ID del personaje receptor: ";
                    int pid = leerEntero(1, 10000);
                    Personaje* p = listaPersonajes->buscarPorId(pid);
                    if (!p) {
                        cout << "Personaje no encontrado. Devolviendo item a la lista global." << endl;
                        listaItems->agregarItem(item);
                        break;
                    }
                    // Agregar al inventario del personaje
                    NodoItem* nodo = new NodoItem(item);
                    nodo->siguiente = p->inventario;
                    p->inventario = nodo;
                    cout << "Item " << item->nombre << " asignado a " << p->nombre << "." << endl;
                    break;
                }
                case 5: {
                    cout << "Guardar estado del juego a 'save.txt'..." << endl;
                    guardarEstado("save.txt");
                    break;
                }
                case 6: {
                    cout << "Cargar estado del juego desde 'save.txt'..." << endl;
                    cargarEstado("save.txt");
                    break;
                }
                case 3:
                    listaItems->mostrarLista();
                    break;
                case 4:
                    cout << "Volviendo..." << endl;
                    break;
            }
        } while (opcion != 4);
    }
    
    // Menú interactivo para el mapa: ver todo, buscar estación por id o por nombre
    void menuMapaInteractivo() {
        int opcion;
        do {
            cout << "\n--- MENU MAPA ---" << endl;
            cout << "1. Mostrar mapa" << endl;
            cout << "2. Ver estacion por ID" << endl;
            cout << "3. Buscar estacion por nombre" << endl;
            cout << "4. Volver" << endl;
            cout << "Seleccione una opcion: ";
            opcion = leerEntero(1,4);
            switch (opcion) {
                case 1:
                    grafoMapa->mostrarMapa();
                    break;
                case 2: {
                    cout << "ID de la estacion: ";
                    int id = leerEntero(1, 10000);
                    Estacion* est = grafoMapa->buscarEstacion(id);
                    if (est) grafoMapa->mostrarEstacion(est);
                    else cout << "Estacion no encontrada." << endl;
                    break;
                }
                case 3: {
                    cout << "Nombre de la estacion: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    string nombre; getline(cin, nombre);
                    Estacion* est = grafoMapa->buscarEstacionPorNombre(nombre);
                    if (est) grafoMapa->mostrarEstacion(est);
                    else cout << "Estacion no encontrada." << endl;
                    break;
                }
                case 4:
                    cout << "Volviendo..." << endl;
                    break;
            }
        } while (opcion != 4);
    }

    void distribuirOroInicial() {
        srand(time(0));
        juego->oroTotal = 100; // 100 lingotes totales
        
        Estacion* actual = juego->mapa->estaciones;
        int oroRestante = juego->oroTotal;
        
        while (actual != nullptr && oroRestante > 0) {
            if (rand() % 3 == 0) { // 33% de probabilidad de tener oro
                int oroEnEstacion = min(10 + rand() % 20, oroRestante);
                actual->tieneOro = true;
                actual->cantidadOro = oroEnEstacion;
                oroRestante -= oroEnEstacion;
            }
            actual = actual->siguiente;
        }
    }
    
    void generarCiviles() {
        vector<string> nombres = {"Juan", "Maria", "Carlos", "Ana", "Luis", "Elena", 
                                "Miguel", "Sofia", "David", "Laura"};
        vector<string> informaciones = {
            "Vi a alguien sospechoso corriendo hacia el tunel norte",
            "Escuche ruidos en la sala de maquinas",
            "Alguien cargaba una maleta pesada hacia la salida este",
            "Habia gente discutiendo en el anden 4",
            "Vi luces moviendose en el pasaje secreto"
        };
        
        Estacion* actual = juego->mapa->estaciones;
        while (actual != nullptr) {
            if (rand() % 2 == 0) { // 50% de probabilidad de tener civil
                string nombre = nombres[rand() % nombres.size()];
                string info = informaciones[rand() % informaciones.size()];
                juego->civiles.push_back(Civil(juego->civiles.size() + 1, nombre, info, actual->id));
                actual->tieneCivil = true;
                actual->infoCivil = info;
            }
            actual = actual->siguiente;
        }
    }
    
    void iniciarJuego() {
    cout << "INICIANDO EL JUEGO - EL ROBO AL TREN DEL ORO" << endl;
    cout << "========================================" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        
        // Asignar personajes a equipos
        asignarPersonajesAEquipos();
        
        // Posicionar personajes en el mapa
        posicionarPersonajesIniciales();
        
        // Iniciar rondas
        while (!juego->juegoTerminado && juego->rondaActual < 10) {
            juego->rondaActual++;
            ejecutarRonda();
            verificarCondicionesVictoria();
        }
        
        if (!juego->juegoTerminado) {
            determinarGanadorPorPuntos();
        }
        
        bitacora.guardarEnArchivo();
        mostrarResultadosFinales();
    }
    
    void asignarPersonajesAEquipos() {
        Personaje* actual = listaPersonajes->obtenerCabeza();
        while (actual != nullptr) {
            // Guardar siguiente original antes de cambiar punteros
            Personaje* siguienteOriginal = actual->siguiente;

            switch(actual->bando) {
                case Bando::POLICIA_HONESTO:
                    actual->siguiente = juego->equipos[0]->miembros;
                    juego->equipos[0]->miembros = actual;
                    juego->equipos[0]->cantidadMiembros++;
                    break;
                case Bando::LADRON:
                    actual->siguiente = juego->equipos[1]->miembros;
                    juego->equipos[1]->miembros = actual;
                    juego->equipos[1]->cantidadMiembros++;
                    break;
                case Bando::CORRUPTO:
                    actual->siguiente = juego->equipos[2]->miembros;
                    juego->equipos[2]->miembros = actual;
                    juego->equipos[2]->cantidadMiembros++;
                    juego->policiasCorruptos.push_back(actual->id);
                    break;
            }

            // Avanzar usando el siguiente original (no el puntero que acabamos de reasignar)
            actual = siguienteOriginal;
        }
    }
    
    void posicionarPersonajesIniciales() {
        // Posicionar policías honestos en estaciones centrales
        Personaje* policia = juego->equipos[0]->miembros;
        int estacionId = 1; // Estación Central
        while (policia != nullptr) {
            policia->posicion = estacionId;
            Estacion* estacion = grafoMapa->buscarEstacion(estacionId);
            if (estacion) {
                estacion->personajesEnEstacion.push_back(policia->id);
            }
            estacionId = (estacionId % 3) + 1; // Circular entre estaciones 1-3
            policia = policia->siguiente;
        }
        
        // Posicionar ladrones en estaciones periféricas
        Personaje* ladron = juego->equipos[1]->miembros;
        estacionId = 7; // Bóveda Antigua
        while (ladron != nullptr) {
            ladron->posicion = estacionId;
            Estacion* estacion = grafoMapa->buscarEstacion(estacionId);
            if (estacion) {
                estacion->personajesEnEstacion.push_back(ladron->id);
            }
            estacionId = (estacionId % 3) + 7; // Circular entre estaciones 7-9
            ladron = ladron->siguiente;
        }
        
        // Posicionar corruptos con policías honestos
        Personaje* corrupto = juego->equipos[2]->miembros;
        if (corrupto) {
            corrupto->posicion = 1; // Estación Central con otros policías
            Estacion* estacion = grafoMapa->buscarEstacion(1);
            if (estacion) {
                estacion->personajesEnEstacion.push_back(corrupto->id);
            }
        }
    }
    
    void ejecutarRonda() {
    cout << "\nRONDA " << juego->rondaActual << endl;
        cout << "========================================" << endl;
        
        // Procesar cada equipo
        for (int i = 0; i < 3; i++) {
            cout << "\n--- Turno de " << juego->equipos[i]->nombre << " ---" << endl;
            procesarTurnoEquipo(juego->equipos[i]);
        }
        
        // Actualizar cooldowns de habilidades
        actualizarCooldowns();
        
        // Civiles comienzan a salir después de la ronda 5
        if (juego->rondaActual > 5) {
            reducirCiviles();
        }
    }
    
    void procesarTurnoEquipo(Equipo* equipo) {
        Personaje* personaje = equipo->miembros;
        while (personaje != nullptr) {
            if (personaje->estado == Estado::ACTIVO) {
                cout << "\nTurno de " << personaje->nombre << endl;
                mostrarOpcionesTurno(personaje);
            }
            personaje = personaje->siguiente;
        }
    }
    
    void mostrarOpcionesTurno(Personaje* personaje) {
        int opcion;
        cout << "¿Qué acción deseas realizar?" << endl;
        cout << "1. Moverse" << endl;
        cout << "2. Investigar" << endl;
        cout << "3. Rastrear oro" << endl;
        cout << "4. Usar habilidad especial" << endl;
        cout << "5. Usar item" << endl;
        cout << "6. Interrogar civil" << endl;
        cout << "7. Capturar sospechoso" << endl;
        cout << "Seleccione: ";
        opcion = leerEntero(1,7);
        
        switch(opcion) {
            case 1:
                moverPersonaje(personaje);
                break;
            case 2:
                investigarEstacion(personaje);
                break;
            case 3:
                rastrearOro(personaje);
                break;
            case 4:
                usarHabilidad(personaje);
                break;
            case 5:
                usarItem(personaje);
                break;
            case 6:
                interrogarCivil(personaje);
                break;
            case 7:
                capturarSospechoso(personaje);
                break;
            default:
                cout << "❌ Opción no válida." << endl;
        }
    }
    
    void moverPersonaje(Personaje* personaje) {
        Estacion* estacionActual = grafoMapa->buscarEstacion(personaje->posicion);
        if (!estacionActual) return;
        
        cout << "Estaciones conectadas desde " << estacionActual->nombre << ":" << endl;
        for (int conexionId : estacionActual->conexiones) {
            Estacion* conexion = grafoMapa->buscarEstacion(conexionId);
            if (conexion) {
                cout << "- " << conexion->nombre << " (ID: " << conexion->id << ")" << endl;
            }
        }
        
    int destinoId;
    cout << "ID de la estacion destino: ";
    destinoId = leerEntero(1, 10000);
        
        // Verificar que la estación destino está conectada
        if (find(estacionActual->conexiones.begin(), estacionActual->conexiones.end(), destinoId) 
            != estacionActual->conexiones.end()) {
            
            // Remover de estación actual
            auto& personajes = estacionActual->personajesEnEstacion;
            personajes.erase(remove(personajes.begin(), personajes.end(), personaje->id), personajes.end());
            
            // Agregar a nueva estación
            Estacion* estacionDestino = grafoMapa->buscarEstacion(destinoId);
            if (estacionDestino) {
                personaje->posicion = destinoId;
                estacionDestino->personajesEnEstacion.push_back(personaje->id);
                bitacora.agregarEntrada("Se movió a " + estacionDestino->nombre, personaje->nombre);
                cout << personaje->nombre << " se movió a " << estacionDestino->nombre << endl;
            }
        } else {
            cout << "No puedes moverte a esa estación." << endl;
        }
    }
    
    void investigarEstacion(Personaje* personaje) {
        Estacion* estacion = grafoMapa->buscarEstacion(personaje->posicion);
        if (!estacion) return;
        
        string resultado = personaje->nombre + " investiga " + estacion->nombre + ": ";
        
        if (estacion->pistaSaboteada) {
            resultado += "Las pistas han sido saboteadas. No se encuentra información útil.";
        } else if (estacion->tienePista) {
            resultado += "Encuentra pista: " + estacion->pista;
            estacion->tienePista = false; // La pista se consume
        } else if (estacion->tieneOro) {
            resultado += "¡Encuentra " + to_string(estacion->cantidadOro) + " lingotes de oro!";
            // El oro puede ser recogido
        } else {
            resultado += "No encuentra nada de interés.";
        }
        
        cout << resultado << endl;
        bitacora.agregarEntrada(resultado);
        
        // Habilidad especial del Inspector Jefe
        if (personaje->tipo == TipoPersonaje::INSPECTOR_JEFE) {
                cout << "Intuición del Inspector Jefe: ";
            detectarCorruptosEnEstacion(personaje);
        }
    }
    
    void detectarCorruptosEnEstacion(Personaje* inspector) {
        Estacion* estacion = grafoMapa->buscarEstacion(inspector->posicion);
        if (!estacion) return;
        
        for (int personajeId : estacion->personajesEnEstacion) {
            Personaje* otro = listaPersonajes->buscarPorId(personajeId);
            if (otro && otro->bando == Bando::CORRUPTO && !otro->reveladoComoCorrupto) {
                if (rand() % 100 < 70) { // 70% de probabilidad de detectar
                    cout << "¡Sospecha que " << otro->nombre << " podría ser corrupto!" << endl;
                    bitacora.agregarEntrada("Sospecha de corrupción sobre " + otro->nombre, inspector->nombre);
                }
            }
        }
    }
    
    void rastrearOro(Personaje* personaje) {
        if (personaje->bando != Bando::LADRON && personaje->bando != Bando::CORRUPTO) {
            cout << "Solo ladrones y corruptos pueden rastrear oro." << endl;
            return;
        }
        
        Estacion* estacionActual = grafoMapa->buscarEstacion(personaje->posicion);
        if (!estacionActual) {
            cout << "No estás en una estacion valida para rastrear oro." << endl;
            return;
        }
                cout << "Rastreando oro desde " << estacionActual->nombre << "..." << endl;
        
        vector<Estacion*> estacionesConOro;
        Estacion* actual = juego->mapa->estaciones;
        
        while (actual != nullptr) {
            if (actual->tieneOro) {
                // Verificar si está en radio de 2 estaciones
                if (estaEnRango(estacionActual->id, actual->id, 2)) {
                    estacionesConOro.push_back(actual);
                }
            }
            actual = actual->siguiente;
        }
        
        if (estacionesConOro.empty()) {
            cout << "No se detecta oro en el área cercana." << endl;
        } else {
            cout << "Oro detectado en:" << endl;
            for (Estacion* estacion : estacionesConOro) {
                cout << "- " << estacion->nombre << " (" << estacion->cantidadOro << " lingotes)" << endl;
            }
        }
        
        bitacora.agregarEntrada("Rastreó oro", personaje->nombre, estacionActual->nombre);
    }
    
    bool estaEnRango(int estacionOrigen, int estacionDestino, int rango) {
        if (estacionOrigen == estacionDestino) return true;
        
        // BFS simple para verificar distancia
        queue<pair<int, int>> cola; // (estacion, distancia)
        vector<bool> visitado(juego->mapa->cantidadEstaciones + 1, false);
        
        cola.push({estacionOrigen, 0});
        visitado[estacionOrigen] = true;
        
        while (!cola.empty()) {
            auto [actual, dist] = cola.front();
            cola.pop();
            
            if (actual == estacionDestino) return dist <= rango;
            if (dist >= rango) continue;
            
            Estacion* estacion = grafoMapa->buscarEstacion(actual);
            for (int conexion : estacion->conexiones) {
                if (!visitado[conexion]) {
                    visitado[conexion] = true;
                    cola.push({conexion, dist + 1});
                }
            }
        }
        
        return false;
    }
    
    void usarHabilidad(Personaje* personaje) {
        cout << "Habilidades disponibles:" << endl;
        for (size_t i = 0; i < personaje->habilidades.size(); i++) {
            Habilidad& hab = personaje->habilidades[i];
            cout << i+1 << ". " << hab.nombre << " - " << hab.descripcion;
            if (!hab.disponible) cout << " (En cooldown: " << hab.cooldownActual << " turnos)";
            cout << endl;
        }
        
        int opcion;
        if (personaje->habilidades.empty()) {
            cout << "No tiene habilidades disponibles." << endl;
            return;
        }
        cout << "Seleccione habilidad: ";
        opcion = leerEntero(1, (int)personaje->habilidades.size());
        
        if (opcion < 1 || opcion > (int)personaje->habilidades.size()) {
            cout << "Habilidad no valida." << endl;
            return;
        }
        
        Habilidad& hab = personaje->habilidades[opcion-1];
        if (!hab.disponible) {
            cout << "Habilidad en cooldown." << endl;
            return;
        }
        
        // Ejecutar habilidad específica
        ejecutarHabilidad(personaje, hab);
        hab.disponible = false;
        hab.cooldownActual = hab.cooldown;
        
        bitacora.agregarEntrada("Usó habilidad: " + hab.nombre, personaje->nombre);
    }
    
    void ejecutarHabilidad(Personaje* personaje, Habilidad& habilidad) {
        // Implementación simplificada de habilidades
        if (habilidad.nombre == "Intuición Incorruptible") {
            detectarCorruptosEnEstacion(personaje);
        } else if (habilidad.nombre == "Sabotaje") {
            Estacion* estacion = grafoMapa->buscarEstacion(personaje->posicion);
            if (estacion) {
                estacion->pistaSaboteada = true;
                cout << "Pistas saboteadas en " << estacion->nombre << endl;
            }
        } else if (habilidad.nombre == "Mover Fichas") {
            cout << "Habilidad 'Mover Fichas' activada - dando turno extra a aliado..." << endl;
            // Implementar lógica para dar acción adicional
        } else {
            cout << "Habilidad '" << habilidad.nombre << "' ejecutada." << endl;
        }
    }
    
    void usarItem(Personaje* personaje) {
        if (!personaje->inventario) {
            cout << "❌ No tienes items en el inventario." << endl;
            return;
        }
        
        cout << "Items en inventario:" << endl;
        NodoItem* actual = personaje->inventario;
        int contador = 1;
        vector<NodoItem*> listado;
        while (actual != nullptr) {
            cout << contador << ". " << actual->item->nombre << " (Usos: " 
                << actual->item->usosRestantes << ")" << endl;
            listado.push_back(actual);
            actual = actual->siguiente;
            contador++;
        }
        
        int opcion;
        cout << "Seleccione item: ";
        opcion = leerEntero(1, contador-1);
        
        // Buscar el item seleccionado
        actual = listado[opcion-1];
        
        if (actual && actual->item->usosRestantes > 0) {
            cout << "Usando " << actual->item->nombre << ": " << actual->item->efecto << endl;
            actual->item->usosRestantes--;
            bitacora.agregarEntrada("Usó item: " + actual->item->nombre, personaje->nombre);
            
            if (actual->item->usosRestantes == 0) {
                cout << "El item se ha agotado y será removido." << endl;
                // Remover item del inventario
                NodoItem* prev = nullptr;
                NodoItem* cur = personaje->inventario;
                while (cur != nullptr && cur != actual) {
                    prev = cur;
                    cur = cur->siguiente;
                }
                if (cur != nullptr) {
                    if (prev != nullptr) {
                        prev->siguiente = cur->siguiente;
                    } else {
                        // era la cabeza del inventario
                        personaje->inventario = cur->siguiente;
                    }
                    // liberar memoria del item y nodo
                    delete cur->item;
                    delete cur;
                }
            }
        } else {
            cout << "Item no válido o sin usos restantes." << endl;
        }
    }
    
    void interrogarCivil(Personaje* personaje) {
        Estacion* estacion = grafoMapa->buscarEstacion(personaje->posicion);
        if (!estacion || !estacion->tieneCivil) {
            cout << "❌ No hay civiles para interrogar en esta estación." << endl;
            return;
        }
        
        // Buscar civil en esta estación
        for (Civil& civil : juego->civiles) {
            if (civil.estacion == personaje->posicion && !civil.interrogado) {
                cout << "Interrogando a " << civil.nombre << "..." << endl;
                cout << "Informacion obtenida: " << civil.informacion << endl;
                civil.interrogado = true;
                estacion->tieneCivil = false;
                bitacora.agregarEntrada("Interrogó a civil: " + civil.informacion, personaje->nombre);
                return;
            }
        }
        
    cout << "Todos los civiles aquí ya han sido interrogados." << endl;
    }
    
    void capturarSospechoso(Personaje* personaje) {
        if (personaje->bando != Bando::POLICIA_HONESTO) {
            cout << "❌ Solo policías honestos pueden capturar sospechosos." << endl;
            return;
        }
        
        Estacion* estacion = grafoMapa->buscarEstacion(personaje->posicion);
        vector<Personaje*> objetivos;
        
        // Buscar ladrones y corruptos revelados en la misma estación
        for (int personajeId : estacion->personajesEnEstacion) {
            Personaje* otro = listaPersonajes->buscarPorId(personajeId);
            if (otro && otro != personaje && 
                (otro->bando == Bando::LADRON || 
                (otro->bando == Bando::CORRUPTO && otro->reveladoComoCorrupto))) {
                objetivos.push_back(otro);
            }
        }
        
        if (objetivos.empty()) {
            cout << "No hay sospechosos para capturar en esta estacion." << endl;
            return;
        }
        
        cout << "Sospechosos disponibles para captura:" << endl;
        for (size_t i = 0; i < objetivos.size(); i++) {
            cout << i+1 << ". " << objetivos[i]->nombre;
            if (objetivos[i]->bando == Bando::CORRUPTO) cout << " (Corrupto)";
            cout << endl;
        }
        
        int opcion;
        cout << "Seleccione sospechoso: ";
    opcion = leerEntero(1, (int)objetivos.size());
        
        if (opcion < 1 || opcion > (int)objetivos.size()) {
            cout << "Opcion no valida." << endl;
            return;
        }
        
        Personaje* objetivo = objetivos[opcion-1];
        iniciarMiniJuegoCaptura(personaje, objetivo);
    }
    
    void iniciarMiniJuegoCaptura(Personaje* policia, Personaje* sospechoso) {
    cout << "\nMINI-JUEGO DE CAPTURA" << endl;
        cout << policia->nombre << " vs " << sospechoso->nombre << endl;
        cout << "========================================" << endl;
        
        int probabilidadExito = 50; // Base 50%
        
        // Modificadores por tipo de personaje
        if (policia->tipo == TipoPersonaje::OFICIAL_ASALTO) probabilidadExito += 20;
        if (sospechoso->tipo == TipoPersonaje::FANTASMA) probabilidadExito -= 15;
        if (sospechoso->tipo == TipoPersonaje::MULO) probabilidadExito += 10;
        
        // Juego simple de adivinanza
    int numeroSecreto = rand() % 10 + 1;
    int intento;
        
    cout << "Adivina el número secreto (1-10) para capturar: ";
    intento = leerEntero(1,10);
        
        if (intento == numeroSecreto) {
            cout << "¡CAPTURA EXITOSA! " << sospechoso->nombre << " ha sido arrestado." << endl;
            
            if (sospechoso->bando == Bando::LADRON) {
                sospechoso->estado = Estado::CAPTURADO;
                juego->ladronesCapturados++;
                if (sospechoso->cargaOro > 0) {
                    juego->oroRecuperado += sospechoso->cargaOro;
                    cout << "Se recuperaron " << sospechoso->cargaOro << " lingotes." << endl;
                    sospechoso->cargaOro = 0;
                }
            } else if (sospechoso->bando == Bando::CORRUPTO) {
                sospechoso->estado = Estado::INHABILITADO;
                juego->corruptosDescubiertos++;
                cout << "¡Policía corrupto descubierto y arrestado!" << endl;
            }
            
            // Remover de la estación
            Estacion* estacion = grafoMapa->buscarEstacion(policia->posicion);
            if (estacion) {
                auto& personajes = estacion->personajesEnEstacion;
                personajes.erase(remove(personajes.begin(), personajes.end(), sospechoso->id), personajes.end());
            }
            
            bitacora.agregarEntrada("Capturó a " + sospechoso->nombre, policia->nombre);
            
        } else {
            cout << "FALLÓ LA CAPTURA: " << sospechoso->nombre << " escapó." << endl;
            cout << "El número era: " << numeroSecreto << endl;
            
            // Mover sospechoso a estación aleatoria conectada
            Estacion* estacionActual = grafoMapa->buscarEstacion(sospechoso->posicion);
            if (estacionActual && !estacionActual->conexiones.empty()) {
                int nuevaEstacionId = estacionActual->conexiones[rand() % estacionActual->conexiones.size()];
                Estacion* nuevaEstacion = grafoMapa->buscarEstacion(nuevaEstacionId);
                
                if (nuevaEstacion) {
                    // Remover de actual
                    auto& personajes = estacionActual->personajesEnEstacion;
                    personajes.erase(remove(personajes.begin(), personajes.end(), sospechoso->id), personajes.end());
                    
                    // Agregar a nueva
                    sospechoso->posicion = nuevaEstacionId;
                    nuevaEstacion->personajesEnEstacion.push_back(sospechoso->id);
                    cout << sospechoso->nombre << " huyó a " << nuevaEstacion->nombre << endl;
                }
            }
            
            bitacora.agregarEntrada("Falló captura de " + sospechoso->nombre, policia->nombre);
        }
    }
    
    void actualizarCooldowns() {
        Personaje* actual = listaPersonajes->obtenerCabeza();
        while (actual != nullptr) {
            for (Habilidad& hab : actual->habilidades) {
                if (!hab.disponible) {
                    hab.cooldownActual--;
                    if (hab.cooldownActual <= 0) {
                        hab.disponible = true;
                        hab.cooldownActual = 0;
                    }
                }
            }
            actual = actual->siguiente;
        }
    }
    
    void reducirCiviles() {
        int civilesAEliminar = max(1, (int)juego->civiles.size() / 3);
        for (int i = 0; i < civilesAEliminar && !juego->civiles.empty(); i++) {
            Civil& civil = juego->civiles.back();
            Estacion* estacion = grafoMapa->buscarEstacion(civil.estacion);
            if (estacion) {
                estacion->tieneCivil = false;
            }
            juego->civiles.pop_back();
        }
    cout << "Algunos civiles han abandonado el metro..." << endl;
    }
    
    void verificarCondicionesVictoria() {
        // Victoria Ladrones: >70% del oro escapó
        int oroEnPosesionLadrones = 0;
        Personaje* ladron = juego->equipos[1]->miembros;
        while (ladron != nullptr) {
            oroEnPosesionLadrones += ladron->cargaOro;
            ladron = ladron->siguiente;
        }
        
        double porcentajeOroLadrones = 0.0;
        if (juego->oroTotal > 0) {
            porcentajeOroLadrones = (oroEnPosesionLadrones * 100.0) / juego->oroTotal;
        }
        
        if (porcentajeOroLadrones > 70.0) {
            cout << "\nLOS LADRONES HAN GANADO!" << endl;
            cout << "Han escapado con el " << porcentajeOroLadrones << "% del oro." << endl;
            juego->juegoTerminado = true;
            return;
        }
        
        // Victoria Policías Honestos: 90% oro recuperado O 80% ladrones capturados O todos corruptos descubiertos
    double porcentajeOroRecuperado = 0.0;
    if (juego->oroTotal > 0) porcentajeOroRecuperado = (juego->oroRecuperado * 100.0) / juego->oroTotal;
    int totalLadrones = juego->equipos[1] ? juego->equipos[1]->cantidadMiembros : 0;
    double porcentajeLadronesCapturados = 0.0;
    if (totalLadrones > 0) porcentajeLadronesCapturados = (juego->ladronesCapturados * 100.0) / totalLadrones;
        
        if (porcentajeOroRecuperado >= 90.0 || porcentajeLadronesCapturados >= 80.0 || 
            juego->corruptosDescubiertos >= juego->equipos[2]->cantidadMiembros) {
            cout << "\nLOS POLICIAS HONESTOS HAN GANADO!" << endl;
            if (porcentajeOroRecuperado >= 90.0) {
                cout << "Han recuperado el " << porcentajeOroRecuperado << "% del oro." << endl;
            } else if (porcentajeLadronesCapturados >= 80.0) {
                cout << "Han capturado el " << porcentajeLadronesCapturados << "% de los ladrones." << endl;
            } else {
                cout << "Han descubierto a todos los policías corruptos." << endl;
            }
            juego->juegoTerminado = true;
            return;
        }
        
        // Victoria Corruptos: 40-60% del oro escapó
        if (porcentajeOroLadrones >= 40.0 && porcentajeOroLadrones <= 60.0) {
            cout << "\nLOS POLICIAS CORRUPTOS HAN GANADO!" << endl;
            cout << "Escapó el " << porcentajeOroLadrones << "% del oro - suficiente para enriquecerse." << endl;
            juego->juegoTerminado = true;
            return;
        }
    }
    
    void determinarGanadorPorPuntos() {
        cout << "\n--- FIN DEL JUEGO POR LÍMITE DE RONDAS ---" << endl;
        
        // Calcular puntuaciones
        int puntosPolicias = juego->oroRecuperado + (juego->ladronesCapturados * 10);
        int puntosLadrones = 0;
        Personaje* ladron = juego->equipos[1]->miembros;
        while (ladron != nullptr) {
            puntosLadrones += ladron->cargaOro;
            ladron = ladron->siguiente;
        }
        int puntosCorruptos = puntosLadrones / 2; // Se benefician del éxito ladrón
        
        cout << "Puntuación Policías: " << puntosPolicias << endl;
        cout << "Puntuación Ladrones: " << puntosLadrones << endl;
        cout << "Puntuación Corruptos: " << puntosCorruptos << endl;
        
        if (puntosPolicias >= puntosLadrones && puntosPolicias >= puntosCorruptos) {
            cout << "🎉 ¡LOS POLICÍAS HONESTOS GANAN POR PUNTUACIÓN!" << endl;
        } else if (puntosLadrones >= puntosCorruptos) {
            cout << "🎉 ¡LOS LADRONES GANAN POR PUNTUACIÓN!" << endl;
        } else {
            cout << "🎉 ¡LOS POLICÍAS CORRUPTOS GANAN POR PUNTUACIÓN!" << endl;
        }
    }
    
    void mostrarResultadosFinales() {
        cout << "\n┌─────────────────────────────────────────────────────────────────────────┐" << endl;
        cout << "│                         RESULTADOS FINALES                             │" << endl;
        cout << "└─────────────────────────────────────────────────────────────────────────┘" << endl;
        
        cout << "Rondas jugadas: " << juego->rondaActual << endl;
        cout << "Oro total: " << juego->oroTotal << " lingotes" << endl;
        cout << "Oro recuperado: " << juego->oroRecuperado << " lingotes" << endl;
        cout << "Ladrones capturados: " << juego->ladronesCapturados << endl;
        cout << "Policías corruptos descubiertos: " << juego->corruptosDescubiertos << endl;
        
        cout << "\nEstado final de los equipos:" << endl;
        for (int i = 0; i < 3; i++) {
            cout << "\n" << juego->equipos[i]->nombre << ":" << endl;
            Personaje* actual = juego->equipos[i]->miembros;
            while (actual != nullptr) {
                cout << " - " << actual->nombre << " | ";
                switch(actual->estado) {
                    case Estado::ACTIVO: cout << "ACTIVO"; break;
                    case Estado::CAPTURADO: cout << "CAPTURADO"; break;
                    case Estado::INHABILITADO: cout << "INHABILITADO"; break;
                }
                cout << " | Oro: " << actual->cargaOro << " | PV: " << actual->puntosVida << endl;
                actual = actual->siguiente;
            }
        }
    }

    // Guardar estado simple a archivo (formato legible)
    void guardarEstado(const string& ruta) {
        ofstream out(ruta);
        if (!out.is_open()) {
            cout << "No se pudo abrir el archivo para guardar: " << ruta << endl;
            return;
        }
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
        out.close();
        cout << "Estado guardado en: " << ruta << endl;
    }

    void cargarEstado(const string& ruta) {
        ifstream in(ruta);
        if (!in.is_open()) {
            cout << "No se pudo abrir el archivo para cargar: " << ruta << endl;
            return;
        }
        string linea;
        while (getline(in, linea)) {
            if (linea.rfind("ronda ", 0) == 0) juego->rondaActual = stoi(linea.substr(6));
            else if (linea.rfind("oroTotal ", 0) == 0) juego->oroTotal = stoi(linea.substr(9));
            else if (linea.rfind("oroRecuperado ", 0) == 0) juego->oroRecuperado = stoi(linea.substr(14));
            else if (linea.rfind("ladronesCapturados ", 0) == 0) juego->ladronesCapturados = stoi(linea.substr(20));
            else if (linea.rfind("corruptosDescubiertos ", 0) == 0) juego->corruptosDescubiertos = stoi(linea.substr(24));
            else if (linea.rfind("P|",0) == 0) {
                // parse personaje
                // formato: P|id|nombre|tipo|bando|pos|estado|pv|carga|cap|revelado
                vector<string> parts;
                size_t start = 2; // after 'P|'
                size_t pos = start;
                while (true) {
                    size_t bar = linea.find('|', pos);
                    if (bar == string::npos) { parts.push_back(linea.substr(pos)); break; }
                    parts.push_back(linea.substr(pos, bar - pos));
                    pos = bar + 1;
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
    
    // Métodos de acceso para los menus
    ListaPersonajes* getListaPersonajes() { return listaPersonajes; }
    ListaItems* getListaItems() { return listaItems; }
    GrafoMapa* getGrafoMapa() { return grafoMapa; }
    SistemaBitacora* getBitacora() { return &bitacora; }
};

// ==================== MENÚ PRINCIPAL MEJORADO ====================

void mostrarMenuPrincipal() {
    cout << "\n";
    cout << "==============================================================" << endl;
    cout << "                EL ROBO AL TREN DEL ORO - MENU                " << endl;
    cout << "==============================================================" << endl;
    cout << "\n";
    cout << "Bienvenido al sistema de gestion del robo al tren del oro" << endl;
    cout << "\n";
    cout << " 1. Gestion de Personajes        4. Gestion de Mochilas" << endl;
    cout << " 2. Gestion de Implementos       5. Gestion de Mapas     " << endl;
    cout << " 3. Gestion de Equipos           6. Consultar Equipo     " << endl;
    cout << "\n";
    cout << " 7. Simular Juego Completo       8. Ver Bitacora         " << endl;
    cout << " 9. Salir del Sistema                                    " << endl;
    cout << "\n";
    cout << "Seleccione una opcion: ";
}

// ------------------ Handlers extraidos del menu principal ------------------
void handleGestionPersonajes(JuegoManager& jm) {
    menuGestionPersonajes(*jm.getListaPersonajes());
}

void handleGestionItems(JuegoManager& jm) {
    jm.menuGestionItemsInteractivo();
}

void handleGestionEquipos(JuegoManager& jm) {
    cout << "Gestion de Equipos - Proximamente..." << endl;
}

void handleGestionMochilas(JuegoManager& jm) {
    cout << "Gestion de Mochilas - Proximamente..." << endl;
}

void handleGestionMapas(JuegoManager& jm) {
    jm.menuMapaInteractivo();
}

void handleConsultarEquipo(JuegoManager& jm) {
    cout << "Consultar Equipo - Proximamente..." << endl;
}

void handleSimularJuego(JuegoManager& jm) {
    // Nota: iniciarJuego() ejecuta la simulacion completa. No se debe ejecutar
    // sin permiso expreso del usuario en este entorno. La funcion queda extraida
    // para mantener la estructura limpia.
    jm.iniciarJuego();
}

void handleVerBitacora(JuegoManager& jm) {
    jm.getBitacora()->mostrarBitacora();
}

void handleSalir(JuegoManager& jm) {
    cout << "Gracias por jugar a El Robo al Tren del Oro!" << endl;
}

int main() {
    srand(time(0)); // Inicializar semilla para números aleatorios
    
    JuegoManager juegoManager;
    int opcion;
    
    cout << "==============================================================" << endl;
    cout << "        EL ROBO AL TREN DEL ORO v1.0                          " << endl;
    cout << "  Un juego de estrategia, traicion y toma de decisiones       " << endl;
    cout << "==============================================================" << endl;
    
    this_thread::sleep_for(chrono::seconds(2));
    
    do {
        mostrarMenuPrincipal();
        opcion = leerEntero(1,9);
        
        switch(opcion) {
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
                cout << "❌ Opción no válida. Por favor, seleccione 1-9." << endl;
        }
        
    } while (opcion != 9);
    
    return 0;
}