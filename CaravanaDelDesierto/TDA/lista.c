#include "lista.h"

void listaCrear(tLista *lista)
{
    *lista = NULL;
}
void listaVaciar(tLista *lista)
{
    tNodoLista *elim;

    while (*lista)
    {
        elim = (*lista)->sig;

        if (elim == *lista)
        {
            *lista = NULL;
        }
        else
        {
            (*lista)->sig      = elim->sig;
            elim->sig->ant     = *lista;
        }

        free(elim->info);
        free(elim);
    }
}
int listaEstaVacia(const tLista *lista)
{
    return *lista == NULL;
}
int listaPoner(tLista *lista, const void *dato, unsigned tam)
{
    tNodoLista *nue = malloc(sizeof(tNodoLista));
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

    if (*lista == NULL)
    {
        nue->sig = nue;
        nue->ant = nue;
        *lista   = nue;
    }
    else
    {
        tNodoLista *primero = (*lista)->sig;

        nue->sig        = primero;
        nue->ant        = *lista;
        (*lista)->sig   = nue;
        primero->ant    = nue;

        *lista = nue;
    }

    return 1;
}
int listaSacar(tLista *lista, void *dato, unsigned tam)
{
    if (!*lista)
        return 0;

    tNodoLista *elim    = (*lista)->sig;

    memcpy(dato, elim->info, MINIMO(tam, elim->tam));

    if (elim == *lista)
    {
        *lista = NULL;
    }
    else
    {
        (*lista)->sig    = elim->sig;
        elim->sig->ant   = *lista;
    }

    free(elim->info);
    free(elim);

    return 1;
}
tNodoLista *listaSiguiente(tNodoLista *nodo)
{
    if (!nodo)
        return NULL;

    return nodo->sig;
}
tNodoLista *listaAnterior(tNodoLista *nodo)
{
    if (!nodo)
        return NULL;

    return nodo->ant;
}
