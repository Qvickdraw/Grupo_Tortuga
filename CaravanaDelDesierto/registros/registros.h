#ifndef REGISTROS_H_INCLUDED
#define REGISTROS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../Juego/tipos.h"

#define ARCHIVO_JUGADORES "jugadores.dat"
#define ARCHIVO_PARTIDAS "partidas.dat"
#define ARCHIVO_INDICE "indice.dat"




int levantarIndice(const char *nomArchIndice, tArbol *arbol);
int guardarIndiceOrdenado(const char *nomArchIndice, tArbol *arbol);
int _cargarIndiceBalanceado(tArbol *arbol, FILE *fp, int li, int ls);
int cmpIndiceJugador(const void *a, const void *b);

int loginJugador(const char *nombreBuscado, tJugador *jugadorActual, long *offsetActual, tArbol *indice);

int guardarPartidaBD(tPartida *nuevaPartida);
int actualizarJugador(tJugador *jugadorModificado, long offsetJugador);

void mostrarRegistroJugadores();
void mostrarRegistroPartidas();


#endif // REGISTROS_H_INCLUDED
