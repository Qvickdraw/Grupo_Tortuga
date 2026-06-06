#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#define MINIMO(a, b) ((a) < (b) ? (a) : (b))

typedef struct sNodoCola
{
    void *info;
    unsigned tam;
    struct sNodoCola *sig;
} tNodoCola;
typedef struct
{
    tNodoCola *pri;
    tNodoCola *ult;
}tCola;

void colaCrear(tCola *cola);
void colaVaciar(tCola *cola);
int colaEstaVacia(const tCola *cola);
int colaEstaLlena(const tCola *cola, unsigned tam);
int colaPoner(tCola *cola, const void *dato, unsigned tam);
int colaSacar(tCola *cola, void *dato, unsigned tam);
int colaPrimero(const tCola *cola, void *dato, unsigned tam);

#endif // COLA_H_INCLUDED
