#ifndef REGISTROS_H_INCLUDED
#define REGISTROS_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Juego/tipos.h"
#include "../TDA/arbol.h"
#define ARCHIVO_JUGADORES "jugadores.dat"
#define ARCHIVO_INDICE "indice.dat"



/* Funciones de gestión del índice */
int levantarIndice(const char *nomArchIndice, tArbol *arbol);
int guardarIndiceOrdenado(const char *nomArchIndice, tArbol *arbol);
int cmpIndiceJugador(const void *a, const void *b);

/* Funciones del flujo principal */
int loginJugador(const char *nombreBuscado, tJugador *jugadorActual, long *offsetActual, tArbol *indice);


/* Funciones de actualizacion de registros */
int guardarPartidaBD(tPartida *nuevaPartida);
int actualizarJugador(tJugador *jugadorModificado, long offsetJugador);
#endif // REGISTROS_H_INCLUDED
