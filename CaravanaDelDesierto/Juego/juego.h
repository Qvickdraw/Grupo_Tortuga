#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "tablero.h"
#include "archivos.h"
#include "../TDA/lista.h"
#include "../TDA/cola.h"

/// JUEGO
int juegoInicializar(tEstadoJuego *estado, const tConfig *config); /* Inicia el juego */
void juegoDestruir(tEstadoJuego *estado); /* Libera toda la memoria del estado del juego */
int juegoLanzarDado();
void esperarEnter();

/// COLA DE MOVIMIENTOS
void juegoEncolarMovJugador(tCola *cola, tDireccion dir, int pasos); /* Encola el movimiento del jugador */
void juegoEncolarMovBandidos(tCola *cola, tEstadoJuego *estado); /* Encola el movimiento de cada bandido */
int juegoDesencolarYProcesar(tCola *cola, tEstadoJuego *estado, int cantPosiciones);
/* Desencola y procesa los movs, 1 = bien - 0 = mal */

/// MOVIMIENTOS
char calcularCaminoMasCorto(tNodoLista* nodoBandido, tNodoLista* nodoJugador);
int  juegoMoverJugador(tEstadoJuego *estado, tDireccion dir, int pasos); /* Mueve al jug y verifica si salio, 1 = llego a la salida - 0 = no llego */
void juegoMoverBandido(tEstadoJuego *estado,tDireccion direccionOptima, int indiceBandido, int pasos); /* Mueve un bandido 1 paso acercandose al jugador */

/// CONTENIDO DE CELDA
void juegoAplicarEfecto(tEstadoJuego *estado); /* Aplica el efecto de la celda donde cayo el jugador */
int juegoDesencolarYProcesar(tCola *cola, tEstadoJuego *estado, int cantPosiciones);
int juegoVerificarColision(tEstadoJuego *estado, int cantPosiciones); /* Verifica colision jugador-bandido. 1 = si - 0 = no */

/// HISTORIAL
void juegoRegistrarMovimiento(char historial[][10], int *cantMovimientos, tDireccion dir, int pasos);
void juegoMostrarHistorial(char historial[][10], int cantMovimientos);

/// JUGAR
int juegoJugar(tConfig *config, const char *nombreJugador, int *puntos); /* 1 = gano - 0 = perdio. Guarda los puntos en *puntos */

#endif // JUEGO_H_INCLUDED
