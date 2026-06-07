#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

#include "../TDA/lista.h"

/// CONSTANTES
#define MAX_NOMBRE      50
#define MAX_HISTORIAL   500

/// TIPO DE CELDA
typedef enum
{
    CELDA_VACIA = '.',
    CELDA_INICIO = 'I',
    CELDA_SALIDA = 'S',
    CELDA_PREMIO = 'P',
    CELDA_VIDA = 'V',
    CELDA_OASIS = 'O',
    CELDA_TORMENTA = 'T'
}tTipoCelda;

/// CELDA DEL TABLERO
typedef struct
{
    int        numero;
    tTipoCelda tipo; /* que hay en la celda */
    int        tieneBandido; /* 1 si hay un bandido en la celda */
    int        tieneJugador; /* 1 si el jugador esta aqui       */
}tCelda;

/// JUGADOR (estado durante la partida)
typedef struct
{
    int vidas;
    int puntos;
    int protegido;
    int pierdeTurno;
    tNodoLista *posicion;
}tJugador;

/// BANDIDO
typedef struct
{
    int id;
    tNodoLista *posicion;
}tBandido;

/// MOVIMIENTO
typedef enum
{
    MOV_ADELANTE, /* 0 */
    MOV_ATRAS /* 1 */
}tDireccion;

typedef struct
{
    tDireccion direccion;
    int pasos;
    int esBandido; /* 1 = es movimiento de bandido, 0 = jugador */
    int idBandido;
}tMovimiento;

/// CONFIGURACION
typedef struct
{
    int cantidadPosiciones;
    int vidasInicio;
    int maximoBandidos;
    int maximoPremios;
    int maximoVidasExtra;
    int maximoOasis;
    int maximoTormentas;
}tConfig;

/// JUGADOR REGISTRADO
typedef struct
{
    char nombre[MAX_NOMBRE];
    int puntosTotal;
    int partidasJugadas;
}tJugadorRegistrado;

/// PARTIDA
typedef struct
{
    char nombreJugador[MAX_NOMBRE];
    int puntos;
    int cantMovimientos;
    int gano; /* 1 = llego a la salida, 0 = perdio */
}tPartida;

#endif // TIPOS_H_INCLUDED
