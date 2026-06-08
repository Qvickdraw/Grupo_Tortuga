#include "caravana.h"
void crearLista(Lista* p)
{

    *p = NULL;
}

int obtenerDatoLista(Nodo* nodo_objetivo, void* destino, unsigned cantBytes)
{

    if(nodo_objetivo == NULL || nodo_objetivo->info == NULL || destino == NULL)
    {
        return ERR_POS;
    }


    memcpy(destino, nodo_objetivo->info, cantBytes);

    return TODO_OK;
}


int actualizarDatoLista(Nodo* nodo_objetivo, const void* origen, unsigned cantBytes)
{

    if(nodo_objetivo == NULL || nodo_objetivo->info == NULL || origen == NULL)
    {
        return ERR_POS;
    }


    memcpy(nodo_objetivo->info, origen, cantBytes);

    return TODO_OK;
}

int vaciarLista(Lista* p)
{
    int cant = 0;


    if(*p != NULL)
    {
        Nodo* act = *p;
        Nodo* aux;


        act->ant->sig = NULL;


        while(act != NULL)
        {

            aux = act->sig;


            free(act->info);

            free(act);


            act = aux;
            cant++;
        }


        *p = NULL;
    }


    return cant;
}
void avanzarEnLista(Lista* ruta, Nodo** punteroAct, char direccion, int cantidad_pasos)
{

    if (ruta == NULL || *ruta == NULL || punteroAct == NULL || *punteroAct == NULL)
    {
        return;
    }

    for (int i = 0; i < cantidad_pasos; i++)
    {
        if (direccion == 'F')
        {
            *punteroAct = (*punteroAct)->sig;
        }
        else if (direccion == 'B')
        {
            *punteroAct = (*punteroAct)->ant;
        }
    }
}



int ponerAlFinal(Lista* p, const void* d, unsigned cantBytes)
{
    Nodo* nue;


    if((nue = malloc(sizeof(Nodo))) == NULL ||
       (nue->info = malloc(cantBytes)) == NULL)
    {
        if (nue != NULL) free(nue);
        return SIN_MEM;
    }


    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;


    if(*p == NULL)
    {

        nue->sig = nue;
        nue->ant = nue;


        *p = nue;
    }
    else
    {

        Nodo* primero = *p;
        Nodo* ultimo = primero->ant;


        nue->sig = primero;
        nue->ant = ultimo;


        ultimo->sig = nue;
        primero->ant = nue;


    }

    return TODO_OK;
}
