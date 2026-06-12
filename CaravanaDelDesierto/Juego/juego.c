#include "juego.h"
#include "../registros/registros.h"
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
        mov.pasos = juegoLanzarDado();

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
                dir = MOV_ATRAS; /* rebote */
            else
                llego = 1;
        }

    }

    celdaDestino = (tCelda*)estado->nodoJugador->info;
    celdaDestino->tieneJugador = 1;

    return llego;
}

/* Función auxiliar para la IA de los bandidos (Búsqueda bidireccional en anillo) */
char calcularCaminoMasCorto(tNodoLista* nodoBandido, tNodoLista* nodoJugador)
{
    /* Si ya están en el mismo lugar, por defecto devolvemos Forward */
    if (nodoBandido == nodoJugador) return 'F';

    tNodoLista* exploradorF = nodoBandido;
    tNodoLista* exploradorB = nodoBandido;

    /* Mandamos dos punteros a caminar en direcciones opuestas al mismo tiempo */
    while (exploradorF != nodoJugador && exploradorB != nodoJugador)
    {
        exploradorF = exploradorF->sig;
        if (exploradorF == nodoJugador) return 'F'; /* Forward (Adelante) */

        exploradorB = exploradorB->ant;
        if (exploradorB == nodoJugador) return 'B'; /* Backward (Atrás) */
    }

    return 'F'; /* Retorno de seguridad */
}

/* Función principal de movimiento del bandido actualizada */
void juegoMoverBandido(tEstadoJuego *estado, int indiceBandido, int pasos)
{
    tCelda *celdaNueva;
    tCelda *celdaVieja = (tCelda*)estado->nodosBandidos[indiceBandido]->info;

    /* 1. Quitamos al bandido de la celda actual */
    if (celdaVieja->cantBandidos > 0)
        celdaVieja->cantBandidos--;

    /* 2. Calculamos el camino real más corto en la topología circular */
    char direccionOptima = calcularCaminoMasCorto(estado->nodosBandidos[indiceBandido], estado->nodoJugador);

    /* 3. Movemos el puntero del bandido hacia la dirección ganadora */
    while (pasos > 0)
    {
        if (direccionOptima == MOV_ADELANTE)
            estado->nodosBandidos[indiceBandido] = estado->nodosBandidos[indiceBandido]->sig;
        else
            estado->nodosBandidos[indiceBandido] = estado->nodosBandidos[indiceBandido]->ant;

        pasos--;
    }
    /* 4. Ponemos al bandido en la nueva celda física */
    celdaNueva = (tCelda *)estado->nodosBandidos[indiceBandido]->info;
    celdaNueva->cantBandidos++;
}

void juegoAplicarEfecto(tEstadoJuego *estado)
{
    tCelda *celda = (tCelda*)estado->nodoJugador->info;

    /* ELIMINAMOS EL system("cls"); QUE ESTABA ACÁ */

    /* Si la celda no tenia ningun efecto avisamos */
    if (!celda->tieneTormenta && !celda->tienePremio &&
            !celda->tieneVida && !celda->tieneOasis)
        printf("\nCasillero vacio. Sin efectos este turno.\n"); /* Agregamos \n al principio */

    /* Tormenta */
    if (celda->tieneTormenta)
    {
        if (estado->protegido)
            printf("\nEl oasis te protegio de la tormenta!\n");
        else
        {
            estado->pierdeTurno = 1;
            printf("\nTormenta de arena! Perdes el proximo turno.\n");
        }
    }

    /* Premio */
    if (celda->tienePremio)
    {
        estado->puntos++;
        celda->tienePremio = 0;
        printf("\nEncontraste un premio! Puntos: %d\n", estado->puntos);
    }

    /* Vida extra */
    if (celda->tieneVida)
    {
        estado->vidas++;
        celda->tieneVida = 0;
        printf("\nEncontraste una vida extra! Vidas: %d\n", estado->vidas);
    }

    /* Oasis */
    if (celda->tieneOasis)
    {
        estado->protegido = 1;
        printf("\nDescansaste en un oasis! Estaras protegido el proximo turno.\n");
    }

    /* Consumimos proteccion si no estamos en oasis */
    if (estado->protegido && !celda->tieneOasis)
        estado->protegido = 0;
}

