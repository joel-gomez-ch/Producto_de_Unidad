#ifndef ORDENAMIENTOS_H
#define ORDENAMIENTOS_H

#include <vector>

struct Metricas {
    unsigned long long comparaciones = 0;
    unsigned long long movimientos = 0;
    unsigned long long intercambios = 0;
    int profundidadMaxima = 0;
};

void mergeSort(std::vector<int>& arreglo, Metricas& metricas);
void quickSort(std::vector<int>& arreglo, Metricas& metricas);

#endif