
/*Te puse comentarios en lista.h, lo de movimientos entidad todavia no esta terminado. por ahora se genera el txt y el hecho de levantarlo esta a medias,
ya que estabamos esperando a ponernos de acuerdo con el struct que usariamos en la lista*/




#include "caravana.h"




int main()
{
    int codError;
    config configuracion;
    estadoJuego estadoJuego;
    cola colaMov;

    srand(time(NULL));

    codError= cargarConfiguracion(CONFIG, &configuracion);

    if(codError!=TODO_OK)
    {
        return codError;
    }

    codError=crearCaravana(configuracion);

    if(codError!=TODO_OK)
    {
        return codError;
    }

    inicializarEstadoJuego(&estadoJuego,&configuracion);

    cargarCaravanaEnLista(CARAVANA,&estadoJuego);

    crearCola(&colaMov);

    printf("=== INICIO DEL JUEGO ===\n");
    mostrarCaravana(&estadoJuego.ruta);
    printf("Presiona ENTER para comenzar...");
    getchar();

    int turno = 1;

    while(estadoJuego.victoria == 0 && estadoJuego.vidas > 0)
    {
        printf("\n========== TURNO %d ==========\n", turno);

        codError = generarMovimientos(&estadoJuego, &colaMov);
        if(codError != TODO_OK)
            return codError;

        mostrarColaMovimientos(&colaMov);

        codError = ejecutarMovimientosLista(&estadoJuego, &colaMov);
        if(codError != TODO_OK)
            return codError;

        codError = determinarNuevoEstado(&estadoJuego);

        if(codError != TODO_OK)
            return codError;

        mostrarCaravana(&estadoJuego.ruta);

        printf("Vidas: %d | Puntos: %d\n", estadoJuego.vidas, estadoJuego.puntos);

        // 7. Evaluar condiciones de corte o pausar para el próximo turno
        if(estadoJuego.victoria == 1)
        {
            printf("\n¡VICTORIA! Has llegado a la Ciudad Refugio.\n");
        }
        else if(estadoJuego.vidas <= 0)
        {
            printf("\nGAME OVER. El desierto ha reclamado tu caravana.\n");
        }
        else
        {
            printf("\nPresiona ENTER para avanzar al siguiente turno...");
            getchar(); // Pausa para que no pase todo de golpe
        }

        turno++;
    }

    vaciarCola(&colaMov);

    vaciarLista(&estadoJuego.ruta);

    if(codError!=TODO_OK)
        return codError;

    return TODO_OK;

}
