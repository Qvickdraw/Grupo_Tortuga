#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

typedef struct sNodo{
    void* info;
    unsigned tamInfo;
    struct sNodo *sig;
    struct sNodo *ant;
} Nodo;

typedef Nodo* Lista;


void crearLista(Lista* p);
int obtenerDatoLista(Nodo* nodo_objetivo, void* destino, unsigned cantBytes);
int actualizarDatoLista(Nodo* nodo_objetivo, const void* origen, unsigned cantBytes);
int vaciarLista(Lista* p);
void avanzarEnLista(Lista* ruta, Nodo** punteroAct, char direccion, int cantidad_pasos);
int ponerAlFinal(Lista* p, const void* d, unsigned cantBytes);
#endif // LISTA_H_INCLUDED
