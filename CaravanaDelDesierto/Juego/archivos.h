#ifndef ARCHIVOS_H_INCLUDED
#define ARCHIVOS_H_INCLUDED

#include <stdio.h>
#include <string.h>
#include "tipos.h"

/// NOMBRES DE ARCHIVOS
#define ARCHIVO_CONFIG "Archivos/config.txt"
#define ARCHIVO_CARAVANA "Archivos/caravana.txt"
#define ARCHIVO_JUGADORES "Archivos/jugadores.dat"
#define ARCHIVO_PARTIDAS "Archivos/partidas.dat"
#define ARCHIVO_INDICE "Archivos/indice.dat"


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
