#include "caravana.h"

int crearCaravana(config datosConfig)  //////////////
{

    int salida, codError;
    unsigned pos;
    CasilleroGen aux;
    VectorTDA caravana;


    codError=crearVectorInicializado(&caravana, datosConfig.cantPosiciones, sizeof(CasilleroGen));

    if(codError!=TODO_OK)
    {
        return SIN_MEM;
    }


    salida=datosConfig.cantPosiciones-1;

    while(datosConfig.cantBandido > 0)
    {
        pos = rand() % datosConfig.cantPosiciones;


        if(pos != 0 && pos != salida)
        {

            obtenerDeVector(&caravana, &aux, pos);


            aux.bandido++;


            actualizarEnVector(&caravana, &aux, pos);


            datosConfig.cantBandido--;
        }
    }

    while(datosConfig.cantOasis>0)
    {
        pos = rand() % datosConfig.cantPosiciones;


        if(pos != 0 && pos != salida)
        {
            obtenerDeVector(&caravana, &aux, pos);
            if(aux.oasis!=1)
            {
                aux.oasis=1;
                actualizarEnVector(&caravana, &aux, pos);
                datosConfig.cantOasis--;
            }
        }

    }
    while(datosConfig.cantVidasExtra>0)
    {

        pos = rand() % datosConfig.cantPosiciones;


        if(pos != 0 && pos != salida)
        {
            obtenerDeVector(&caravana, &aux, pos);
            if(aux.vidaExtra!=1)
            {
                aux.vidaExtra=1;
                actualizarEnVector(&caravana, &aux, pos);
                datosConfig.cantVidasExtra--;
            }
        }
    }

    while(datosConfig.cantPremios>0)
    {
        pos = rand() % datosConfig.cantPosiciones;
        if(pos != 0 && pos != salida)
        {
            obtenerDeVector(&caravana, &aux, pos);
            if(aux.premio!=1)
            {
                aux.premio=1;
                actualizarEnVector(&caravana, &aux, pos);
                datosConfig.cantPremios--;
            }
        }
    }


    while(datosConfig.cantTormenta>0)
    {
        pos = rand() % datosConfig.cantPosiciones;

        if(pos != 0 && pos != salida)
        {
            obtenerDeVector(&caravana, &aux, pos);
            if(aux.tormenta!=1 && aux.oasis!=1)
            {
                aux.tormenta=1;
                actualizarEnVector(&caravana, &aux, pos);
                datosConfig.cantTormenta--;
            }
        }
    }




    obtenerDeVector(&caravana, &aux, 0);
    aux.inicio = 1;
    aux.jugador = 1;
    actualizarEnVector(&caravana, &aux, 0);


    obtenerDeVector(&caravana, &aux, salida);
    aux.salida = 1;
    actualizarEnVector(&caravana, &aux, salida);

    codError=generarArchivoCaravana(&caravana);
    destruirVector(&caravana);

    if(codError!=TODO_OK)
        return codError;




    return TODO_OK;

}

int cargarConfiguracion(const char *ruta_archivo, config *ptConfig)
{
    char linea[100];
    char clave[50];
    int valor;
    FILE *archivo;


    archivo = fopen(ruta_archivo, "r");
    if (archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", ruta_archivo);
        return ERR_ARCH;
    }



    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {

        if (sscanf(linea, " %49[^:] : %d", clave, &valor) == 2)
        {


            if (strstr(clave, "cantidad_posiciones"))
            {
                ptConfig->cantPosiciones = valor;
            }
            else if (strstr(clave, "vidas_inicio"))
            {
                ptConfig->cantVidasInicio = valor;
            }
            else if (strstr(clave, "maximo_bandidos"))
            {
                ptConfig->cantBandido = valor;
            }
            else if (strstr(clave, "maximo_premios"))
            {
                ptConfig->cantPremios = valor;
            }
            else if (strstr(clave, "maximo_vidas_extra"))
            {
                ptConfig->cantVidasExtra = valor;
            }
            else if (strstr(clave, "maximo_oasis"))
            {
                ptConfig->cantOasis = valor;
            }
            else if (strstr(clave, "maximo_tormentas"))
            {
                ptConfig->cantTormenta = valor;
            }
        }
    }

    fclose(archivo);
    return TODO_OK;
}


