#include "tablero.h"

int posicionUsada(int *usadas, int cantidad, int pos)
{
    int i;

    for (i = 0; i < cantidad; i++)
    {
        if (usadas[i] == pos)
            return 1;
    }

    return 0;
}

int posicionAleatoria(int *usadas, int cantidad, int cantPos)
{
    int pos;

    do
    {
        pos = 1 + rand() % (cantPos - 2); /* entre 1 y cantPos-2 asi aseguramos que no sea la posicion del inicio (0) ni la salida (cantPos-1) */
    }while (posicionUsada(usadas, cantidad, pos));

    return pos;
}

int tableroGenerar(tLista *lista, const tConfig *config)
{
    srand((unsigned int)time(NULL));
    int i, pos, cantUsadas = 0;
    int cantPos = config->cantidadPosiciones;

    /* Total de elementos que ocupan celdas/pos */
    int totalEspeciales = config->maximoBandidos + config->maximoPremios
                          + config->maximoVidasExtra + config->maximoOasis
                          + config->maximoTormentas;

    if (totalEspeciales + 3 > cantPos) /* 3 = 1 inicio + 1 salida + 1 espacio para moverse */
    {
        printf("Error: no hay suficientes posiciones para todos los elementos.\n");
        return 0;
    }

    int *usadas = malloc(cantPos * sizeof(int)); /* Guardo las pos que ya fueron usadas */
    tCelda *celdas = malloc(cantPos * sizeof(tCelda)); /* temp para el tablero antes de insertar */

    if (!usadas || !celdas)
    {
        free(usadas);
        free(celdas);
        printf("Error: memoria insuficiente para generar el tablero.\n");
        return 0;
    }

    /* Inicio las celdas como vacias */
    for (i = 0; i < cantPos; i++)
    {
        celdas[i].numero = i + 1;
        celdas[i].tipo = CELDA_VACIA;
        celdas[i].tieneBandido = 0;
        celdas[i].tieneJugador = 0;
    }

    /* Pos 0 = inicio, pos cantPos-1 = salida */
    celdas[0].tipo = CELDA_INICIO;
    celdas[0].tieneJugador = 1; /* Jugador arranca en el incio */
    celdas[cantPos - 1].tipo = CELDA_SALIDA;

    usadas[cantUsadas++] = 0; /* Reservo la celda inicio */
    usadas[cantUsadas++] = cantPos - 1; /* Reservo la celda salida */

    /* Premios */
    for (i = 0; i < config->maximoPremios; i++)
    {
        pos = posicionAleatoria(usadas, cantUsadas, cantPos);
        celdas[pos].tipo = CELDA_PREMIO;
        usadas[cantUsadas++] = pos;
    }

    /* Vidas Extras */
    for (i = 0; i < config->maximoVidasExtra; i++)
    {
        pos = posicionAleatoria(usadas, cantUsadas, cantPos);
        celdas[pos].tipo = CELDA_VIDA;
        usadas[cantUsadas++] = pos;
    }

    /* Oasis */
    for (i = 0; i < config->maximoOasis; i++)
    {
        pos = posicionAleatoria(usadas, cantUsadas, cantPos);
        celdas[pos].tipo = CELDA_OASIS;
        usadas[cantUsadas++] = pos;
    }

    /* Tormentas, no puede estar en la primera pos */
    for (i = 0; i < config->maximoTormentas; i++)
    {
        do
        {
            pos = posicionAleatoria(usadas, cantUsadas, cantPos);
        }while (pos <= 1);

        celdas[pos].tipo = CELDA_TORMENTA;
        usadas[cantUsadas++] = pos;
    }

    /* Bandidos, no pueden estar en las 3 primeras pos */
    for (i = 0; i < config->maximoBandidos; i++)
    {
        do
        {
            pos = posicionAleatoria(usadas, cantUsadas, cantPos);
        }while (pos <= 2);

        celdas[pos].tieneBandido = 1;
        usadas[cantUsadas++] = pos;
    }

    /* Insertamos en la lista */
    listaCrear(lista);
    for (i = 0; i < cantPos; i++)
    {
        if (!listaPoner(lista, &celdas[i], sizeof(tCelda)))
        {
            free(usadas);
            free(celdas);
            printf("Error: no se pudo insertar celda en la lista.\n");
            return 0;
        }
    }

    free(usadas);
    free(celdas);

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
        printf("\nTablero Vacio.\n");
        return;
    }

    tNodoLista *inicio = (*lista)->sig;
    tNodoLista *actual = inicio;
    int i;

    printf("\n=== CARAVANA DEL DESIERTO ===\n");

    for (i = 0; i < cantPosiciones; i++)
    {
        tCelda *celda = (tCelda *)actual->info;

        printf("%02d: ", celda->numero);

        if (celda->tieneJugador)
        {
            if (celda->tipo == CELDA_VACIA || celda->tipo == CELDA_INICIO)
                printf("[J]");
            else
                printf("[%c J]", (char)celda->tipo);
        }
        else if (celda->tieneBandido && celda->tipo != CELDA_VACIA)
        {
            printf("[%c B]", (char)celda->tipo);
        }
        else if (celda->tieneBandido)
        {
            printf("B");
        }
        else
        {
            printf("%c", (char)celda->tipo);
        }

        printf("\n");
        actual = actual->sig;
    }

    printf("=============================\n");
}