int juegoVerificarColision(tEstadoJuego *estado, int cantPosiciones)
{
    if (estado->protegido)
        return 0;

    int i, j;
    int bandidoBorrado = 0; /* Bandera para evitar el break */
    tCelda *celdaInicio;
    tCelda *celdaJugador = (tCelda*)estado->nodoJugador->info;

    if (celdaJugador->cantBandidos == 0)
        return 0;

    printf("\n¡Un bandido te intercepto! Perdes una vida.\n");
    celdaJugador->tieneJugador = 0;

    /* Agregamos la bandera a la condición del for */
    for (i = 0; i < estado->cantBandidosVivos && !bandidoBorrado; i++)
    {
        if (estado->nodosBandidos[i] == estado->nodoJugador)
        {
            celdaJugador->cantBandidos--;

            for (j = i; j < estado->cantBandidosVivos - 1; j++)
                estado->nodosBandidos[j] = estado->nodosBandidos[j + 1];

            estado->cantBandidosVivos--;

            /* Encendemos la bandera. El ciclo for va a cortar solo en la siguiente evaluación */
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

int juegoDesencolarYProcesar(tCola *cola, tEstadoJuego *estado, int cantPosiciones)
{

    tMovimiento mov;
    int llego = 0;

    system("cls");
    printf("=== MOVIMIENTOS DEL TURNO ===\n");

    while (!colaEstaVacia(cola))
    {
        colaSacar(cola, &mov, sizeof(tMovimiento));

        if (!mov.esBandido)
        {
            printf("Jugador:%s%d\n", mov.direccion == MOV_ADELANTE ? "F" : "B",mov.pasos);
            llego = juegoMoverJugador(estado, mov.direccion, mov.pasos);
        }
        else
        {
            printf("Bandido:%s%d\n", mov.direccion == MOV_ADELANTE ? "F" : "B",mov.pasos);
            juegoMoverBandido(estado, mov.idBandido, mov.pasos);
        }
    }


    printf("=== RESULTADO DEL TURNO ===\n");
    tableroMostrar(&estado->ruta, cantPosiciones);

    if (juegoVerificarColision(estado, cantPosiciones))
    {
        /* Si el jugador pierde todas sus vidas, devolvemos 0 para terminar */
        if (estado->vidas <= 0)
            return 0;
    }
    else if(!llego)
    {
        /* Solo si no hubo colisión, aplicamos los efectos de la celda */
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

    printf("\n=== CARAVANA DEL DESIERTO ===\n");
    printf("Jugador: %s | Vidas: %d | Puntos: %d\n", nombreJugador, estado.vidas, estado.puntos);
    esperarEnter();

    /* Turno */

    while (partidaActiva)
    {

        system("cls");
        tableroMostrar(&estado.ruta, config->cantidadPosiciones);
        printf("Vidas: %d | Puntos: %d\n", estado.vidas, estado.puntos);

        /* --- BIFURCACIÓN ESTRUCTURADA DEL TURNO --- */
        if (estado.pierdeTurno)
        {
            /* CASO A: ESTÁ ATRAPADO EN LA TORMENTA */
            printf("\n¡¡¡TURNO PERDIDO POR TORMENTA!!!\n");
            printf("\n--- Los bandidos aprovecharon la tormenta y se movieron ---\n");
            estado.pierdeTurno = 0;

            /* Los bandidos igual se mueven */
            juegoEncolarMovBandidos(&cola, &estado);
            partidaActiva = juegoDesencolarYProcesar(&cola, &estado, config->cantidadPosiciones);

        }
        else
        {

            /* CASO B: TURNO NORMAL DEL JUGADOR */
            printf("\nPresione Enter para TIRAR EL DADO...");
            while(getchar() != '\n'); /* Frena el juego hasta que el jugador apriete Enter */

            dado = juegoLanzarDado();
            printf("\n¡Sacaste un %d!\n", dado);
            printf("\nMover hacia adelante(A) o atras(S)? ");
            do
            {
                scanf(" %c", &opcion);
                while(getchar() != '\n');
                if(toupper(opcion)!= 'A' ||  toupper(opcion)!= 'S')
                {
                    printf("\nDireccion elegida invalida\n");
                    printf("\nMover hacia adelante(A) o atras(S)? ");
                }
            }while(toupper(opcion)!= 'A' &&  toupper(opcion)!= 'S');


            dir = (toupper(opcion) == 'A')? MOV_ADELANTE : MOV_ATRAS;

            /* Encolar: primero bandidos, despues el jugador */
            juegoEncolarMovJugador(&cola, dir, dado);
            juegoEncolarMovBandidos(&cola, &estado);


            /* Registrar en historial */
            juegoRegistrarMovimiento(historial, &cantMovimientos, dir, dado);

            /* Desencolar y procesar */
            partidaActiva = juegoDesencolarYProcesar(&cola, &estado, config->cantidadPosiciones);
        }

        /* --- VERIFICACIÓN GENERAL DE FIN DE PARTIDA --- */
        /* Esto se ejecuta siempre, sin importar qué tipo de turno fue */
        if (estado.nodoJugador == estado.nodoSalida && partidaActiva == 0)
        {
            gano = 1;
        }
        else if (estado.vidas <= 0)
        {
            gano = 0;
        }
        //////////pendiente a cambio////////////////
    }


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
    guardarPartidaBD(&partida);

    /* Devolvemos los puntos */
    *puntos = estado.puntos;

    /* Libero memoria */
    colaVaciar(&cola);
    juegoDestruir(&estado);

    return gano;
}