int generarArchivoCaravana(VectorTDA* caravana)
{
    int contBandidos, cantElementos, imprimirEspacio, i;
    CasilleroGen aux;


    FILE* archivo = fopen(CARAVANA, "wt");

    if (archivo == NULL)
    {
        return ERR_ARCH;
    }



    for (i = 0; i < caravana->cap ; i++)

    {
        obtenerDeVector(caravana, &aux, i);

        fprintf(archivo, "%02d:", i + 1);

        cantElementos = aux.inicio + aux.salida + aux.jugador +
                        aux.premio + aux.vidaExtra + aux.oasis +
                        aux.tormenta + aux.bandido;


        if (cantElementos == 0)
        {

            fprintf(archivo, ".\n");

        }
        else
        {

            if (cantElementos > 1)
            {
                fprintf(archivo, "[");
            }

            imprimirEspacio = 0;

            if (aux.inicio)
            {
                fprintf(archivo, "I");
                imprimirEspacio = 1;
            }

            if (aux.salida)
            {
                if(imprimirEspacio) fprintf(archivo, " ");
                fprintf(archivo, "S");
                imprimirEspacio = 1;
            }

            if (aux.premio)
            {
                if(imprimirEspacio) fprintf(archivo, " ");
                fprintf(archivo, "P");
                imprimirEspacio = 1;
            }

            if (aux.vidaExtra)
            {
                if(imprimirEspacio) fprintf(archivo, " ");
                fprintf(archivo, "V");
                imprimirEspacio = 1;
            }

            if (aux.oasis)
            {
                if(imprimirEspacio) fprintf(archivo, " ");
                fprintf(archivo, "O");
                imprimirEspacio = 1;
            }

            if (aux.tormenta)
            {
                if(imprimirEspacio) fprintf(archivo, " ");
                fprintf(archivo, "T");
                imprimirEspacio = 1;
            }

            contBandidos = aux.bandido;
            while (contBandidos > 0)
            {
                if(imprimirEspacio) fprintf(archivo, " ");
                fprintf(archivo, "B");
                imprimirEspacio = 1;
                contBandidos--;
            }

            if (aux.jugador)
            {
                if(imprimirEspacio) fprintf(archivo, " ");
                fprintf(archivo, "J");
            }

            if (cantElementos > 1)
            {
                fprintf(archivo, "]");
            }

            fprintf(archivo, "\n");
        }
    }

    fclose(archivo);

    return TODO_OK;
}


// Cambiamos la firma para que reciba directamente la estructura del juego
int cargarCaravanaEnLista(const char *ruta_archivo, estadoJuego *estado)
{
    FILE *archivo = fopen(ruta_archivo, "r");
    if (archivo == NULL)
    {
        printf("Error: No se pudo abrir el archivo %s\n", ruta_archivo);
        return ERR_ARCH;
    }

    char linea[100];
    char elementosCrudos[50];
    casilleroJuego nuevoCasillero;

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        char *salto_n = strchr(linea, '\n');
        if (salto_n != NULL)
        {
            *salto_n = '\0';
        }
        char *salto_r = strchr(linea, '\r');
        if (salto_r != NULL)
        {
            *salto_r = '\0';
        }

        memset(&nuevoCasillero, 0, sizeof(casilleroJuego));

        if (sscanf(linea, "%d:%[^\n]", &nuevoCasillero.numeroPosicion, elementosCrudos) == 2)
        {
            for (int i = 0; elementosCrudos[i] != '\0'; i++)
            {
                switch (elementosCrudos[i])
                {
                case 'I':
                    nuevoCasillero.inicio = 1;
                    break;
                case 'S':
                    nuevoCasillero.salida = 1;
                    break;
                case 'J':
                    nuevoCasillero.jugador = 1;
                    break;
                case 'P':
                    nuevoCasillero.premio = 1;
                    break;
                case 'V':
                    nuevoCasillero.vidaExtra = 1;
                    break;
                case 'O':
                    nuevoCasillero.oasis = 1;
                    break;
                case 'T':
                    nuevoCasillero.tormenta = 1;
                    break;
                case 'B':
                    nuevoCasillero.bandido++; // ACUMULAMOS en vez de = 1
                    break;
                }
            }

            ponerAlFinal(&(estado->ruta), &nuevoCasillero, sizeof(casilleroJuego));

            Nodo* nodoRecienInsertado = estado->ruta->ant;

            if (nuevoCasillero.inicio)
            {
                estado->nodoInicio = nodoRecienInsertado;
            }
            if (nuevoCasillero.jugador)
            {
                estado->nodoJugador = nodoRecienInsertado;
            }

            for (int b = 0; b < nuevoCasillero.bandido; b++)
            {
                estado->nodosBandidos[estado->cantBandidosVivos] = nodoRecienInsertado;
                estado->cantBandidosVivos++;
            }
        }
    }


    fclose(archivo);
    return TODO_OK;
}


