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
    tableroGenerar(&lista, &config);
    tableroMostrar(&lista, config.cantidadPosiciones);
    guardarCaravana(&lista, config.cantidadPosiciones);
    tableroDestruir(&lista);
    tableroMostrar(&lista, config.cantidadPosiciones);

    return 0;
}
