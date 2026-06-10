#include "juego.h"

void esperarEnter()
{
    printf("\nPresione Enter para continuar...\n\n");
    while(getchar()!= '\n');
}

int juegoInicializar(tEstadoJuego *estado, const tConfig *config)
{
    if(!leerCaravana(&estado->ruta, config->cantidadPosiciones))
    {
        printf("\nNo se puedo cargar la caravana.\n");
        return 0;
    }

    tNodoLista *actual = estado->ruta->sig;
    tCelda *celda;
    int i, j;

    /* Inicializamos estado de juego */
    estado->nodoInicio = estado->ruta->sig; /* Nodo Inicio */
    estado->nodoSalida = tableroBuscarSalida(&estado->ruta, config->cantidadPosiciones); /* Nodo Salida */

    /* Ini Jugador */
    estado->nodoJugador = estado->nodoInicio;
    estado->vidas = config->vidasInicio;
    estado->puntos = 0;
    estado->protegido = 0;
    estado->pierdeTurno = 0;

    /* Ini Bandidos */
    estado->nodosBandidos = malloc(config->maximoBandidos * sizeof(tNodoLista*));
    if(!estado->nodosBandidos)
        return 0;
    estado->cantBandidosVivos = 0;

    for (i = 0; i < config->cantidadPosiciones; i++) /* Buscamos los bandidos */
    {
        celda = (tCelda *)actual->info;
        for (j = 0; j < celda->cantBandidos; j++)
        {
            estado->nodosBandidos[estado->cantBandidosVivos] = actual;
            estado->cantBandidosVivos++;
        }
        actual = actual->sig;
    }

    return 1;
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

void juegoEncolarMovJugador(tCola *cola, tDireccion dir, int pasos)
{
    tMovimiento mov;
    mov.direccion = dir;
    mov.pasos = pasos;
    mov.esBandido = 0;
    mov.idBandido = -1;

    colaPoner(cola, &mov, sizeof(tMovimiento));
}

void juegoEncolarMovBandidos(tCola *cola, tEstadoJuego *estado)
{
    tCelda *celdaBandido, *celdaJugador;
    tMovimiento mov;
    int i;

    for (i = 0; i < estado->cantBandidosVivos; i++)
    {
        celdaBandido = (tCelda *)estado->nodosBandidos[i]->info;
        celdaJugador = (tCelda *)estado->nodoJugador->info;

        mov.esBandido = 1;
        mov.idBandido = i; /* usamos el indice como id */
        mov.pasos = 1;

        if (celdaJugador->numero > celdaBandido->numero)
            mov.direccion = MOV_ADELANTE;
        else
            mov.direccion = MOV_ATRAS;

        colaPoner(cola, &mov, sizeof(tMovimiento));
    }
}

int juegoMoverJugador(tEstadoJuego *estado, tDireccion dir, int pasos)
{
    tCelda *celdaDestino;
    int pasosRestantes = pasos, llego = 0;

    /* Quitamos al jugador de la celda actual */
    tCelda *celdaActual = (tCelda *)estado->nodoJugador->info;
    celdaActual->tieneJugador = 0;

    while (pasosRestantes > 0)
    {
        if (dir == MOV_ADELANTE)
            estado->nodoJugador = estado->nodoJugador->sig;
        else
            estado->nodoJugador = estado->nodoJugador->ant;

        pasosRestantes--;

        if (estado->nodoJugador == estado->nodoSalida)
        {
            if (pasosRestantes > 0)
                dir = MOV_ATRAS; /* rebote */
            else
                llego = 1;
        }
    }

    celdaDestino = (tCelda*)estado->nodoJugador->info;
    celdaDestino->tieneJugador = 1;

    return llego;
}

void juegoMoverBandido(tEstadoJuego *estado, int indice)
{
    tCelda *celdaNueva;
    tCelda *celdaVieja = (tCelda*)estado->nodosBandidos[indice]->info;
    tCelda *celdaJugador = (tCelda*)estado->nodoJugador->info;

    /* Quitamos el bandido de la celda actual */
    if (celdaVieja->cantBandidos > 0)
        celdaVieja->cantBandidos--;

    /* Avanzamos o retrocedemos segun donde este el jugador */
    if (celdaJugador->numero > celdaVieja->numero)
        estado->nodosBandidos[indice] = estado->nodosBandidos[indice]->sig;
    else
        estado->nodosBandidos[indice] = estado->nodosBandidos[indice]->ant;

    /* Ponemos el bandido en la nueva celda */
    celdaNueva = (tCelda *)estado->nodosBandidos[indice]->info;
    celdaNueva->cantBandidos++;
}

void juegoAplicarEfecto(tEstadoJuego *estado)
{
    tCelda *celda = (tCelda*)estado->nodoJugador->info;

    system("cls");

    /* Si la celda no tenia ningun efecto avisamos */
    if (!celda->tieneTormenta && !celda->tienePremio &&
        !celda->tieneVida && !celda->tieneOasis)
        printf("Casillero vacio. Sin efectos este turno.\n");

    /* Tormenta */
    if (celda->tieneTormenta)
    {
        if (estado->protegido)
            printf("El oasis te protegio de la tormenta!\n");
        else
        {
            estado->pierdeTurno = 1;
            printf("Tormenta de arena! Perdes el proximo turno.\n");
        }
    }

    /* Premio */
    if (celda->tienePremio)
    {
        estado->puntos++;
        celda->tienePremio = 0;
        printf("Encontraste un premio! Puntos: %d\n", estado->puntos);
    }

    /* Vida extra */
    if (celda->tieneVida)
    {
        estado->vidas++;
        celda->tieneVida = 0;
        printf("Encontraste una vida extra! Vidas: %d\n", estado->vidas);
    }

    /* Oasis */
    if (celda->tieneOasis)
    {
        estado->protegido = 1;
        printf("Descansaste en un oasis! Estaras protegido el proximo turno.\n");
    }

    /* Consumimos proteccion si no estamos en oasis */
    if (estado->protegido && !celda->tieneOasis)
        estado->protegido = 0;
}

int juegoVerificarColision(tEstadoJuego *estado)
{
    if (estado->protegido)
        return 0;

    int i, j;
    tCelda *celdaInicio;
    tCelda *celdaJugador = (tCelda*)estado->nodoJugador->info;

    if (celdaJugador->cantBandidos == 0)
        return 0;

    printf("Un bandido te intercepto! Perdes una vida.\n");

    /* Quitamos al jugador de la celda */
    celdaJugador->tieneJugador = 0;

    /* Eliminamos 1 bandido de la celda y del arreglo */
    for (i = 0; i < estado->cantBandidosVivos; i++)
    {
        if (estado->nodosBandidos[i] == estado->nodoJugador)
        {
            celdaJugador->cantBandidos--;

            /* Corremos el arreglo */
            for (j = i; j < estado->cantBandidosVivos - 1; j++)
                estado->nodosBandidos[j] = estado->nodosBandidos[j + 1];

            estado->cantBandidosVivos--;
            break;
        }
    }

    /* Jugador pierde vida y vuelve al inicio */
    estado->vidas--;
    estado->nodoJugador = estado->nodoInicio;

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
        sprintf(historial[*cantMovimientos], "F%d", pasos);
    else
        sprintf(historial[*cantMovimientos], "B%d", pasos);

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

int juegoDesencolarYProcesar(tCola *cola, tEstadoJuego *estado)
{
    tMovimiento mov;
    int llego;

    while (!colaEstaVacia(cola))
    {
        colaSacar(cola, &mov, sizeof(tMovimiento));

        if (!mov.esBandido)
        {
            llego = juegoMoverJugador(estado, mov.direccion, mov.pasos);
            if (llego)
                return 0;  /* gano */

            juegoAplicarEfecto(estado);

            if (juegoVerificarColision(estado))
                if (estado->vidas <= 0)
                    return 0;  /* perdio */

            esperarEnter();
        }
        else
        {
            juegoMoverBandido(estado, mov.idBandido);

            if (juegoVerificarColision(estado))
                if (estado->vidas <= 0)
                    return 0;  /* perdio */
        }

    }

    return 1;  /* partida continua */
}

int juegoJugar(tConfig *config, const char *nombreJugador, int *puntos)
{
    tEstadoJuego estado;
    tCola cola;
    char historial[MAX_HISTORIAL][10], opcion;
    int cantMovimientos = 0, partidaActiva = 1, gano = 0, dado;
    tDireccion dir;
    tPartida partida;

    /* Generar y escribir caravana.txt */
    if (!tableroGenerar(config))
    {
        printf("Error al generar el tablero.\n");
        return 0;
    }

    /* Inicializar estado: lee caravana.txt y llena la lista */
    if (!juegoInicializar(&estado, config))
    {
        printf("Error al inicializar el juego.\n");
        return 0;
    }

    /* Inicializar cola */
    colaCrear(&cola);

    printf("=== CARAVANA DEL DESIERTO ===\n");
    printf("Jugador: %s | Vidas: %d | Puntos: %d\n", nombreJugador, estado.vidas, estado.puntos);
    esperarEnter();

    /* Turno */
    while (partidaActiva)
    {
        system("cls");
        tableroMostrar(&estado.ruta, config->cantidadPosiciones);
        printf("Vidas: %d | Puntos: %d\n", estado.vidas, estado.puntos);

        /* Turno perdido por tormenta */
        if (estado.pierdeTurno)
        {
            printf("\nPerdes este turno por la tormenta!\n");
            estado.pierdeTurno = 0;

            /* Los bandidos igual se mueven con la posicion anterior del jugador */
            juegoEncolarMovBandidos(&cola, &estado);
            partidaActiva = juegoDesencolarYProcesar(&cola, &estado);
            continue;
        }

        /* Lanzar dado */
        dado = juegoLanzarDado();
        printf("\nTiro del dado: %d\n", dado);
        printf("\nMover hacia adelante(A) o atras(S)? ");
        scanf(" %c", &opcion);
        while(getchar() != '\n');

        dir = (opcion == 'A' || opcion == 'a')? MOV_ADELANTE : MOV_ATRAS;

        /* Encolar: primero bandidos (usan posicion anterior del jugador), despues el jugador */
        juegoEncolarMovBandidos(&cola, &estado);
        juegoEncolarMovJugador(&cola, dir, dado);

        /* Registrar en hist */
        juegoRegistrarMovimiento(historial, &cantMovimientos, dir, dado);

        /* Desencolar */
        partidaActiva = juegoDesencolarYProcesar(&cola, &estado);

        /* Verificar condiciones de fin */
        if (estado.nodoJugador == estado.nodoSalida)
        {
            gano = 1;
            partidaActiva = 0;
        }
        else if (estado.vidas <= 0)
        {
            gano = 0;
            partidaActiva = 0;
        }

    }

    /* Fin de partida */
    tableroMostrar(&estado.ruta, config->cantidadPosiciones);

    if (gano)
        printf("\nLlegaste a la Ciudad Refugio! Ganaste con %d puntos!\n", estado.puntos);
    else
        printf("\nSe agotaron tus vidas. Fin del juego.\n");

    juegoMostrarHistorial(historial, cantMovimientos);

    /* Guardo partida */
    strncpy(partida.nombreJugador, nombreJugador, MAX_NOMBRE - 1);
    partida.nombreJugador[MAX_NOMBRE - 1] = '\0';
    partida.puntos = estado.puntos;
    partida.cantMovimientos = cantMovimientos;
    partida.gano = gano;
    guardarPartida(&partida);

    /* Devolvemos los puntos */
    *puntos = estado.puntos;

    /* Libero memoria */
    colaVaciar(&cola);
    juegoDestruir(&estado);

    return gano;
}
