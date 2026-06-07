#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tipos.h"
#include "archivos.h"
#include "../TDA/lista.h"

/// TABLERO
int tableroGenerar(tLista *lista, const tConfig *config); /* Genera la lista/tablero con sus celdas segun la config, devuelve 1 bien o 0 mal */
void tableroDestruir(tLista *lista); /* Libera toda la memoria de la lista del tablero */
void tableroMostrar(tLista *lista, int cantPosiciones); /* Muestra el tablero */

int posicionUsada(int *usadas, int cantidad, int pos); /* Devuelve 1 si la posicion pos ya fue usada en el arreglo */
int posicionAleatoria(int *usadas, int cantidad, int cantPos); /* Genera una posicion aleatoria que no este usada */

#endif // TABLERO_H_INCLUDED