void moverEntidad(Lista* ruta, Nodo** punteroEntidad, char tipoEntidad, char direccion, int cantidad_pasos)
{
    casilleroJuego aux;

    obtenerDatoLista(*punteroEntidad, &aux, sizeof(casilleroJuego));

    if (tipoEntidad == 'J')
    {
        aux.jugador = 0;
    }
    else if (tipoEntidad == 'B')
    {
        aux.bandido--;
    }

    actualizarDatoLista(*punteroEntidad, &aux, sizeof(casilleroJuego));

    avanzarEnLista(ruta, punteroEntidad, direccion, cantidad_pasos);

    obtenerDatoLista(*punteroEntidad, &aux, sizeof(casilleroJuego));

    if (tipoEntidad == 'J')
    {
        aux.jugador = 1;
    }
    else if (tipoEntidad == 'B')
    {
        aux.bandido++;
    }

    actualizarDatoLista(*punteroEntidad, &aux, sizeof(casilleroJuego));
}

int generarMovimientos(estadoJuego* estado, cola* colaTurno)
{

    movimiento movActual;


    if (estado->turnoPerdido == 0)
    {
        movActual.entidad = 'J';

        movActual.cantidad = (rand() % 6) + 1;

        movActual.direccion = 'F';

        ponerEnCola(colaTurno, &movActual, sizeof(movimiento));
    }
    else
    {

        estado->turnoPerdido = 0;
    }

    for (int i = 0; i < estado->cantBandidosVivos; i++)
    {
        movActual.entidad = 'B';

        movActual.cantidad = (rand() % 3) + 1;

        Nodo* bandidoActual = *(estado->nodosBandidos+i);
        movActual.direccion = calcularCaminoMasCorto(bandidoActual, estado->nodoJugador);

        ponerEnCola(colaTurno, &movActual, sizeof(movimiento));
    }

    return TODO_OK;
}
char calcularCaminoMasCorto(Nodo* nodoBandido, Nodo* nodoJugador)
{

    if (nodoBandido == nodoJugador) return 'F';

    Nodo* exploradorF = nodoBandido;
    Nodo* exploradorB = nodoBandido;

    while (exploradorF != nodoJugador && exploradorB != nodoJugador)
    {
        exploradorF = exploradorF->sig;
        if (exploradorF == nodoJugador) return 'F';

        exploradorB = exploradorB->ant;
        if (exploradorB == nodoJugador) return 'B';
    }

    return 'F';
}
int ejecutarMovimientosLista(estadoJuego* estado, cola* colaTurno)
{
    movimiento movActual;
    int indiceBandido = 0;
    casilleroJuego casAux;

    while(sacarDeCola(colaTurno, &movActual, sizeof(movimiento)) == TODO_OK)
    {

        if(movActual.entidad == 'J')
        {

            obtenerDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));
            casAux.jugador = 0;
            actualizarDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));

            char dirActual = movActual.direccion;

            for(int p = 0; p < movActual.cantidad; p++)
            {
                avanzarEnLista(&(estado->ruta), &(estado->nodoJugador), dirActual, 1);

                obtenerDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));
                if(casAux.salida == 1 && p < (movActual.cantidad - 1))
                {
                    dirActual = (dirActual == 'F') ? 'B' : 'F';
                }
            }

            obtenerDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));
            casAux.jugador = 1;
            actualizarDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));
        }
        else if(movActual.entidad == 'B')
        {

            if (indiceBandido < estado->cantBandidosVivos)
            {
                Nodo* nodoB = *(estado->nodosBandidos + indiceBandido);

                obtenerDatoLista(nodoB, &casAux, sizeof(casilleroJuego));
                casAux.bandido--;
                actualizarDatoLista(nodoB, &casAux, sizeof(casilleroJuego));

                avanzarEnLista(&(estado->ruta), &nodoB, movActual.direccion, movActual.cantidad);

                obtenerDatoLista(nodoB, &casAux, sizeof(casilleroJuego));
                casAux.bandido++;
                actualizarDatoLista(nodoB, &casAux, sizeof(casilleroJuego));

                *(estado->nodosBandidos + indiceBandido) = nodoB;
            }

            indiceBandido++;
        }
    }

    return TODO_OK;
}
void eliminarBandidoDelArreglo(estadoJuego* estado, int indiceBandido)
{
    for(int i = indiceBandido; i < estado->cantBandidosVivos - 1; i++)
    {
        estado->nodosBandidos[i] = estado->nodosBandidos[i + 1];
    }
    estado->cantBandidosVivos--;
}
int determinarNuevoEstado(estadoJuego* estado)
{
    casilleroJuego casAux;
    obtenerDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));

    if(casAux.salida == 1)
    {

        estado->victoria = 1;

        return TODO_OK;
    }

    if(casAux.bandido > 0)
    {
        if(estado->protegidoPorOasis == 0)
        {
            estado->vidas--;
            casAux.bandido--;
            casAux.jugador = 0;
            actualizarDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));

            int i = 0;
            int bandidoEncontrado = 0;

            while(i < estado->cantBandidosVivos && bandidoEncontrado == 0)
            {
                if(*(estado->nodosBandidos + i) == estado->nodoJugador)
                {
                    eliminarBandidoDelArreglo(estado, i);
                    bandidoEncontrado = 1;
                }
                i++;
            }

            if(estado->vidas <= 0)
            {
                estado->victoria = 2;
            }
            else
            {

                estado->nodoJugador = estado->nodoInicio;
                obtenerDatoLista(estado->nodoInicio, &casAux, sizeof(casilleroJuego));
                casAux.jugador = 1;
                actualizarDatoLista(estado->nodoInicio, &casAux, sizeof(casilleroJuego));
            }

            return TODO_OK;
        }
    }

    if(casAux.premio > 0)
    {
        estado->puntos++;
        casAux.premio = 0;
    }

    if(casAux.vidaExtra > 0)
    {
        estado->vidas++;
        casAux.vidaExtra = 0;
    }

    if(casAux.tormenta == 1)
    {
        if(estado->protegidoPorOasis == 0)
        {
            estado->turnoPerdido = 1;
        }
    }

    if(casAux.oasis == 1)
    {
        estado->protegidoPorOasis = 1;
    }
    else
    {
        estado->protegidoPorOasis = 0;
    }

    actualizarDatoLista(estado->nodoJugador, &casAux, sizeof(casilleroJuego));

    return TODO_OK;
}
void mostrarCaravana(Lista* p)
{
    if(!*p)
        return;

    Nodo* act = *p;
    char bufferElementos[30];

    printf("\n--- ESTADO DE LA CARAVANA ---\n");


    do
    {
        casilleroJuego* casillero = (casilleroJuego*)act->info;

        bufferElementos[0] = '\0';

        if (casillero->inicio) strcat(bufferElementos, "I ");
        if (casillero->salida) strcat(bufferElementos, "S ");
        if (casillero->jugador) strcat(bufferElementos, "J ");
        if (casillero->oasis) strcat(bufferElementos, "O ");
        if (casillero->tormenta) strcat(bufferElementos, "T ");

        for (int i = 0; i < casillero->premio; i++) strcat(bufferElementos, "P ");
        for (int i = 0; i < casillero->vidaExtra; i++) strcat(bufferElementos, "V ");
        for (int i = 0; i < casillero->bandido; i++) strcat(bufferElementos, "B ");

        printf("%02d:", casillero->numeroPosicion);

        int longitud = strlen(bufferElementos);

        if (longitud == 0)
        {
            printf(".\n");
        }
        else
        {
            bufferElementos[longitud - 1] = '\0';
            longitud--;

            if (longitud == 1)
            {
                printf("%s\n", bufferElementos);
            }
            else
            {
                printf("[%s]\n", bufferElementos);
            }
        }


        act = act->sig;

    }
    while(act != *p);

    printf("-----------------------------\n");
}
int inicializarEstadoJuego(estadoJuego* estado, config *datosConfig)
{

    crearLista(&(estado->ruta));


    estado->nodosBandidos = malloc(datosConfig->cantBandido * sizeof(Nodo*));
    if (estado->nodosBandidos == NULL)
    {
        return SIN_MEM;
    }


    estado->nodoJugador = NULL;
    estado->nodoInicio = NULL;
    estado->cantBandidosVivos = 0;

    estado->vidas = datosConfig->cantVidasInicio;
    estado->puntos = 0;
    estado->protegidoPorOasis = 0;
    estado->turnoPerdido = 0;
    estado->victoria = 0;

    return TODO_OK;
}
