#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "tipos.h"

/// NOMBRES DE ARCHIVOS
#define ARCHIVO_CONFIG "config.txt"
#define ARCHIVO_CARAVANA "caravana.txt"
#define ARCHIVO_JUGADORES "jugadores.dat"
#define ARCHIVO_PARTIDAS "partidas.dat"
#define ARCHIVO_INDICE "indice.dat"


/// CONFIG
int leerConfig(tConfig *config); /* Lee la config, devuelve 1 bien o 0 mal */
void mostrarConfig(const tConfig *config); /* Muestra por pantalla la config */

/// CARAVANA
void celdaAString(const tCelda *celda, char *buf);
int escribirCaravana(tCelda *celdas, int cantPosiciones);
int leerCaravana(tLista *lista, int cantPosiciones);

/// PARTIDAS
int guardarPartida(const tPartida *partida); /* Guarda el registro de una partida en partidas.dat, devuelve 1 bien o 0 mal */

#endif // ARCHIVOS_H_INCLUDED
