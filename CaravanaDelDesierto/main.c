#include <stdio.h>
#include <stdlib.h>
#include "TDA/arbol.h"
#include "Juego/tipos.h"
#include "Juego/archivos.h"
#include "Juego/tablero.h"
#include "Juego/juego.h"
#include "registros/registros.h"



int main()
{
    tConfig config;
    tArbol indiceJugadores;
    tJugador jugadorActual;
    long offsetJugador;
    char nombreIngresado[MAX_NOMBRE];
    int opcionMenu = 0;
    int puntosObtenidos;

    /* 1. INICIALIZAR EL SISTEMA DE DATOS */
    arbolCrear(&indiceJugadores);
    levantarIndice(ARCHIVO_INDICE, &indiceJugadores);

    /* 2. CARGAR CONFIGURACIÓN */
    if (!leerConfig(&config))
    {
        printf("Error al cargar config.txt\n");
        return 1;
    }

    /* 3. PANTALLA DE BIENVENIDA Y LOGIN */
    system("cls"); // O la función que usen para limpiar pantalla
    printf("===================================\n");
    printf("      CARAVANA DEL DESIERTO        \n");
    printf("===================================\n");
    printf("Ingrese su nombre de jugador: ");
    scanf("%s", nombreIngresado);
    while(getchar() != '\n'); /* Limpiamos el buffer del teclado */

    /* Llamamos a tu nueva función de registros.c */
    if (!loginJugador(nombreIngresado, &jugadorActual, &offsetJugador, &indiceJugadores))
    {
        printf("Error crítico al acceder a la base de datos de jugadores.\n");
        arbolVaciar(&indiceJugadores);
        return 1;
    }

    esperarEnter();

    /* 4. BUCLE DEL MENÚ PRINCIPAL */
    while (opcionMenu != 3)
    {
        system("cls");
        printf("===================================\n");
        printf("          MENU PRINCIPAL           \n");
        printf("===================================\n");
        /* Mostramos las stats en vivo en el menú */
        printf("Jugador activo: %s | Partidas: %d | Puntos Totales: %d\n",
               jugadorActual.nombre, jugadorActual.partidasJugadas, jugadorActual.puntajeTotal);
        printf("-----------------------------------\n");
        printf("1. Jugar nueva partida\n");
        printf("2. Ver Ranking de posiciones\n");
        printf("3. Salir del juego\n");
        printf("Seleccione una opcion: ");

        scanf("%d", &opcionMenu);
        while(getchar() != '\n');

        if (opcionMenu == 1)
        {
            /* Jugamos la partida pasándole el nombre real del jugador logueado */
            juegoJugar(&config, jugadorActual.nombre, &puntosObtenidos);

            /* Actualizamos las estadísticas en RAM al terminar */
            jugadorActual.partidasJugadas++;
            jugadorActual.puntajeTotal += puntosObtenidos;

            /* GUARDAMOS EN DISCO: Pisamos su registro con los nuevos datos */
            actualizarJugador(&jugadorActual, offsetJugador);

            esperarEnter();
        }
        else if (opcionMenu == 2)
        {
            system("cls");
            printf("\n--- RANKING DE JUGADORES ---\n");
            printf("(Proximamente...)\n");
            esperarEnter();
        }
    }

    /* 5. CIERRE DEL JUEGO: Guardamos el índice ordenado y liberamos memoria */
    guardarIndiceOrdenado(ARCHIVO_INDICE, &indiceJugadores);
    arbolVaciar(&indiceJugadores);

    printf("\n¡Gracias por jugar! Caravana del desierto cerrada correctamente.\n");
    return 0;
}
