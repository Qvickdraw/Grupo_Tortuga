#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdlib.h>
#include <string.h>


#define MINIMO(a, b) ((a) < (b) ? (a) : (b))

typedef struct sNodoLista
{
    void *info;
    unsigned tam;
    struct sNodoLista *sig;
    struct sNodoLista *ant;
}tNodoLista;
typedef tNodoLista *tLista;

void listaCrear(tLista *lista);
void listaVaciar(tLista *lista);
int listaEstaVacia(const tLista *lista);
int listaPoner(tLista *lista, const void *dato, unsigned tam);
int listaSacar(tLista *lista, void *dato, unsigned tam);
tNodoLista *listaSiguiente(tNodoLista *nodo);
tNodoLista *listaAnterior(tNodoLista *nodo);


///LISTA SIMPLE

typedef int(*CMP_LISTA)(const void*, const void*);
typedef int(*ACC_LISTA)(void**, unsigned*, const void*, unsigned);
typedef void (*MOSTRAR)(const void*, int);

typedef struct sNodoSimple
{
    void *info;
    unsigned tamInfo;
    struct sNodoSimple* sig;
}sNodoSimple;

typedef sNodoSimple* Lista;

void crearLista(Lista* p);
int vaciarLista(Lista* p);
int vaciarListaYMostrar(Lista* p, MOSTRAR MostrarLista);
int ponerEnOrden(Lista* lista, const void* d, unsigned cantBytes, CMP_LISTA cmpLista, ACC_LISTA accLista);


#endif // LISTA_H_INCLUDED
