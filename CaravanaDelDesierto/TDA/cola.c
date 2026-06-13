#include "../Juego/tipos.h"

void colaCrear(tCola *cola)
{
    cola->pri = cola->ult = NULL;
}
void colaVaciar(tCola *cola)
{
    tNodoCola *aux;

    while (cola->pri)
    {
        aux       = cola->pri;
        cola->pri = aux->sig;
        free(aux->info);
        free(aux);
    }

    cola->ult = NULL;
}
int colaEstaVacia(const tCola *cola)
{
    return cola->pri == NULL;
}
int colaEstaLlena(const tCola *cola, unsigned tam)
{
    (void)cola;

    tNodoCola *aux = malloc(sizeof(tNodoCola));
    void      *inf = malloc(tam);

    int llena = (aux == NULL || inf == NULL);

    free(aux);
    free(inf);

    return llena;
}
int colaPoner(tCola *cola, const void *dato, unsigned tam)
{
    tNodoCola *nue = malloc(sizeof(tNodoCola));
    if (!nue)
        return SIN_MEM;

    nue->info = malloc(tam);
    if (!nue->info)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, dato, tam);
    nue->tam = tam;
    nue->sig = NULL;

    if (cola->ult)
        cola->ult->sig = nue;
    else
        cola->pri = nue;

    cola->ult = nue;

    return TODO_OK;
}
int colaSacar(tCola *cola, void *dato, unsigned tam)
{
    tNodoCola *aux = cola->pri;

    if (!aux)
        return COLA_VACIA;

    cola->pri = aux->sig;

    memcpy(dato, aux->info, MINIMO(tam, aux->tam));

    free(aux->info);
    free(aux);

    if (cola->pri == NULL)
        cola->ult = NULL;

    return TODO_OK;
}
int colaPrimero(const tCola *cola, void *dato, unsigned tam)
{
    if (!cola->pri)
        return COLA_VACIA;

    memcpy(dato, cola->pri->info, MINIMO(tam, cola->pri->tam));

    return TODO_OK;
}
