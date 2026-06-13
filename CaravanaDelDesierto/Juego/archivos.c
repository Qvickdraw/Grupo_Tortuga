#include "archivos.h"

int leerConfig(tConfig *config)
{
    FILE *pf = fopen(ARCHIVO_CONFIG, "r");
    if (!pf)
    {
        printf("Error: no se pudo abrir %s\n", ARCHIVO_CONFIG);
        return ERR_ARCH;
    }

    fscanf(pf, "cantidad_posiciones:%d\n", &config->cantidadPosiciones);
    fscanf(pf, "vidas_inicio:%d\n", &config->vidasInicio);
    fscanf(pf, "maximo_bandidos:%d\n", &config->maximoBandidos);
    fscanf(pf, "maximo_premios:%d\n", &config->maximoPremios);
    fscanf(pf, "maximo_vidas_extra:%d\n", &config->maximoVidasExtra);
    fscanf(pf, "maximo_oasis:%d\n", &config->maximoOasis);
    fscanf(pf, "maximo_tormentas:%d\n", &config->maximoTormentas);


    if (config->maximoBandidos < config->vidasInicio)
    {
        printf("\nError: Ingreso invalido en la configuracion.\n");
        printf("La cantidad maxima de bandidos (%d) no puede ser menor a las vidas de inicio (%d).\n",config->maximoBandidos, config->vidasInicio);

        fclose(pf);
        return ERR_CONFIG;
    }
    if (config->cantidadPosiciones < 3)
    {
        printf("Error: se necesitan al menos 3 posiciones.\n");
        return ERR_CONFIG;
    }
    fclose(pf);
    return TODO_OK;
}

void mostrarConfig(const tConfig *config)
{
    printf("--- Configuracion cargada ---\n");
    printf("Posiciones    : %d\n", config->cantidadPosiciones);
    printf("Vidas inicio  : %d\n", config->vidasInicio);
    printf("Bandidos      : %d\n", config->maximoBandidos);
    printf("Premios       : %d\n", config->maximoPremios);
    printf("Vidas extra   : %d\n", config->maximoVidasExtra);
    printf("Oasis         : %d\n", config->maximoOasis);
    printf("Tormentas     : %d\n", config->maximoTormentas);
    printf("-----------------------------\n");
}

void celdaAString(const tCelda *celda, char *buf)
{
    char tmp[50] = "[";
    int i;

    int cant = celda->estaInicio + celda->estaSalida
               + celda->tienePremio + celda->tieneVida
               + celda->tieneOasis + celda->tieneTormenta
               + celda->cantBandidos + celda->tieneJugador;

    if (cant == 0)
    {
        sprintf(buf, ".");
        return;
    }

    if (cant == 1 && !celda->tieneJugador)
    {

        if (celda->estaInicio)
        {
            sprintf(buf, "I");
            return;
        }
        if (celda->estaSalida)
        {
            sprintf(buf, "S");
            return;
        }
        if (celda->tienePremio)
        {
            sprintf(buf, "P");
            return;
        }
        if (celda->tieneVida)
        {
            sprintf(buf, "V");
            return;
        }
        if (celda->tieneOasis)
        {
            sprintf(buf, "O");
            return;
        }
        if (celda->tieneTormenta)
        {
            sprintf(buf, "T");
            return;
        }
        if (celda->cantBandidos == 1)
        {
            sprintf(buf, "B");
            return;
        }
    }

    if (celda->estaInicio) strcat(tmp, "I ");
    if (celda->estaSalida) strcat(tmp, "S ");
    if (celda->tienePremio) strcat(tmp, "P ");
    if (celda->tieneVida) strcat(tmp, "V ");
    if (celda->tieneOasis) strcat(tmp, "O ");
    if (celda->tieneTormenta) strcat(tmp, "T ");

    for (i = 0; i < celda->cantBandidos; i++)
        strcat(tmp, "B ");

    if (celda->tieneJugador) strcat(tmp, "J ");

    tmp[strlen(tmp) - 1] = ']';

    strcpy(buf, tmp);
}

int escribirCaravana(tCelda *celdas, int cantPosiciones)
{
    int i;
    char buf[50];
    FILE *pf = fopen(ARCHIVO_CARAVANA, "w");
    if (!pf)
    {
        printf("Error: no se pudo crear %s\n", ARCHIVO_CARAVANA);
        return ERR_ARCH;
    }

    for (i = 0; i < cantPosiciones; i++)
    {
        celdaAString((celdas + i), buf);

        fprintf(pf, "%02d: %s\n", (celdas + i)->numero, buf);
    }

    fclose(pf);
    return TODO_OK;
}

int leerCaravana(tLista *lista, int cantPosiciones)
{
    int codError=TODO_OK;
    char *c;
    char linea[100];
    int  leidos = 0;
    FILE *f = fopen(ARCHIVO_CARAVANA, "r");
    if (!f)
    {
        printf("Error: no se pudo abrir %s\n", ARCHIVO_CARAVANA);
        return ERR_ARCH;
    }

    listaCrear(lista);

    while (fgets(linea, sizeof(linea), f) && leidos < cantPosiciones)
    {
        tCelda celda;
        celda.numero = 0;
        celda.estaInicio = 0;
        celda.estaSalida = 0;
        celda.tienePremio = 0;
        celda.tieneVida = 0;
        celda.tieneOasis = 0;
        celda.tieneTormenta = 0;
        celda.cantBandidos = 0;
        celda.tieneJugador = 0;

        celda.numero = atoi(linea);

        char *contenido = strchr(linea, ':');

        if (contenido)
        {
            contenido++;
        }

        for (c = contenido; *c != '\0' && *c != '\n'; c++)
        {
            switch (*c)
            {
            case CHAR_INICIO:
                celda.estaInicio = 1;
                break;
            case CHAR_SALIDA:
                celda.estaSalida = 1;
                break;
            case CHAR_PREMIO:
                celda.tienePremio = 1;
                break;
            case CHAR_VIDA:
                celda.tieneVida = 1;
                break;
            case CHAR_OASIS:
                celda.tieneOasis = 1;
                break;
            case CHAR_TORMENTA:
                celda.tieneTormenta= 1;
                break;
            case CHAR_BANDIDO:
                celda.cantBandidos++;
                break;
            case CHAR_JUGADOR:
                celda.tieneJugador = 1;
                break;
            default:
                break;
            }
        }
        codError=listaPoner(lista, &celda, sizeof(tCelda));
        if (codError!=TODO_OK)
        {
            fclose(f);
            listaVaciar(lista);
            return codError;
        }

        leidos++;
    }

    fclose(f);
    return codError;
}

int guardarPartida(const tPartida *partida)
{
    FILE *pf = fopen(ARCHIVO_PARTIDAS, "ab");
    if (!pf)
    {
        printf("Error: no se pudo abrir %s\n", ARCHIVO_PARTIDAS);
        return ERR_ARCH;
    }

    fwrite(partida, sizeof(tPartida), 1, pf);
    fclose(pf);
    return TODO_OK;
}
