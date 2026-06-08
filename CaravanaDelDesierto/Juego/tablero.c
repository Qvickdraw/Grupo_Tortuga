#include "tablero.h"

int tableroGenerar(tLista *lista, const tConfig *config)
{
    srand((unsigned int)time(NULL));
    int i, pos;
    int cantPos = config->cantidadPosiciones;

    if (cantPos < 3) /* Verificamos min de pos, 1 para ini 1 para salida y otra para moverse */
    {
        printf("Error: se necesitan al menos 3 posiciones.\n");
        return 0;
    }

    tCelda *celdas = malloc(cantPos * sizeof(tCelda));
    if (!celdas)
    {
        printf("Error: memoria insuficiente.\n");
        return 0;
    }

    /* Inicializamos las celdas */
    for (i = 0; i < cantPos; i++)
    {
        celdas[i].numero = i + 1;
        celdas[i].estaInicio = 0;
        celdas[i].estaSalida = 0;
        celdas[i].tienePremio = 0;
        celdas[i].tieneVida = 0;
        celdas[i].tieneOasis = 0;
        celdas[i].tieneTormenta = 0;
        celdas[i].cantBandidos = 0;
        celdas[i].tieneJugador = 0;
    }

    /* Inicio y salida */
    celdas[0].estaInicio = 1;
    celdas[0].tieneJugador = 1;
    celdas[cantPos - 1].estaSalida = 1;

    /* Premios  */
    for (i = 0; i < config->maximoPremios; i++)
    {
        do
        {
            pos = 1 + rand() % (cantPos - 2);
        }while(celdas[pos].tienePremio);

        celdas[pos].tienePremio = 1;
    }

    /* Vidas extra */
    for (i = 0; i < config->maximoVidasExtra; i++)
    {
        do
        {
            pos = 1 + rand() % (cantPos - 2);
        }while(celdas[pos].tieneVida);
        celdas[pos].tieneVida = 1;
    }

    /* Oasis */
    for (i = 0; i < config->maximoOasis; i++)
    {
        int pos;
        do
        {
            pos = 1 + rand() % (cantPos - 2);
        }while(celdas[pos].tieneOasis);

        celdas[pos].tieneOasis = 1;
    }

    /* Tormentas */
    for (i = 0; i < config->maximoTormentas; i++)
    {
        int pos;
        do
        {
            pos = 1 + rand() % (cantPos - 2);
        }while(celdas[pos].tieneTormenta || pos <= 1);

        celdas[pos].tieneTormenta = 1;
    }

    /* Bandidos */
    for (i = 0; i < config->maximoBandidos; i++)
    {
        int pos;
        do
        {
            pos = 1 + rand() % (cantPos - 2); /* Me aseguro que no este en el inicio ni al final */
        }while(celdas[pos].cantBandidos > 0 || pos <= 2);

        celdas[pos].cantBandidos = 1;
    }

    if (!escribirCaravana(celdas, cantPos)) /* Generamos el caravana.txt */
    {
        free(celdas);
        return 0;
    }
    free(celdas);

    if (!leerCaravana(lista, cantPos)) /* Leemos caravana.txt y llenamos la lista */
        return 0;

    return 1;
}

void tableroDestruir(tLista *lista)
{
    listaVaciar(lista);
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

    printf("\n=== CARAVANA DEL DESIERTO ===\n");

    for (i = 0; i < cantPosiciones; i++)
    {
        tCelda *c = (tCelda *)actual->info;

        /* Contamos cuantos elementos hay */
        int cant = c->estaInicio + c->estaSalida + c->tienePremio
                 + c->tieneVida + c->tieneOasis + c->tieneTormenta
                 + c->cantBandidos + c->tieneJugador;

        printf("%02d:", c->numero);

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
            printf("\b]");  /* borramos el ultimo espacio y cerramos */
        }

        printf("\n");
        actual = actual->sig;
    }

    printf("=============================\n");
}
