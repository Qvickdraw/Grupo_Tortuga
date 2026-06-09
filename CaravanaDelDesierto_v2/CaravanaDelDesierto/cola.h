#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#define minimo(x, y)((x) <= (y) ? (x) : (y))




typedef struct nodoCola
{
    void* info;
    unsigned tamInfo;
    struct nodoCola *sig;
} nodoCola;

typedef struct
{
    nodoCola *prim,
            *ult;
} cola;
void crearCola(cola *p);
int colaVacia(cola *p);
int colaLlena(cola *p, unsigned cantBytes);
int ponerEnCola(cola *p, const void *d, unsigned cantBytes);
int sacarDeCola(cola* p, void* d, unsigned cantBytes);
void mostrarColaMovimientos(cola* p);
int verPrimeroCola(const cola* p, void* d, unsigned cantBytes);
void vaciarCola(cola* p);

#endif // COLA_H_INCLUDED
