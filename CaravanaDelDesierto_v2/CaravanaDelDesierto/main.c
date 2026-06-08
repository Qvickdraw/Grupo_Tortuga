/*Te puse comentarios en lista.h, lo de movimientos entidad todavia no esta terminado. por ahora se genera el txt y el hecho de levantarlo esta a medias,
ya que estabamos esperando a ponernos de acuerdo con el struct que usariamos en la lista*/




#include "caravana.h"




int main()
{
    int codError;
    config configuracion;
    estadoJuego estadoJuego;


    srand(time(NULL));

    codError= cargarConfiguracion(CONFIG, &configuracion);

    if(codError!=TODO_OK)
    {
        return SIN_MEM;
    }

    codError=crearCaravana(configuracion);

    if(codError!=TODO_OK)
    {
        return SIN_MEM;
    }

    inicializarEstadoJuego(&estadoJuego,&configuracion);


    cargarCaravanaEnLista(CARAVANA,&estadoJuego);



    mostrarCaravana(&estadoJuego.ruta);


    vaciarLista(&estadoJuego.ruta);
    if(codError!=TODO_OK)
        return codError;






    return TODO_OK;

}
