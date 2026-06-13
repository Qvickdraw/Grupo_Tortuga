#ifndef TABLERO_H_INCLUDED
#define TABLERO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#include "archivos.h"


/// TABLERO
int tableroGenerar(const tConfig *config); /* Genera la lista/tablero con sus celdas segun la config, devuelve 1 bien o 0 mal */
void tableroDestruir(tLista *lista); /* Libera toda la memoria de la lista del tablero */
void tableroMostrar(tLista *lista, int cantPosiciones); /* Muestra el tablero */
tNodoLista *tableroBuscarSalida(tLista *lista, int cantPosiciones); /* Devuelve la salida del tablero */

#endif // TABLERO_H_INCLUDED
