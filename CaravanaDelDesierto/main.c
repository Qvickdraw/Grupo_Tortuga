#include <stdio.h>
#include <stdlib.h>

#include "TDA/arbol.h"
#include "Juego/tipos.h"
#include "Juego/archivos.h"
#include "Juego/tablero.h"
#include "Juego/juego.h"
#include "Juego/ranking.h"
#include "registros/registros.h"



int main()
{
    tConfig config;
    tArbol indiceJugadores;
    tJugador jugadorActual;
    long offsetJugador;
    char nombreIngresado[MAX_NOMBRE];
    int opcionMenu = 0,codError;
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


    while (opcionMenu != 5)
    {
        do
        {
            system("cls");
            printf("===================================\n");
            printf("          MENU PRINCIPAL           \n");
            printf("===================================\n");

            printf("Jugador activo: %s | Partidas: %d | Puntos Totales: %d\n",
                   jugadorActual.nombre, jugadorActual.partidasJugadas, jugadorActual.puntajeTotal);
            printf("-----------------------------------\n");
            printf("1. Jugar nueva partida\n");
            printf("2. Ver Registro de jugadores\n");
            printf("3. Ver Registro de partidas\n");
            printf("4. Ver Ranking de jugadores\n");
            printf("5. Salir del juego\n");
            printf("Seleccione una opcion: ");

            scanf("%d", &opcionMenu);
            while(getchar() != '\n');
            if(opcionMenu>5 || opcionMenu<0)
            {
                printf("\nOpcion invalida\n");
                esperarEnter();
            }

        }while(opcionMenu>5 || opcionMenu<0);


        switch (opcionMenu)
        {
        case 1:
            juegoJugar(&config, jugadorActual.nombre, &puntosObtenidos);

            jugadorActual.partidasJugadas++;
            jugadorActual.puntajeTotal += puntosObtenidos;

            actualizarJugador(&jugadorActual, offsetJugador);

            esperarEnter();
            break;
        case 2:
            system("cls");
            printf("\n--- REGISTRO DE JUGADORES ---\n");

            mostrarRegistroJugadores();

            esperarEnter();
            break;
        case 3:
            system("cls");
            printf("\n--- REGISTRO DE PARTIDAS ---\n");

            mostrarRegistroPartidas();

            esperarEnter();
            break;
        case 4:
            system("cls");
            printf("\n--- RANKING DE JUGADORES ---\n");

            codError=mostrarRankingJugadores();
            if(codError!=TODO_OK)
                return codError;

            esperarEnter();
            break;
        }

    }


    guardarIndiceOrdenado(ARCHIVO_INDICE, &indiceJugadores);
    arbolVaciar(&indiceJugadores);
    system("cls");
    printf("===================================\n");
    printf("      CARAVANA DEL DESIERTO        \n");
    printf("===================================\n");
    printf("\n¡Gracias por jugar! Caravana del desierto cerrada correctamente.\n");
    return 0;
}
