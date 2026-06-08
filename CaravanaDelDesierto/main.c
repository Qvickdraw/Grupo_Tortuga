#include "Juego/tipos.h"
#include "Juego/archivos.h"
#include "Juego/tablero.h"

int main()
{
    /// TESTS
    tConfig config;
    leerConfig(&config);
    mostrarConfig(&config);

    tLista lista;
    tableroGenerar(&config);
    leerCaravana(&lista, config.cantidadPosiciones);
    tableroMostrar(&lista, config.cantidadPosiciones);
    tableroDestruir(&lista);
    tableroMostrar(&lista, config.cantidadPosiciones);

    return 0;
}
