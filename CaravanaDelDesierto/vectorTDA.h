#ifndef VECTORTDA_H_INCLUDED
#define VECTORTDA_H_INCLUDED



typedef struct
{
    void* vec;
    int ce;
    int cap;
    unsigned tamElem;
} VectorTDA;

int crearVectorInicializado(VectorTDA* vector, int capacidad, unsigned tamElem);
int obtenerDeVector(VectorTDA* vector, void* aux, int pos);
int actualizarEnVector(VectorTDA* vector, void* aux, int pos);
int destruirVector(VectorTDA* vector);
#endif // VECTORTDA_H_INCLUDED
