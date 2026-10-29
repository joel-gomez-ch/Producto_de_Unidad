#include "../include/ordenamientos.h"

// Ordena una sección del vector usando partición de Hoare.
void ordenarQuick(std::vector<int>& arreglo,
                  int inicio, int fin) {

    int i = inicio;
    int j = fin;

    // Elegimos como pivote el elemento central.
    int pivote = arreglo[inicio + (fin - inicio) / 2];

    while (i <= j) {

        // Buscar un elemento que debería estar a la derecha.
        while (arreglo[i] < pivote) {
            i++;
        }

        // Buscar un elemento que debería estar a la izquierda.
        while (arreglo[j] > pivote) {
            j--;
        }

        // Intercambiar si los recorridos todavía se cruzan.
        if (i <= j) {
            int temporal = arreglo[i];
            arreglo[i] = arreglo[j];
            arreglo[j] = temporal;

            i++;
            j--;
        }
    }

    // Ordenar recursivamente las regiones restantes.
    if (inicio < j) {
        ordenarQuick(arreglo, inicio, j);
    }

    if (i < fin) {
        ordenarQuick(arreglo, i, fin);
    }
}

// Función pública.
void quickSort(std::vector<int>& arreglo) {
    if (arreglo.empty()) {
        return;
    }

    ordenarQuick(arreglo, 0,
                 static_cast<int>(arreglo.size()) - 1);
}