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

int guardarCaravana(tLista *lista, int cantPosiciones)
{
    int i = 0;
    FILE *pf = fopen(ARCHIVO_CARAVANA, "w");
    if (!pf)
    {
        printf("Error: no se pudo crear %s\n", ARCHIVO_CARAVANA);
        return 0;
    }

    if (!*lista)
    {
        fclose(pf);
        return 0;
    }

    tNodoLista *inicio = (*lista)->sig;
    tNodoLista *actual = inicio;

    do
    {
        tCelda *celda = (tCelda *)actual->info;

        fprintf(pf, "%02d:", celda->numero);

        if (celda->tieneJugador)
        {
            if (celda->tipo == CELDA_VACIA || celda->tipo == CELDA_INICIO)
                fprintf(pf, "[J]");
            else
                fprintf(pf, "[%c J]", (char)celda->tipo);
        }
        else if (celda->tieneBandido && celda->tipo != CELDA_VACIA)
        {
            fprintf(pf, "[%c B]", (char)celda->tipo);
        }
        else if (celda->tieneBandido)
        {
            fprintf(pf, "B");
        }
        else
        {
            fprintf(pf, "%c", (char)celda->tipo);
        }

        fprintf(pf, "\n");

        actual = actual->sig;
        i++;

    } while (actual != inicio && i < cantPosiciones);

    fclose(pf);
    return 1;
}
