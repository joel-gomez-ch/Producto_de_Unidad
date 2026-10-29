#include "../include/ordenamientos.h"

#include <algorithm>
#include <iostream>
#include <vector>

void mostrarVector(const std::vector<int>& arreglo) {
    std::cout << "[ ";

    for (int elemento : arreglo) {
        std::cout << elemento << " ";
    }

    std::cout << "]\n";
}

bool probarCaso(const std::vector<int>& original) {

    // Cada algoritmo recibe una copia idéntica.
    std::vector<int> datosMerge = original;
    std::vector<int> datosQuick = original;

    mergeSort(datosMerge);
    quickSort(datosQuick);

    // Resultado esperado, calculado con la biblioteca estándar.
    std::vector<int> esperado = original;
    std::sort(esperado.begin(), esperado.end());

    bool mergeCorrecto = (datosMerge == esperado);
    bool quickCorrecto = (datosQuick == esperado);

    std::cout << "\nEntrada: ";
    mostrarVector(original);

    std::cout << "MergeSort: ";
    mostrarVector(datosMerge);

    std::cout << "QuickSort: ";
    mostrarVector(datosQuick);

    if (mergeCorrecto && quickCorrecto) {
        std::cout << "Resultado: CORRECTO\n";
        return true;
    }

    std::cout << "Resultado: ERROR\n";
    return false;
}

int main() {

    std::vector<std::vector<int>> pruebas = {
        {38, 12, 27, 9, 31, 18, 43, 25},
        {},
        {7},
        {5, 5, 5, 5},
        {1, 2, 3, 4, 5},
        {5, 4, 3, 2, 1},
        {-3, 8, 0, -3, 2, 8}
    };

    int exitos = 0;

    for (const auto& prueba : pruebas) {
        if (probarCaso(prueba)) {
            exitos++;
        }
    }

    std::cout << "\nPruebas superadas: "
              << exitos << " de " << pruebas.size() << "\n";

    return (exitos == static_cast<int>(pruebas.size()))
           ? 0 : 1;
}