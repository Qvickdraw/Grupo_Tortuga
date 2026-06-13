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


    arbolCrear(&indiceJugadores);

    codError=levantarIndice(ARCHIVO_INDICE, &indiceJugadores);

    if (codError!=TODO_OK)
    {
        printf("Error al levantar indice.txt\n");
        arbolVaciar(&indiceJugadores);
        return codError;
    }
    codError=leerConfig(&config);

    if (codError!=TODO_OK)
    {
        printf("Error al cargar config.txt\n");
        arbolVaciar(&indiceJugadores);
        return codError;
    }



    do
    {
        system("cls");
        printf("===================================\n");
        printf("      CARAVANA DEL DESIERTO        \n");
        printf("===================================\n");
        printf("Ingrese su nombre de jugador: ");

        scanf("%s", nombreIngresado);
        while(getchar() != '\n');

        codError = loginJugador(nombreIngresado, &jugadorActual, &offsetJugador, &indiceJugadores);
        if (codError == SIN_MEM || codError == ERR_ARCH)
        {
            printf("Error critico al acceder a la base de datos de jugadores.\n");
            arbolVaciar(&indiceJugadores);
            return codError;
        }


    }
    while(codError!=TODO_OK);


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
            if(opcionMenu>5 || opcionMenu<1)
            {
                printf("\nOpcion invalida\n");
                esperarEnter();
            }

        }
        while(opcionMenu>5 || opcionMenu<1);


        switch (opcionMenu)
        {
        case 1:
            juegoJugar(&config, jugadorActual.nombre, &puntosObtenidos);

            jugadorActual.partidasJugadas++;
            jugadorActual.puntajeTotal += puntosObtenidos;

            codError = actualizarJugador(&jugadorActual, offsetJugador);
            if (codError != TODO_OK)
            {
                printf("\nError al guardar el progreso del jugador.\n");
            }

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


    codError = guardarIndiceOrdenado(ARCHIVO_INDICE, &indiceJugadores);
    if (codError != TODO_OK)
    {
        printf("\nAdvertencia: No se pudo guardar el indice de jugadores correctamente.\n");
    }
    arbolVaciar(&indiceJugadores);
    system("cls");
    printf("===================================\n");
    printf("      CARAVANA DEL DESIERTO        \n");
    printf("===================================\n");
    printf("\n¡Gracias por jugar! Caravana del desierto cerrada correctamente.\n");
    return codError;
}
