#ifndef CARAVANA_H_INCLUDED
#define CARAVANA_H_INCLUDED



#define TODO_OK 1
#define SIN_MEM -1
#define ERR_ARCH -2
#define ERR_VEC -3
#define ERR_POS -4
#define CONFIG "config.txt"
#define CARAVANA "caravana.txt"


#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vectorTDA.h"
#include "lista.h"





/// estructura que se uso para el vector tda, con el q se hizo el caravana.txt

typedef struct {
    int inicio;
    int salida;
    int jugador;
    int premio;
    int vidaExtra;
    int oasis;
    int tormenta;
    int bandido;
} CasilleroGen;
/// Esta estructura la pensamos mas q nada para mantener la ubicacion conocida de los bandidos y el jugador
/// de otro modo tendriamos q recorrer la lista y es muy ineficiente, esto lo cargamos en el momento de pasar el caravana.txt a la lista. ya q en el caravana ya estan todos los
/// elementos genrados aleatoriamente
typedef struct {
    Lista ruta;
    Nodo* nodoJugador;
    Nodo* nodoInicio;
    Nodo** nodosBandidos;
    int cantBandidosVivos;
    int vidas;
    int puntos;
    int protegidoPorOasis;
    int turnoPerdido;
} estadoJuego;

/// este seria el struct que deciamos de lista, la idea era q sea robusta para q no haya problemas, simplemente al final del estado
/// agarrar y hacer un if sobre el casillero dond esta el jugador y mirar que tenemos, creo q seria bastante robusta
typedef struct {
    int numeroPosicion;
    int inicio;
    int salida;
    int jugador;
    int premio;
    int vidaExtra;
    int oasis;
    int tormenta;
    int bandido;
} casilleroJuego;

///struct q sacamos del archivo config

typedef struct
{
    int cantPosiciones;
    int cantPremios;
    int cantVidasInicio;
    int cantVidasExtra;
    int cantOasis;
    int cantTormenta;
    int cantBandido;
}config;

int crearCaravana(const char* nombreDeArchivo);
int cargarConfiguracion(const char *ruta_archivo, config *ptConfig);
int generarArchivoCaravana(VectorTDA* caravana);
void moverEntidad(Lista* ruta, Nodo** punteroEntidad, char tipoEntidad, char direccion, int cantidad_pasos);
int cargarCaravanaEnLista(const char *ruta_archivo, estadoJuego *estado);

#endif // CARAVANA_H_INCLUDED
