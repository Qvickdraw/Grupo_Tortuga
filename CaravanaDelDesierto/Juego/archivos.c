#include "archivos.h"

int leerConfig(tConfig *config)
{
    FILE *pf = fopen(ARCHIVO_CONFIG, "r");
    if (!pf)
    {
        printf("Error: no se pudo abrir %s\n", ARCHIVO_CONFIG);
        return 0;
    }

    /* EL ORDEN EN EL CONFIG.TXT IMPORTA! */
    fscanf(pf, "cantidad_posiciones:%d\n", &config->cantidadPosiciones);
    fscanf(pf, "vidas_inicio:%d\n", &config->vidasInicio);
    fscanf(pf, "maximo_bandidos:%d\n", &config->maximoBandidos);
    fscanf(pf, "maximo_premios:%d\n", &config->maximoPremios);
    fscanf(pf, "maximo_vidas_extra:%d\n", &config->maximoVidasExtra);
    fscanf(pf, "maximo_oasis:%d\n", &config->maximoOasis);
    fscanf(pf, "maximo_tormentas:%d\n", &config->maximoTormentas);

    fclose(pf);
    return 1;
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

    /* Contamos cuantos elementos hay en la celda */
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
        /* Un solo elemento fijo sin jugador: se escribe directo */
        if (celda->estaInicio) { sprintf(buf, "I"); return; }
        if (celda->estaSalida) { sprintf(buf, "S"); return; }
        if (celda->tienePremio) { sprintf(buf, "P"); return; }
        if (celda->tieneVida) { sprintf(buf, "V"); return; }
        if (celda->tieneOasis) { sprintf(buf, "O"); return; }
        if (celda->tieneTormenta) { sprintf(buf, "T"); return; }
        if (celda->cantBandidos == 1) { sprintf(buf, "B"); return; }
    }

    /* Mas de un elemento o jugador presente: usamos corchetes */
    if (celda->estaInicio) strcat(tmp, "I ");
    if (celda->estaSalida) strcat(tmp, "S ");
    if (celda->tienePremio) strcat(tmp, "P ");
    if (celda->tieneVida) strcat(tmp, "V ");
    if (celda->tieneOasis) strcat(tmp, "O ");
    if (celda->tieneTormenta) strcat(tmp, "T ");

    for (i = 0; i < celda->cantBandidos; i++)
        strcat(tmp, "B ");

    if (celda->tieneJugador) strcat(tmp, "J ");

    /* Reemplazamos el ultimo espacio por el cierre */
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
        return 0;
    }

    for (i = 0; i < cantPosiciones; i++)
    {
        celdaAString(&celdas[i], buf);
        fprintf(pf, "%02d:%s\n", celdas[i].numero, buf);
    }

    fclose(pf);
    return 1;
}

int leerCaravana(tLista *lista, int cantPosiciones)
{
    char *c;
    char linea[100];
    int  leidos = 0;
    FILE *f = fopen(ARCHIVO_CARAVANA, "r");
    if (!f)
    {
        printf("Error: no se pudo abrir %s\n", ARCHIVO_CARAVANA);
        return 0;
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

        /* Numero de pos antes de ':' */
        celda.numero = atoi(linea);

        /* Contenido despues de : */
        char *contenido = strchr(linea, ':');
        if (!contenido)
            continue;
        contenido++; /* saltamos el ':' */

        /* Recorremos el contenido caracter por caracter */
        for (c = contenido; *c != '\0' && *c != '\n'; c++)
        {
            switch (*c)
            {
                case CHAR_INICIO: celda.estaInicio = 1; break;
                case CHAR_SALIDA: celda.estaSalida = 1; break;
                case CHAR_PREMIO: celda.tienePremio = 1; break;
                case CHAR_VIDA: celda.tieneVida = 1; break;
                case CHAR_OASIS: celda.tieneOasis = 1; break;
                case CHAR_TORMENTA: celda.tieneTormenta= 1; break;
                case CHAR_BANDIDO: celda.cantBandidos++; break;
                case CHAR_JUGADOR: celda.tieneJugador = 1; break;
                default: break;
            }
        }

        if (!listaPoner(lista, &celda, sizeof(tCelda)))
        {
            fclose(f);
            return 0;
        }

        leidos++;
    }

    fclose(f);
    return 1;
}
