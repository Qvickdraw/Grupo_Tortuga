#include "Juego/tipos.h"
#include "Juego/archivos.h"
#include "Juego/tablero.h"
#include "Juego/juego.h"

int main()
{
    /// TESTS
    tConfig config;
    leerConfig(&config);
    mostrarConfig(&config);
    esperarEnter();

//    tLista lista;
//    tableroGenerar(&config);
//    leerCaravana(&lista, config.cantidadPosiciones);
//    tableroMostrar(&lista, config.cantidadPosiciones);
//    tableroDestruir(&lista);
//    tableroMostrar(&lista, config.cantidadPosiciones);

    char name[] = "Jisus";
    int puntos;
    juegoJugar(&config, name, &puntos);

    return 0;
}
