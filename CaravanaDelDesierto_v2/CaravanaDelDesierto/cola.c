#include "caravana.h"

void crearCola(cola *p)
{
    p->prim=NULL;
    p->ult=NULL;
}
int colaVacia(cola *p)
{
    return p->prim == NULL;
}
int colaLLena(cola *p, unsigned cantBytes)
{
    nodoCola* aux = malloc(sizeof(nodoCola));
    void* info = malloc(cantBytes);
    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}
int ponerEnCola(cola *p, const void *d, unsigned cantBytes)
{
    nodoCola *aux = malloc(sizeof(nodoCola));
    if(!aux)
        return SIN_MEM;
    aux->info=malloc(cantBytes);
    if(!aux->info)
    {
        free(aux);
        return SIN_MEM;
    }
    memcpy(aux->info,d,cantBytes);
    aux->tamInfo=cantBytes;
    aux->sig=NULL;
    if(p->ult)
        p->ult->sig=aux;
    else
        p->prim=aux;
    p->ult=aux;
    return TODO_OK;
}
int sacarDeCola(cola* p, void* d, unsigned cantBytes)
{
    nodoCola *aux= p->prim;
    if(!aux)
        return SIN_ELEM;
    memcpy(d,aux->info,minimo(aux->tamInfo,cantBytes));
    p->prim=p->prim->sig;
    free(aux->info);
    free(aux);


    if(!p->prim)
    {
        p->ult=NULL;
    }
    return TODO_OK;
}
int verPrimeroCola(const cola* p, void* d, unsigned cantBytes)
{
    if(!p->prim)
        return -1;
    memcpy(d,p->prim->info,minimo(p->prim->tamInfo,cantBytes));
    return TODO_OK;
}
void vaciarCola(cola* p)
{
    nodoCola *aux;
    while(p->prim)
    {
        aux=p->prim;
        p->prim=aux->sig;
        free(aux->info);
        free(aux);
    }
    p->ult=NULL;
}
void mostrarColaMovimientos(cola* p)
{
    cola colaAux;
    crearCola(&colaAux);
    movimiento movActual;

    printf("\n--- COLA DE MOVIMIENTOS DEL TURNO ---\n");


    if (colaVacia(p))
    {
        printf("La cola esta vacia.\n");
    }
    else
    {
        while (sacarDeCola(p, &movActual, sizeof(movimiento)) == TODO_OK)
        {
            if (movActual.entidad == 'J')
            {
                printf("Jugador : %c%d\n", movActual.direccion, movActual.cantidad);
            }
            else if (movActual.entidad == 'B')
            {
                printf("Bandido : %c%d\n", movActual.direccion, movActual.cantidad);
            }

            ponerEnCola(&colaAux, &movActual, sizeof(movimiento));
        }

        while (sacarDeCola(&colaAux, &movActual, sizeof(movimiento)) == TODO_OK)
        {
            ponerEnCola(p, &movActual, sizeof(movimiento));
        }
    }

    printf("-------------------------------------\n");
}
