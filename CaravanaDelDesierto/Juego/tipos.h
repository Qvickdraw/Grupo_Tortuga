#ifndef TIPOS_H_INCLUDED
#define TIPOS_H_INCLUDED

#include "../TDA/lista.h"

/// CONSTANTES
#define MAX_NOMBRE      50
#define MAX_HISTORIAL   500
#define CHAR_INICIO    'I'
#define CHAR_SALIDA    'S'
#define CHAR_PREMIO    'P'
#define CHAR_VIDA      'V'
#define CHAR_OASIS     'O'
#define CHAR_TORMENTA  'T'
#define CHAR_BANDIDO   'B'
#define CHAR_JUGADOR   'J'
#define CHAR_VACIA     '.'

/// CELDA DEL TABLERO
typedef struct
{
    int numero; /* numero de posicion: 1..N */
    int estaInicio; /* 1 si es el campamento inicial */
    int estaSalida; /* 1 si es la ciudad refugio */
    int tienePremio; /* 1 si hay un premio */
    int tieneVida; /* 1 si hay vida extra */
    int tieneOasis; /* 1 si hay oasis */
    int tieneTormenta; /* 1 si hay tormenta */
    int cantBandidos; /* cantidad de bandidos en esta celda */
    int tieneJugador; /* 1 si el jugador esta aqui */
}tCelda;

/// JUGADOR (estado durante la partida)
typedef struct
{
    int vidas; /* hp */
    int puntos; /* puntos acumulados */
    int protegido; /* 1 protc oasis */
    int pierdeTurno; /* 1 pierde prox turno */
    tNodoLista *posicion; /* pos actual */
}tJugador;

/// BANDIDO
typedef struct
{
    int id;
    tNodoLista *posicion; /* pos actual */
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
