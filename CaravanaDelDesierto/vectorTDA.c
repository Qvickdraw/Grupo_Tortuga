#include "caravana.h"



int crearVectorInicializado(VectorTDA* vector, int capacidad, unsigned tamElem)
{

    if (capacidad <= 0 || tamElem <= 0)
    {
        return SIN_MEM;
    }


    vector->vec = calloc(capacidad, tamElem);


    if (vector->vec == NULL)
    {
        return SIN_MEM;
    }

    vector->cap = capacidad;
    vector->ce = capacidad;
    vector->tamElem = tamElem;

    return TODO_OK;
}

int obtenerDeVector(VectorTDA* vector, void* aux, int pos)
{

    if (pos < 0 || pos >= vector->cap)
    {
        return ERR_VEC;
    }

    void* direccionOrigen = (vector->vec) + (pos * vector->tamElem);


    memcpy(aux, direccionOrigen, vector->tamElem);

    return TODO_OK;
}

int actualizarEnVector(VectorTDA* vector, void* aux, int pos)
{

    if (pos < 0 || pos >= vector->cap)
    {
        return ERR_VEC;
    }

    void* direccionDestino = (vector->vec) + (pos * vector->tamElem);

    memcpy(direccionDestino, aux, vector->tamElem);

    return TODO_OK;
}

int destruirVector(VectorTDA* vector)
{
    if (vector->vec != NULL)
    {
        free(vector->vec);
        vector->vec = NULL;
    }


    vector->ce = 0;
    vector->cap = 0;
    vector->tamElem = 0;
    return TODO_OK;
}
