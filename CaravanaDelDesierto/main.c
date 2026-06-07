/*Te puse comentarios en lista.h, lo de movimientos entidad todavia no esta terminado. por ahora se genera el txt y el hecho de levantarlo esta a medias,
ya que estabamos esperando a ponernos de acuerdo con el struct que usariamos en la lista*/




#include "caravana.h"




int main()
{
    int codError;

    codError=crearCaravana(CONFIG);

    if(codError!=TODO_OK)
        return codError;

    return TODO_OK;

}
