
#include "registros.h"

/* Función de comparación adaptada a strings */
int cmpIndiceJugador(const void *a, const void *b)
{
    const tRegIndice *ia = (const tRegIndice *)a;
    const tRegIndice *ib = (const tRegIndice *)b;

    return strcmp(ia->nombre, ib->nombre);
}


void accionGuardarIndice(void *info, void *param)
{
    FILE *fp = (FILE*)param;
    fwrite(info, sizeof(tRegIndice), 1, fp);
}

int guardarIndiceOrdenado(const char *nomArchIndice, tArbol *arbol)
{
    FILE *fp = fopen(nomArchIndice, "wb");
    if(!fp) return 0;


    arbolRecorrerIO(arbol, accionGuardarIndice, fp);

    fclose(fp);
    return 1;
}


void _cargarIndiceBalanceado(tArbol *arbol, FILE *fp, int li, int ls)
{
    if(li > ls) return;

    int m = (li + ls) / 2;
    tRegIndice indAux;


    fseek(fp, m * sizeof(tRegIndice), SEEK_SET);
    fread(&indAux, sizeof(tRegIndice), 1, fp);


    arbolInsertar(arbol, &indAux, sizeof(tRegIndice), cmpIndiceJugador);


    _cargarIndiceBalanceado(arbol, fp, li, m - 1);
    _cargarIndiceBalanceado(arbol, fp, m + 1, ls);
}

int levantarIndice(const char *nomArchIndice, tArbol *arbol)
{
    FILE *fp = fopen(nomArchIndice, "rb");
    if(!fp) return 0;

    fseek(fp, 0, SEEK_END);
    int cantRegistros = ftell(fp) / sizeof(tRegIndice);

    if(cantRegistros > 0)
    {

        _cargarIndiceBalanceado(arbol, fp, 0, cantRegistros - 1);
    }

    fclose(fp);
    return 1;
}

int loginJugador(const char *nombreBuscado, tJugador *jugadorActual, long *offsetActual, tArbol *indice)
{
    tRegIndice regBusqueda;
    tRegIndice regEncontrado;
    FILE *fpJugadores;


    strcpy(regBusqueda.nombre, nombreBuscado);

    if (arbolBuscarElem(indice, &regBusqueda, &regEncontrado, sizeof(tRegIndice), cmpIndiceJugador))
    {

        fpJugadores = fopen(ARCHIVO_JUGADORES, "rb");
        if (!fpJugadores)
            return 0;


        fseek(fpJugadores, regEncontrado.offset, SEEK_SET);
        fread(jugadorActual, sizeof(tJugador), 1, fpJugadores);
        fclose(fpJugadores);


        *offsetActual = regEncontrado.offset;

        printf("\n===================================\n");
        printf("¡Bienvenido nuevamente, %s!\n", jugadorActual->nombre);
        printf("Partidas jugadas: %d | Puntaje total: %d\n",
               jugadorActual->partidasJugadas, jugadorActual->puntajeTotal);
        printf("===================================\n");
        return 1;
    }
    else
    {

        strcpy(jugadorActual->nombre, nombreBuscado);
        jugadorActual->puntajeTotal = 0;
        jugadorActual->partidasJugadas = 0;


        fpJugadores = fopen(ARCHIVO_JUGADORES, "ab");
        if (!fpJugadores)
            return 0;


        *offsetActual = ftell(fpJugadores);
        fwrite(jugadorActual, sizeof(tJugador), 1, fpJugadores);
        fclose(fpJugadores);


        regEncontrado.offset = *offsetActual;
        strcpy(regEncontrado.nombre, nombreBuscado);
        arbolInsertar(indice, &regEncontrado, sizeof(tRegIndice), cmpIndiceJugador);

        printf("\n===================================\n");
        printf("Jugador nuevo registrado.\n¡Bienvenido a la Caravana, %s!\n", jugadorActual->nombre);
        printf("===================================\n");
        return 1;
    }
}



int actualizarJugador(tJugador *jugadorModificado, long offsetJugador)
{

    FILE *fp = fopen(ARCHIVO_JUGADORES, "r+b");
    if (!fp) return 0;


    fseek(fp, offsetJugador, SEEK_SET);


    fwrite(jugadorModificado, sizeof(tJugador), 1, fp);

    fclose(fp);
    return 1;
}
int guardarPartidaBD(tPartida *nuevaPartida)
{

    FILE *fp = fopen("partidas.dat", "ab");
    if (!fp) return 0;

    fwrite(nuevaPartida, sizeof(tPartida), 1, fp);

    fclose(fp);
    return 1;
}
