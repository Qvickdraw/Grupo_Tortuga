
#include "registros.h"


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
        printf("Bienvenido nuevamente, %s!\n", jugadorActual->nombre);
        printf("Partidas jugadas: %d | Puntaje total: %d\n", jugadorActual->partidasJugadas, jugadorActual->puntajeTotal);
        printf("===================================\n");
        return 1;
    }
    else
    {
        strcpy(jugadorActual->nombre, nombreBuscado);
        jugadorActual->puntajeTotal = 0;
        jugadorActual->partidasJugadas = 0;

        fpJugadores = fopen(ARCHIVO_JUGADORES, "r+b");
        if (!fpJugadores)
        {
            fpJugadores = fopen(ARCHIVO_JUGADORES, "w+b");
            if (!fpJugadores)
                return 0;
        }

        fseek(fpJugadores, 0, SEEK_END);

        *offsetActual = ftell(fpJugadores);

        fwrite(jugadorActual, sizeof(tJugador), 1, fpJugadores);
        fclose(fpJugadores);

        regEncontrado.offset = *offsetActual;
        strcpy(regEncontrado.nombre, nombreBuscado);
        arbolInsertar(indice, &regEncontrado, sizeof(tRegIndice), cmpIndiceJugador);

        printf("\n===================================\n");
        printf("Jugador nuevo registrado.\nBienvenido a la Caravana, %s!\n", jugadorActual->nombre);
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

    FILE *fp = fopen(ARCHIVO_PARTIDAS, "ab");
    if (!fp) return 0;

    fwrite(nuevaPartida, sizeof(tPartida), 1, fp);

    fclose(fp);
    return 1;
}
void mostrarRegistroJugadores()
{
    FILE *fp = fopen(ARCHIVO_JUGADORES, "rb");
    tJugador jugador;

    if (!fp)
    {
        printf("\nNo se pudo abrir el archivo de jugadores o no hay registros previos.\n");
        return;
    }

    printf("\n================ REGISTRO DE JUGADORES ================\n");
    printf("%-20s | %-15s | %-15s\n", "NOMBRE", "PARTIDAS JUGADAS", "PUNTAJE TOTAL");
    printf("-------------------------------------------------------\n");


    while (fread(&jugador, sizeof(tJugador), 1, fp) == 1)
    {
        printf("%-20s | %-15d | %-15d\n",
               jugador.nombre,
               jugador.partidasJugadas,
               jugador.puntajeTotal);
    }

    printf("=======================================================\n");
    fclose(fp);
}


void mostrarRegistroPartidas()
{
    FILE *fp = fopen(ARCHIVO_PARTIDAS, "rb");
    tPartida partida;

    if (!fp)
    {
        printf("\nNo se pudo abrir el archivo de partidas o no hay partidas jugadas.\n");
        return;
    }

    printf("\n================== HISTORIAL DE PARTIDAS ==================\n");
    printf("%-20s | %-10s | %-12s | %-10s\n", "JUGADOR", "PUNTOS", "MOVIMIENTOS", "RESULTADO");
    printf("-----------------------------------------------------------\n");

    /* Leemos registro por registro hasta el fin del archivo */
    while (fread(&partida, sizeof(tPartida), 1, fp) == 1)
    {
        printf("%-20s | %-10d | %-12d | %-10s\n",
               partida.nombreJugador,
               partida.puntos,
               partida.cantMovimientos,
               partida.gano ? "Victoria" : "Derrota");
    }

    printf("===========================================================\n");
    fclose(fp);
}
