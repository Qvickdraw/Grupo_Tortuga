
#include "ranking.h"
void mostrarRanking(const void *info, int pos)
{
    const tJugador *jugador = (const tJugador *)info;


    printf("%2d. | %-20s | Puntos: %-8d | Partidas: %-5d\n", pos, jugador->nombre, jugador->puntajeTotal, jugador->partidasJugadas);
}
int cmpJugadoresPorPuntos(const void *a, const void *b)
{
    const tJugador *jugadorA = (const tJugador *)a;
    const tJugador *jugadorB = (const tJugador *)b;

    int diferencia = jugadorB->puntajeTotal - jugadorA->puntajeTotal;

    if (diferencia != 0)
        return diferencia;

    int diferenciaPartidas = jugadorA->partidasJugadas - jugadorB->partidasJugadas;

    if (diferenciaPartidas != 0)
        return diferenciaPartidas;

    return strcmp(jugadorA->nombre, jugadorB->nombre);
}
int cargarJugadoresEnLista(const char *nomArchivo, Lista *lista)
{
    FILE *fp = fopen(nomArchivo, "rb");
    tJugador jugadorAux;
    int estado;

    if (!fp)
    {
        return ERR_ARCH;
    }

    while (fread(&jugadorAux, sizeof(tJugador), 1, fp) == TODO_OK)
    {
        estado = ponerEnOrden(lista, &jugadorAux, sizeof(tJugador), cmpJugadoresPorPuntos, NULL);

        if (estado == SIN_MEM)
        {

            vaciarLista(lista);
            fclose(fp);
            return estado;
        }
    }

    fclose(fp);
    return TODO_OK;
}
int mostrarRankingJugadores()
{
    Lista rankingJugadores;
    int codError;

    crearLista(&rankingJugadores);

    codError=cargarJugadoresEnLista(ARCHIVO_JUGADORES, &rankingJugadores);

    if (codError==ERR_ARCH)
    {
        printf("\nNo se pudo abrir el archivo de jugadores.\n");
        return codError;
    }
    else if (codError==SIN_MEM)
    {
        printf("\nError al crear la lista de Ranking.\n");
        return codError;
    }

    if (rankingJugadores == NULL)
    {
        printf("\nAun no hay jugadores registrados para mostrar el ranking.\n");
        return codError;
    }

    printf("\n======================= RANKING DE JUGADORES =======================\n");
    printf("NRO | %-20s | %-16s | %-15s\n", "NOMBRE", "PUNTAJE TOTAL", "PARTIDAS");
    printf("--------------------------------------------------------------------\n");


    vaciarListaYMostrar(&rankingJugadores, mostrarRanking);

    printf("====================================================================\n");

    return codError;
}


