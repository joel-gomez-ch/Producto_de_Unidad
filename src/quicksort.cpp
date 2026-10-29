#include "../include/ordenamientos.h"

// Ordena una sección usando partición de Hoare.
void ordenarQuick(std::vector<int>& arreglo,
                  int inicio, int fin,
                  Metricas& metricas,
                  int profundidad) {

    if (profundidad > metricas.profundidadMaxima) {
        metricas.profundidadMaxima = profundidad;
    }

    int i = inicio;
    int j = fin;

    int pivote = arreglo[inicio + (fin - inicio) / 2];

    while (i <= j) {

        // Buscar desde la izquierda.
        while (true) {
            metricas.comparaciones++;

            if (!(arreglo[i] < pivote)) {
                break;
            }

            i++;
        }

        // Buscar desde la derecha.
        while (true) {
            metricas.comparaciones++;

            if (!(arreglo[j] > pivote)) {
                break;
            }

            j--;
        }

        if (i <= j) {

            // Solo contar un intercambio si son posiciones distintas.
            if (i < j) {
                int temporal = arreglo[i];
                arreglo[i] = arreglo[j];
                arreglo[j] = temporal;

                metricas.intercambios++;
                metricas.movimientos += 3;
            }

            i++;
            j--;
        }
    }

    // Ordenar las regiones restantes.
    if (inicio < j) {
        ordenarQuick(arreglo, inicio, j,
                     metricas, profundidad + 1);
    }

    if (i < fin) {
        ordenarQuick(arreglo, i, fin,
                     metricas, profundidad + 1);
    }
}

// Función pública.
void quickSort(std::vector<int>& arreglo, Metricas& metricas) {

    metricas = Metricas{};

    if (arreglo.empty()) {
        return;
    }

    ordenarQuick(arreglo, 0,
                 static_cast<int>(arreglo.size()) - 1,
                 metricas, 1);
}