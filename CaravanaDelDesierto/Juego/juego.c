#include "juego.h"
#include "../registros/registros.h"
void esperarEnter()
{
    printf("\nPresione Enter para continuar...\n\n");
    while(getchar()!= '\n');
}

int juegoInicializar(tEstadoJuego *estado, const tConfig *config)
{

    int codError;
    codError=leerCaravana(&estado->ruta, config->cantidadPosiciones);

    if(codError!=TODO_OK)
    {
        printf("\nNo se puedo cargar la caravana.\n");
        return codError;
    }

    tNodoLista *actual = estado->ruta->sig;
    tCelda *celda;
    int i, j;

    estado->nodoInicio = estado->ruta->sig;
    estado->nodoSalida = estado->ruta;

    estado->nodoJugador = estado->nodoInicio;
    estado->vidas = config->vidasInicio;
    estado->puntos = 0;
    estado->protegido = 0;
    estado->pierdeTurno = 0;

    estado->nodosBandidos = malloc(config->maximoBandidos * sizeof(tNodoLista*));

    if(!estado->nodosBandidos)
        return SIN_MEM;

    estado->cantBandidosVivos = 0;

    for (i = 0; i < config->cantidadPosiciones; i++)
    {
        celda = (tCelda *)actual->info;
        for (j = 0; j < celda->cantBandidos; j++)
        {
            *(estado->nodosBandidos + estado->cantBandidosVivos) = actual;
            estado->cantBandidosVivos++;
        }
        actual = actual->sig;
    }

    return TODO_OK;
}

void juegoDestruir(tEstadoJuego *estado)
{
    free(estado->nodosBandidos);
    estado->nodosBandidos = NULL;
    listaVaciar(&estado->ruta);
}

int juegoLanzarDado()
{
    return 1 + rand() % 6;
}

int juegoEncolarMovJugador(tCola *cola, tDireccion dir, int pasos)
{
    int codError;
    tMovimiento mov;
    mov.direccion = dir;
    mov.pasos = pasos;
    mov.esBandido = 0;
    mov.idBandido = -1;

    codError=colaPoner(cola, &mov, sizeof(tMovimiento));
    if(codError!=TODO_OK)
        return codError;

    return TODO_OK;
}

int juegoEncolarMovBandidos(tCola *cola, tEstadoJuego *estado)
{
    tMovimiento mov;
    int codError;
    int i;

    for (i = 0; i < estado->cantBandidosVivos; i++)
    {

        mov.esBandido = 1;
        mov.idBandido = i;
        mov.pasos = juegoLanzarDado();

        mov.direccion=calcularCaminoMasCorto(estado->nodosBandidos[i], estado->nodoJugador);

        codError=colaPoner(cola, &mov, sizeof(tMovimiento));
        if(codError!=TODO_OK)
            return codError;
    }
    return TODO_OK;
}

int juegoMoverJugador(tEstadoJuego *estado, tDireccion dir, int pasos)
{
    tCelda *celdaDestino;
    int pasosRestantes = pasos, llego = 0;

    tCelda *celdaActual = (tCelda *)estado->nodoJugador->info;
    celdaActual->tieneJugador = 0;

    while (pasosRestantes > 0)
    {
        if (estado->nodoJugador == estado->nodoInicio && dir == MOV_ATRAS )
        {
            if (pasosRestantes > 0)
                dir = MOV_ADELANTE;
        }
        if (dir == MOV_ADELANTE)
            estado->nodoJugador = estado->nodoJugador->sig;
        else
            estado->nodoJugador = estado->nodoJugador->ant;

        pasosRestantes--;

        if (estado->nodoJugador == estado->nodoSalida)
        {
            if (pasosRestantes > 0)
                dir = MOV_ATRAS;
            else
                llego = 1;
        }

    }

    celdaDestino = (tCelda*)estado->nodoJugador->info;
    celdaDestino->tieneJugador = 1;

    return llego;
}


char calcularCaminoMasCorto(tNodoLista* nodoBandido, tNodoLista* nodoJugador)
{
    if (nodoBandido == nodoJugador) return MOV_ADELANTE;

    tNodoLista* exploradorF = nodoBandido;
    tNodoLista* exploradorB = nodoBandido;


    while (exploradorF != nodoJugador && exploradorB != nodoJugador)
    {
        exploradorF = exploradorF->sig;
        if (exploradorF == nodoJugador) return MOV_ADELANTE;

        exploradorB = exploradorB->ant;
        if (exploradorB == nodoJugador) return MOV_ATRAS;
    }

    return MOV_ADELANTE;
}


