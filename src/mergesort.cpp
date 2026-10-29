#include "../include/ordenamientos.h"

// Combina dos mitades que ya están ordenadas.
void mezclar(std::vector<int>& arreglo,
             int inicio, int medio, int fin) {

    std::vector<int> auxiliar;

    int i = inicio;
    int j = medio + 1;

    // Comparar los elementos de ambas mitades.
    while (i <= medio && j <= fin) {
        if (arreglo[i] <= arreglo[j]) {
            auxiliar.push_back(arreglo[i]);
            i++;
        } else {
            auxiliar.push_back(arreglo[j]);
            j++;
        }
    }

    // Copiar los elementos que quedaron en la primera mitad.
    while (i <= medio) {
        auxiliar.push_back(arreglo[i]);
        i++;
    }

    // Copiar los elementos que quedaron en la segunda mitad.
    while (j <= fin) {
        auxiliar.push_back(arreglo[j]);
        j++;
    }

    // Devolver los elementos ordenados al arreglo original.
    for (int k = 0; k < static_cast<int>(auxiliar.size()); k++) {
        arreglo[inicio + k] = auxiliar[k];
    }
}

// Función recursiva auxiliar.
void ordenarMerge(std::vector<int>& arreglo,
                  int inicio, int fin) {

    if (inicio >= fin) {
        return;
    }

    int medio = inicio + (fin - inicio) / 2;

    ordenarMerge(arreglo, inicio, medio);
    ordenarMerge(arreglo, medio + 1, fin);

    mezclar(arreglo, inicio, medio, fin);
}

// Función pública.
void mergeSort(std::vector<int>& arreglo) {
    if (arreglo.empty()) {
        return;
    }

    ordenarMerge(arreglo, 0,
                 static_cast<int>(arreglo.size()) - 1);
}