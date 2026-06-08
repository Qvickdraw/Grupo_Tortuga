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
            if(aux.tormenta!=1 && aux.tormenta!=1)
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


 printf("\nsoy re hetero");

    while (fgets(linea, sizeof(linea), archivo) != NULL)
    {
        char *salto_n = strchr(linea, '\n');
        if (salto_n != NULL)
        {
            *salto_n = '\0'; // Desreferenciamos el puntero y clavamos el fin de cadena
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

            // 1. Insertamos el casillero en la lista circular del estado
            ponerAlFinal(&(estado->ruta), &nuevoCasillero, sizeof(casilleroJuego));

            // 2. Capturamos la dirección en memoria del nodo recién creado.
            // Al ser circular, el nodo que acabás de insertar al final siempre es el 'ant' del primero.
            Nodo* nodoRecienInsertado = estado->ruta->ant;

            // 3. Asignamos los punteros de control global
            if (nuevoCasillero.inicio)
            {
                estado->nodoInicio = nodoRecienInsertado;
            }
            if (nuevoCasillero.jugador)
            {
                estado->nodoJugador = nodoRecienInsertado;
            }

            // 4. Si hay bandidos, guardamos el puntero en el arreglo dinámico
            for (int b = 0; b < nuevoCasillero.bandido; b++)
            {
                estado->nodosBandidos[estado->cantBandidosVivos] = nodoRecienInsertado;
                estado->cantBandidosVivos++;
            }
        }
    }

     printf("\nsoy re gay");

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



void mostrarCaravana(Lista* p)
{
    if(!*p)
        return;

    Nodo* act = *p;
    char bufferElementos[30];

    printf("\n--- ESTADO DE LA CARAVANA ---\n");

    // Reemplazamos el while por un do-while
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

        // Avanzamos al siguiente nodo
        act = act->sig;

    } while(act != *p); // Condición de corte: Si volvimos a la cabeza, dimos 1 vuelta exacta.

    printf("-----------------------------\n");
}
int inicializarEstadoJuego(estadoJuego* estado, config *datosConfig)
{

    crearLista(&(estado->ruta));


    estado->nodosBandidos = malloc(datosConfig->cantBandido * sizeof(Nodo*));
    if (estado->nodosBandidos == NULL) {
        return SIN_MEM;
    }


    estado->nodoJugador = NULL;
    estado->nodoInicio = NULL;
    estado->cantBandidosVivos = 0;

    estado->vidas = datosConfig->cantVidasInicio;
    estado->puntos = 0;
    estado->protegidoPorOasis = 0;
    estado->turnoPerdido = 0;

    return TODO_OK;
}
