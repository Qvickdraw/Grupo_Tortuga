#include "../Juego/tipos.h"
///LISTA DOBLEMENTE ENLAZADA CIRCULAR
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
        return SIN_MEM;

    nue->info = malloc(tam);
    if (!nue->info)
    {
        free(nue);
        return SIN_MEM;
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

    return TODO_OK;
}
int listaSacar(tLista *lista, void *dato, unsigned tam)
{
    if (!*lista)
        return LISTA_VACIA;

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

    return TODO_OK;
}

///LISTA SIMPLE

void crearLista(Lista* p)
{
    *p = NULL;
}
int vaciarLista(Lista* p)
{
    int cant = 0;
    while(*p)
    {
        sNodoSimple* aux = *p;

        cant++;
        *p = aux->sig;
        free(aux->info);
        free(aux);
    }
    return cant;
}
int vaciarListaYMostrar(Lista* p, MOSTRAR MostrarLista)
{
    int cant = 0;
    while(*p)
    {
        sNodoSimple* aux = *p;

        cant++;
        *p = aux->sig;

        if(MostrarLista)
            MostrarLista(aux->info, cant);

        free(aux->info);
        free(aux);
    }
    return cant;
}
int ponerEnOrden(Lista* lista, const void* d, unsigned cantBytes, CMP_LISTA cmpLista, ACC_LISTA accLista)
{
    sNodoSimple* nuevo;

    while(*lista && cmpLista((*lista)->info, d) < 0)
        lista = &(*lista)->sig;

    if(*lista && cmpLista((*lista)->info, d) == 0)
    {
        if(accLista)
            if(!accLista(&(*lista)->info, &(*lista)->tamInfo, d, cantBytes))
                return SIN_MEM;
        return CLA_DUP;
    }

    if((nuevo = malloc(sizeof(sNodoSimple))) == NULL || (nuevo->info = malloc(cantBytes)) == NULL)
    {
        free(nuevo);
        return SIN_MEM;
    }

    memcpy(nuevo->info, d, cantBytes);
    nuevo->tamInfo = cantBytes;
    nuevo->sig = *lista;
    *lista = nuevo;

    return TODO_OK;
}