void juegoMoverBandido(tEstadoJuego *estado, tDireccion direccionOptima, int indiceBandido, int pasos)
{
    tCelda *celdaNueva;
    tCelda *celdaVieja = (tCelda*)(*(estado->nodosBandidos + indiceBandido))->info;

    if (celdaVieja->cantBandidos > 0)
        celdaVieja->cantBandidos--;

    while (pasos > 0)
    {
        if (direccionOptima == MOV_ADELANTE)
            *(estado->nodosBandidos + indiceBandido) = (*(estado->nodosBandidos + indiceBandido))->sig;
        else
            *(estado->nodosBandidos + indiceBandido) = (*(estado->nodosBandidos + indiceBandido))->ant;

        pasos--;
    }

    celdaNueva = (tCelda *)(*(estado->nodosBandidos + indiceBandido))->info;
    celdaNueva->cantBandidos++;
}

void juegoAplicarEfecto(tEstadoJuego *estado)
{
    tCelda *celda = (tCelda*)estado->nodoJugador->info;

    if (!celda->tieneTormenta && !celda->tienePremio && !celda->tieneVida && !celda->tieneOasis)
    {
        printf("\nCasillero vacio. Sin efectos este turno.\n");
        return;
    }


    if (celda->tieneTormenta)
    {
        if (estado->protegido)
            printf("\nEl oasis te protegio de la tormenta!\n");
        else
        {
            printf("\n¡¡¡TURNO PERDIDO POR TORMENTA!!!\n");
            printf("\n--- Los bandidos aprovecharon la tormenta y se movieron ---\n");
            estado->pierdeTurno = 1;
        }
    }

    if (celda->tienePremio)
    {
        estado->puntos++;
        celda->tienePremio = 0;
        printf("\nEncontraste un premio! Puntos: %d\n", estado->puntos);
    }

    if (celda->tieneVida)
    {
        estado->vidas++;
        celda->tieneVida = 0;
        printf("\nEncontraste una vida extra! Vidas: %d\n", estado->vidas);
    }

    if (celda->tieneOasis)
    {
        estado->protegido = 1;
        printf("\nDescansaste en un oasis! Estaras protegido el proximo turno.\n");
    }

    if (estado->protegido && !celda->tieneOasis)
        estado->protegido = 0;

    return;
}

int juegoVerificarColision(tEstadoJuego *estado, int cantPosiciones)
{
    if (estado->protegido)
        return 0;

    int i, j;
    int bandidoBorrado = 0;
    tCelda *celdaInicio;
    tCelda *celdaJugador = (tCelda*)estado->nodoJugador->info;

    if (celdaJugador->cantBandidos == 0)
        return 0;

    printf("\n¡Un bandido te intercepto! Perdes una vida.\n");
    celdaJugador->tieneJugador = 0;

    for (i = 0; i < estado->cantBandidosVivos && !bandidoBorrado; i++)
    {
        if (estado->nodosBandidos[i] == estado->nodoJugador)
        {
            celdaJugador->cantBandidos--;

            for (j = i; j < estado->cantBandidosVivos - 1; j++)
                *(estado->nodosBandidos+j) = *(estado->nodosBandidos+j+1);

            estado->cantBandidosVivos--;

            bandidoBorrado = 1;
        }
    }

    estado->vidas--;
    estado->nodoJugador = estado->nodoInicio;
    estado->pierdeTurno = 0;

    celdaInicio = (tCelda*)estado->nodoInicio->info;
    celdaInicio->tieneJugador = 1;

    printf("Vidas restantes: %d\n", estado->vidas);
    return 1;
}

void juegoRegistrarMovimiento(char historial[][10], int *cantMovimientos, tDireccion dir, int pasos)
{
    if (*cantMovimientos >= MAX_HISTORIAL)
        return;

    if (dir == MOV_ADELANTE)
        sprintf(*(historial + *cantMovimientos), "F%d", pasos);
    else
        sprintf(*(historial + *cantMovimientos), "B%d", pasos);

    (*cantMovimientos)++;
}

void juegoMostrarHistorial(char historial[][10], int cantMovimientos)
{
    int i;
    printf("\n=== HISTORIAL DE MOVIMIENTOS ===\n");
    for (i = 0; i < cantMovimientos; i++)
        printf("%s ", historial[i]);
    printf("\n================================\n");
}

int juegoDesencolarYProcesar(tCola *cola, tEstadoJuego *estado, int cantPosiciones)
{

    tMovimiento mov;
    int llego = 0;
    int jugadorSeMovio = 0;


    system("cls");
    printf("=== MOVIMIENTOS DEL TURNO ===\n");

    while (!colaEstaVacia(cola))
    {
        colaSacar(cola, &mov, sizeof(tMovimiento));

        if (!mov.esBandido)
        {
            printf("Jugador:%s%d\n", mov.direccion == MOV_ADELANTE ? "F" : "B",mov.pasos);
            llego = juegoMoverJugador(estado, mov.direccion, mov.pasos);
            jugadorSeMovio = 1;
        }
        else
        {
            printf("Bandido:%s%d\n", mov.direccion == MOV_ADELANTE ? "F" : "B",mov.pasos);
            juegoMoverBandido(estado, mov.direccion, mov.idBandido, mov.pasos);
        }
    }

    printf("=== RESULTADO DEL TURNO ===\n");
    tableroMostrar(&estado->ruta, cantPosiciones);

    if (juegoVerificarColision(estado, cantPosiciones))
    {
        if (estado->vidas <= 0)
            return 0;
        esperarEnter();
        return 1;
    }
    else if(!llego && jugadorSeMovio)
    {
        juegoAplicarEfecto(estado);
    }

    if(!llego)
        esperarEnter();

    return llego ? 0 : 1;
}

