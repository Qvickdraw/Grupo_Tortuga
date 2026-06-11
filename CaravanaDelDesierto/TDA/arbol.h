#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdlib.h>
#include <string.h>

typedef int (*CMP_ARBOL)(const void *, const void *);
typedef void (*ACC_ARBOL)(void *);
typedef void (*accion)(void *, void *);
typedef struct sNodoArbol
{
    void *info;
    unsigned tam;
    struct sNodoArbol *izq;
    struct sNodoArbol *der;
} tNodoArbol;

typedef tNodoArbol *tArbol;

void arbolCrear(tArbol *arbol);
void arbolVaciar(tArbol *arbol);
int arbolEstaVacio(const tArbol *arbol);
int arbolEstaLleno(const tArbol *arbol, unsigned tam);
int arbolInsertar(tArbol *arbol, const void *dato, unsigned tam, CMP_ARBOL cmp);
void arbolRecorrerIO(const tArbol *arbol, accion acc, void *param);
int arbolBuscarElem(const tArbol *arbol, const void *clave, void *dato, unsigned tam, CMP_ARBOL cmp);
tNodoArbol *arbolBuscarNodo(const tArbol *arbol, const void *clave, CMP_ARBOL cmp);
tArbol *arbolBuscar(tArbol *arbol, const void *clave, CMP_ARBOL cmp);

#endif // ARBOL_H_INCLUDED
