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

#endif // LISTA_H_INCLUDED
