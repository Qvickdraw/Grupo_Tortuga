#include "arbol.h"

void arbolCrear(tArbol *arbol)
{
    *arbol = NULL;
}
void arbolVaciar(tArbol *arbol)
{
    if (!*arbol)
        return;

    arbolVaciar(&(*arbol)->izq);
    arbolVaciar(&(*arbol)->der);

    free((*arbol)->info);
    free(*arbol);
    *arbol = NULL;
}
int arbolEstaVacio(const tArbol *arbol)
{
    return *arbol == NULL;
}
int arbolEstaLleno(const tArbol *arbol, unsigned tam)
{
    (void)arbol;

    tNodoArbol *nue  = malloc(sizeof(tNodoArbol));
    if (!nue)
        return 1;

    void *info = malloc(tam);
    if (!info)
    {
        free(nue);
        return 1;
    }

    free(info);
    free(nue);

    return 0;
}
int arbolInsertar(tArbol *arbol, const void *dato, unsigned tam, CMP_ARBOL cmp)
{
    if (*arbol)
    {
        int comp = cmp(dato, (*arbol)->info);

        if (comp < 0)
            return arbolInsertar(&(*arbol)->izq, dato, tam, cmp);
        else if (comp > 0)
            return arbolInsertar(&(*arbol)->der, dato, tam, cmp);
        else
            return 0; /* duplicado */
    }

    tNodoArbol *nue = malloc(sizeof(tNodoArbol));
    if (!nue)
        return 0;

    nue->info = malloc(tam);
    if (!nue->info)
    {
        free(nue);
        return 0;
    }

    memcpy(nue->info, dato, tam);
    nue->tam = tam;
    nue->izq = NULL;
    nue->der = NULL;
    *arbol   = nue;

    return 1;
}
void arbolRecorrerIO(const tArbol *arbol, accion acc, void *param)
{
    if (!*arbol)
        return;

    arbolRecorrerIO(&(*arbol)->izq, acc, param); /* Va a la izquierda pasando el param */

    acc((*arbol)->info, param);                  /* Ejecuta la acción con la info y el param */

    arbolRecorrerIO(&(*arbol)->der, acc, param); /* Va a la derecha pasando el param */
}
int arbolBuscarElem(const tArbol *arbol, const void *clave, void *dato, unsigned tam, CMP_ARBOL cmp)
{
    const tArbol *p = arbol;

    while (*p)
    {
        int comp = cmp(clave, (*p)->info);

        if (comp == 0)
        {
            memcpy(dato, (*p)->info, tam < (*p)->tam ? tam : (*p)->tam);
            return 1;
        }

        p = (comp < 0) ? &(*p)->izq : &(*p)->der;
    }

    return 0;
}
tNodoArbol *arbolBuscarNodo(const tArbol *arbol, const void *clave, CMP_ARBOL cmp)
{
    const tArbol *p = arbol;

    while (*p)
    {
        int comp = cmp(clave, (*p)->info);

        if (comp == 0)
            return *p;

        p = (comp < 0) ? &(*p)->izq : &(*p)->der;
    }

    return NULL;
}
tArbol *arbolBuscar(tArbol *arbol, const void *clave, CMP_ARBOL cmp)
{
    while (*arbol)
    {
        int comp = cmp(clave, (*arbol)->info);

        if (comp == 0)
            return arbol;

        arbol = (comp < 0) ? &(*arbol)->izq : &(*arbol)->der;
    }

    return arbol;
}
