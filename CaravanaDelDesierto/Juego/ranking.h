#ifndef RANKING_H_INCLUDED
#define RANKING_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "archivos.h"

void mostrarRanking(const void *info, int pos);

int cmpJugadoresPorPuntos(const void *a, const void *b);

int cargarJugadoresEnLista(const char *nomArchivo, Lista *lista);

int mostrarRankingJugadores();

#endif // RANKING_H_INCLUDED
