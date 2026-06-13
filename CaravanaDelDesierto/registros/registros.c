
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
    if(!fp)
        return ERR_ARCH;


    arbolRecorrerIO(arbol, accionGuardarIndice, fp);

    fclose(fp);
    return TODO_OK;
}

int _cargarIndiceBalanceado(tArbol *arbol, FILE *fp, int li, int ls)
{
    if(li > ls)
        return TODO_OK;
    int codError;
    int m = (li + ls) / 2;
    tRegIndice indAux;


    fseek(fp, m * sizeof(tRegIndice), SEEK_SET);
    fread(&indAux, sizeof(tRegIndice), 1, fp);


    codError = arbolInsertar(arbol, &indAux, sizeof(tRegIndice), cmpIndiceJugador);
    if(codError != TODO_OK)
        return codError;


    codError = _cargarIndiceBalanceado(arbol, fp, li, m - 1);
    if(codError != TODO_OK)
        return codError;

    codError = _cargarIndiceBalanceado(arbol, fp, m + 1, ls);
    if(codError != TODO_OK)
        return codError;

    return TODO_OK;
}

int levantarIndice(const char *nomArchIndice, tArbol *arbol)
{



    FILE *fp = fopen(nomArchIndice, "rb");
    if (!fp)
    {
        fp = fopen(nomArchIndice, "w+b");
        if (!fp)
            return ERR_ARCH;
    }

    int codError;
    fseek(fp, 0, SEEK_END);
    int cantRegistros = ftell(fp) / sizeof(tRegIndice);

    if(cantRegistros > 0)
    {
        codError = _cargarIndiceBalanceado(arbol, fp, 0, cantRegistros - 1);
        if(codError != TODO_OK)
        {
            fclose(fp);
            arbolVaciar(arbol);
            return codError;
        }
    }

    fclose(fp);
    return TODO_OK;
}

int loginJugador(const char *nombreBuscado, tJugador *jugadorActual, long *offsetActual, tArbol *indice)
{
    tRegIndice regBusqueda;
    tRegIndice regEncontrado;
    FILE *fpJugadores;
    int codError;
    char confirmacion;


    strcpy(regBusqueda.nombre, nombreBuscado);

    if (arbolBuscarElem(indice, &regBusqueda, &regEncontrado, sizeof(tRegIndice), cmpIndiceJugador))
    {
        printf("\n===================================\n");
        printf("Se encontro un registro para el jugador: %s\n", regEncontrado.nombre);
        printf("Sos vos este jugador? (S/N): ");
        do
        {
            scanf(" %c", &confirmacion);
            while(getchar() != '\n');
            confirmacion = toupper(confirmacion);

            if (confirmacion != 'S' && confirmacion != 'N')
            {
                printf("Opcion invalida. Por favor ingresa S o N: ");
            }
        }
        while (confirmacion != 'S' && confirmacion != 'N');

        if (confirmacion == 'S')
        {
            fpJugadores = fopen(ARCHIVO_JUGADORES, "rb");
            if (!fpJugadores)
                return ERR_ARCH;

            fseek(fpJugadores, regEncontrado.offset, SEEK_SET);
            fread(jugadorActual, sizeof(tJugador), 1, fpJugadores);
            fclose(fpJugadores);

            *offsetActual = regEncontrado.offset;

            printf("\n===================================\n");
            printf("Bienvenido nuevamente, %s!\n", jugadorActual->nombre);
            printf("Partidas jugadas: %d | Puntaje total: %d\n", jugadorActual->partidasJugadas, jugadorActual->puntajeTotal);
            printf("===================================\n");

            return TODO_OK;
        }
        else
        {
            return JUGADOR_EQUIVOCADO;
        }

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
                return ERR_ARCH;
        }

        fseek(fpJugadores, 0, SEEK_END);

        *offsetActual = ftell(fpJugadores);

        if (fwrite(jugadorActual, sizeof(tJugador), 1, fpJugadores) != 1)
        {
            fclose(fpJugadores);
            return ERR_ARCH;
        }
        fclose(fpJugadores);

        regEncontrado.offset = *offsetActual;
        strcpy(regEncontrado.nombre, nombreBuscado);
        codError = arbolInsertar(indice, &regEncontrado, sizeof(tRegIndice), cmpIndiceJugador);
        if (codError != TODO_OK)
            return codError;

        printf("\n===================================\n");
        printf("Jugador nuevo registrado.\nBienvenido a la Caravana, %s!\n", jugadorActual->nombre);
        printf("===================================\n");
        return TODO_OK;
    }
}

int actualizarJugador(tJugador *jugadorModificado, long offsetJugador)
{

    FILE *fp = fopen(ARCHIVO_JUGADORES, "r+b");
    if (!fp)
        return ERR_ARCH;


    fseek(fp, offsetJugador, SEEK_SET);


    fwrite(jugadorModificado, sizeof(tJugador), 1, fp);

    fclose(fp);
    return TODO_OK;
}
int guardarPartidaBD(tPartida *nuevaPartida)
{

    FILE *fp = fopen(ARCHIVO_PARTIDAS, "ab");
    if (!fp)
        return ERR_ARCH;

    fwrite(nuevaPartida, sizeof(tPartida), 1, fp);

    fclose(fp);
    return TODO_OK;
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


