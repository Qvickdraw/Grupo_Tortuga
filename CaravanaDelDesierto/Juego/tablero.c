#include "tablero.h"

int tableroGenerar(const tConfig *config)
{
    srand((unsigned int)time(NULL));
    int i, pos, codError;
    int cantPos = config->cantidadPosiciones;



    tCelda *celdas = malloc(cantPos * sizeof(tCelda));
    if (!celdas)
    {
        printf("Error al generar celda.\n");
        return SIN_MEM;
    }

    for (i = 0; i < cantPos; i++)
    {
        (celdas + i)->numero = i + 1;
        (celdas + i)->estaInicio = 0;
        (celdas + i)->estaSalida = 0;
        (celdas + i)->tienePremio = 0;
        (celdas + i)->tieneVida = 0;
        (celdas + i)->tieneOasis = 0;
        (celdas + i)->tieneTormenta = 0;
        (celdas + i)->cantBandidos = 0;
        (celdas + i)->tieneJugador = 0;
    }

    celdas->estaInicio = 1;
    celdas->tieneJugador = 1;
    (celdas + cantPos - 1)->estaSalida = 1;


    for (i = 0; i < config->maximoPremios; i++)
    {
        do
        {
            pos = rand() % cantPos;
        } while((celdas + pos)->tienePremio || (celdas + pos)->estaInicio || (celdas + pos)->estaSalida);

        (celdas + pos)->tienePremio = 1;
    }

    for (i = 0; i < config->maximoVidasExtra; i++)
    {
        do
        {
            pos = rand() % cantPos;
        } while((celdas + pos)->tieneVida || (celdas + pos)->estaInicio || (celdas + pos)->estaSalida);

        (celdas + pos)->tieneVida = 1;
    }

    for (i = 0; i < config->maximoOasis; i++)
    {
        do
        {
            pos = rand() % cantPos;
        } while((celdas + pos)->tieneOasis || (celdas + pos)->estaInicio || (celdas + pos)->estaSalida);

        (celdas + pos)->tieneOasis = 1;
    }

    for (i = 0; i < config->maximoTormentas; i++)
    {
        do
        {
            pos = rand() % cantPos;
        } while((celdas + pos)->tieneTormenta || (celdas + pos)->tieneOasis || (celdas + pos)->estaInicio || (celdas + pos)->estaSalida);

        (celdas + pos)->tieneTormenta = 1;
    }

    for (i = 0; i < config->maximoBandidos; i++)
    {
        do
        {
            pos = rand() % cantPos;
        } while((celdas + pos)->cantBandidos > 0 || (celdas + pos)->estaInicio || (celdas + pos)->estaSalida);

        (celdas + pos)->cantBandidos++;
    }

    codError=escribirCaravana(celdas, cantPos);
    if (codError!=TODO_OK)
    {
        free(celdas);
        return codError;
    }

    free(celdas);

    return TODO_OK;
}


void tableroMostrar(tLista *lista, int cantPosiciones)
{
    if (!*lista)
    {
        printf("\nTablero vacio.\n");
        return;
    }

    tNodoLista *actual = (*lista)->sig;
    int i, j;

    printf("=== CARAVANA DEL DESIERTO ===\n");

    for (i = 0; i < cantPosiciones; i++)
    {
        tCelda *c = (tCelda *)actual->info;

        int cant = c->estaInicio + c->estaSalida + c->tienePremio
                 + c->tieneVida + c->tieneOasis + c->tieneTormenta
                 + c->cantBandidos + c->tieneJugador;

        printf("%02d: ", c->numero);

        if (cant == 0)
        {
            printf(".");
        }
        else if (cant == 1 && !c->tieneJugador)
        {
            if (c->estaInicio) printf("I");
            if (c->estaSalida) printf("S");
            if (c->tienePremio) printf("P");
            if (c->tieneVida) printf("V");
            if (c->tieneOasis) printf("O");
            if (c->tieneTormenta) printf("T");
            if (c->cantBandidos == 1) printf("B");
        }
        else
        {
            printf("[");
            if (c->estaInicio) printf("I ");
            if (c->estaSalida) printf("S ");
            if (c->tienePremio) printf("P ");
            if (c->tieneVida) printf("V ");
            if (c->tieneOasis) printf("O ");
            if (c->tieneTormenta) printf("T ");

            for (j = 0; j < c->cantBandidos; j++) printf("B ");

            if (c->tieneJugador) printf("J ");
            printf("\b]");
        }

        printf("\n");
        actual = actual->sig;
    }

    printf("=============================\n");
}

