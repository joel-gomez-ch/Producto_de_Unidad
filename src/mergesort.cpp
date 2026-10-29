#include "../include/ordenamientos.h"

// Combina dos mitades ordenadas.
void mezclar(std::vector<int>& arreglo,
             int inicio, int medio, int fin,
             Metricas& metricas) {

    std::vector<int> auxiliar;
    auxiliar.reserve(fin - inicio + 1);

    int i = inicio;
    int j = medio + 1;

    // Comparar elementos de ambas mitades.
    while (i <= medio && j <= fin) {
        metricas.comparaciones++;

        if (arreglo[i] <= arreglo[j]) {
            auxiliar.push_back(arreglo[i]);
            metricas.movimientos++;
            i++;
        } else {
            auxiliar.push_back(arreglo[j]);
            metricas.movimientos++;
            j++;
        }
    }

    // Copiar los elementos restantes de la primera mitad.
    while (i <= medio) {
        auxiliar.push_back(arreglo[i]);
        metricas.movimientos++;
        i++;
    }

    // Copiar los elementos restantes de la segunda mitad.
    while (j <= fin) {
        auxiliar.push_back(arreglo[j]);
        metricas.movimientos++;
        j++;
    }

    // Copiar el resultado al arreglo original.
    for (int k = 0; k < static_cast<int>(auxiliar.size()); k++) {
        arreglo[inicio + k] = auxiliar[k];
        metricas.movimientos++;
    }
}

// Función recursiva.
void ordenarMerge(std::vector<int>& arreglo,
                  int inicio, int fin,
                  Metricas& metricas,
                  int profundidad) {

    // Registrar la mayor profundidad alcanzada.
    if (profundidad > metricas.profundidadMaxima) {
        metricas.profundidadMaxima = profundidad;
    }

    if (inicio >= fin) {
        return;
    }

    int medio = inicio + (fin - inicio) / 2;

    ordenarMerge(arreglo, inicio, medio,
                 metricas, profundidad + 1);

    ordenarMerge(arreglo, medio + 1, fin,
                 metricas, profundidad + 1);

    mezclar(arreglo, inicio, medio, fin, metricas);
}

// Función pública.
void mergeSort(std::vector<int>& arreglo, Metricas& metricas) {

    metricas = Metricas{};

    if (arreglo.empty()) {
        return;
    }

    ordenarMerge(arreglo, 0,
                 static_cast<int>(arreglo.size()) - 1,
                 metricas, 1);
}