int juegoJugar(tConfig *config, const char *nombreJugador, int *puntos)
{
    tEstadoJuego estado;
    tCola cola;
    char historial[MAX_HISTORIAL][10], opcion;
    int cantMovimientos = 0, partidaActiva = 1, gano = 0, dado;
    tDireccion dir;
    tPartida partida;
    int codError;

    codError=tableroGenerar(config);
    if (codError!=TODO_OK)
    {
        printf("Error al generar el tablero.\n");
        return codError;
    }

    codError=juegoInicializar(&estado, config);
    if (codError!=TODO_OK)
    {
        printf("Error al inicializar el juego.\n");
        return codError;
    }


    colaCrear(&cola);

    printf("\n=== CARAVANA DEL DESIERTO ===\n");
    printf("Jugador: %s | Vidas: %d | Puntos: %d\n", nombreJugador, estado.vidas, estado.puntos);
    esperarEnter();



    while (partidaActiva)
    {

        system("cls");
        tableroMostrar(&estado.ruta, config->cantidadPosiciones);
        printf("Vidas: %d | Puntos: %d\n", estado.vidas, estado.puntos);

        if (estado.pierdeTurno)
        {


            codError=juegoEncolarMovBandidos(&cola, &estado);
            if (codError!=TODO_OK)
            {
                printf("Error al encolar movimientos de bandido.\n");
                colaVaciar(&cola);
                juegoDestruir(&estado);
                return codError;
            }
            partidaActiva = juegoDesencolarYProcesar(&cola, &estado, config->cantidadPosiciones);
            estado.pierdeTurno = 0;

        }
        else
        {

            printf("\nPresione Enter para TIRAR EL DADO...");
            while(getchar() != '\n');

            dado = juegoLanzarDado();
            printf("\n¡Sacaste un %d!\n", dado);
            printf("\nMover hacia adelante(A) o atras(S)? ");
            do
            {
                scanf(" %c", &opcion);
                while(getchar() != '\n');
                if(toupper(opcion)!= 'A' &&  toupper(opcion)!= 'S')
                {
                    system("cls");
                    tableroMostrar(&estado.ruta, config->cantidadPosiciones);
                    printf("\nDireccion elegida invalida\n");
                    printf("\nMover hacia adelante(A) o atras(S)? ");
                }
            }
            while(toupper(opcion)!= 'A' &&  toupper(opcion)!= 'S');


            dir = (toupper(opcion) == 'A')? MOV_ADELANTE : MOV_ATRAS;


            codError=juegoEncolarMovJugador(&cola, dir, dado);
            if (codError!=TODO_OK)
            {
                printf("Error al encolar movimientos de jugador.\n");
                colaVaciar(&cola);
                juegoDestruir(&estado);
                return codError;
            }


            codError=juegoEncolarMovBandidos(&cola, &estado);
            if (codError!=TODO_OK)
            {
                printf("Error al encolar movimientos de bandido.\n");
                colaVaciar(&cola);
                juegoDestruir(&estado);
                return codError;
            }
            juegoRegistrarMovimiento(historial, &cantMovimientos, dir, dado);

            partidaActiva = juegoDesencolarYProcesar(&cola, &estado, config->cantidadPosiciones);
        }

        if (estado.nodoJugador == estado.nodoSalida && partidaActiva == 0)
        {
            gano = 1;
        }
        else if (estado.vidas <= 0)
        {
            gano = 0;
        }
    }


    if (gano)
    {
        printf("\nLlegaste a la Ciudad Refugio!");
        printf("\nPuntos obtenidos: %d", estado.puntos);
        printf("\nPuntos por victoria: 2");
        estado.puntos+=2;
        printf("\nPuntos totales:%d\n", estado.puntos);
    }
    else
    {
        printf("\nSe agotaron tus vidas. Fin del juego.");
        printf("\nPierdes todos tus puntos!!!\n");
        estado.puntos=0;
    }

    juegoMostrarHistorial(historial, cantMovimientos);

    strncpy(partida.nombreJugador, nombreJugador, MAX_NOMBRE - 1);
    partida.nombreJugador[MAX_NOMBRE - 1] = '\0';
    partida.puntos = estado.puntos;
    *puntos = estado.puntos;
    partida.cantMovimientos = cantMovimientos;
    partida.gano = gano;


    codError=guardarPartidaBD(&partida);
    if (codError!=TODO_OK)
    {
        printf("Error al guardar partida.\n");
        colaVaciar(&cola);
        juegoDestruir(&estado);
        return codError;
    }


    colaVaciar(&cola);
    juegoDestruir(&estado);

    return gano;
}
