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

/// ESTADO DE JUEGO
typedef struct
{
    tLista ruta; /* tablero */
    tNodoLista *nodoJugador; /* posicion actual del jugador */
    tNodoLista *nodoInicio; /* referencia al campamento inicial */
    tNodoLista *nodoSalida; /* referencia a la ciudad refugio   */
    tNodoLista **nodosBandidos; /* vec bandidos */
    int cantBandidosVivos; /* cantidad de bandidos activos */
    int vidas; /* hp del jugador */
    int puntos; /* puntos acumulados en la partida */
    int protegido; /* 1 = tiene proteccion de oasis */
    int pierdeTurno; /* 1 = pierde el proximo turno */
}tEstadoJuego;

/// MOVIMIENTO ----REMOVER COMPLETAMENTE ----- REEMPLAZAR CON CHAR
typedef enum
{
    MOV_ADELANTE, /* 0 */
    MOV_ATRAS /* 1 */
}tDireccion;

typedef struct
{
    tDireccion direccion; /* adelante o atras */
    int pasos; /* cant de posiciones */
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

/// JUGADOR REGISTRADO (arbol)
typedef struct
{
    char nombre[MAX_NOMBRE];
    int puntosTotal; /* acu de todas las partidas */
    int partidasJugadas; /* cant de partidas */
}tJugadorRegistrado;

/// PARTIDA
typedef struct
{
    char nombreJugador[MAX_NOMBRE];
    int puntos;
    int cantMovimientos;
    int gano; /* 1 = llego a la salida, 0 = perdio */
}tPartida;


typedef struct
{
    char nombre[MAX_NOMBRE];
    int puntajeTotal;
    int partidasJugadas;
} tJugador;

////
typedef struct
{
    char nombre[MAX_NOMBRE]; /* La clave de búsqueda */
    long offset;             /* Posición física en jugadores.dat */
} tRegIndice;

#endif // TIPOS_H_INCLUDED
