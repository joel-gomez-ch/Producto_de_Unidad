#include "../include/ordenamientos.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

using namespace std;

const uint64_t SEMILLA_BASE = 20261029;

const vector<int> TAMANOS = {1000, 10000, 100000};

const vector<string> PATRONES = {
    "aleatorio",
    "ascendente",
    "descendente",
    "casi_ordenado",
    "duplicados"
};

const int REPETICIONES = 30;

// Genera un vector según el patrón seleccionado.
vector<int> generarDatos(
    int n,
    int patron,
    mt19937_64& generador
) {
    vector<int> datos(n);

    if (patron == 0) {
        // Aleatorio uniforme: valores entre 0 y 1 000 000.
        uniform_int_distribution<int> distribucion(0, 1000000);

        for (int& valor : datos) {
            valor = distribucion(generador);
        }
    }
    else if (patron == 1) {
        // Orden ascendente.
        iota(datos.begin(), datos.end(), 0);
    }
    else if (patron == 2) {
        // Orden descendente.
        iota(datos.begin(), datos.end(), 0);
        reverse(datos.begin(), datos.end());
    }
    else if (patron == 3) {
        // Casi ordenado: aproximadamente 5 % de posiciones
        // afectadas por intercambios aleatorios.
        iota(datos.begin(), datos.end(), 0);

        uniform_int_distribution<int> posicion(0, n - 1);

        int intercambios = n / 40;

        if (n >= 2 && intercambios == 0) {
            intercambios = 1;
        }

        for (int k = 0; k < intercambios; k++) {
            int i = posicion(generador);
            int j = posicion(generador);

            swap(datos[i], datos[j]);
        }
    }
    else if (patron == 4) {
        // Valores repetidos entre 1 y 20.
        uniform_int_distribution<int> distribucion(1, 20);

        for (int& valor : datos) {
            valor = distribucion(generador);
        }
    }

    return datos;
}

// Función que ejecuta y mide un algoritmo.
double medir(
    void (*algoritmo)(vector<int>&, Metricas&),
    vector<int>& datos,
    Metricas& metricas
) {
    auto inicio = chrono::steady_clock::now();

    algoritmo(datos, metricas);

    auto fin = chrono::steady_clock::now();

    return chrono::duration<double, milli>(
        fin - inicio
    ).count();
}

// Comprueba que el resultado esté ordenado.
bool validar(const vector<int>& datos) {
    return is_sorted(datos.begin(), datos.end());
}

// Registra una fila del CSV.
void registrar(
    ofstream& archivo,
    const string& algoritmo,
    int n,
    const string& patron,
    int repeticion,
    double tiempoMs,
    const Metricas& metricas,
    uint64_t semilla,
    const string& commitHash
) {
    archivo
        << algoritmo << ','
        << n << ','
        << patron << ','
        << repeticion << ','
        << tiempoMs << ','
        << metricas.comparaciones << ','
        << metricas.movimientos << ','
        << metricas.intercambios << ','
        << metricas.profundidadMaxima << ','
        << semilla << ','
        << commitHash << '\n';
}

int main(int argc, char* argv[]) {

    string commitHash = "no-disponible";

    if (argc >= 2) {
        commitHash = argv[1];
    }

    ofstream archivo("results/resultados.csv");

    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir results/resultados.csv\n";
        cerr << "Comprueba que exista la carpeta results.\n";
        return 1;
    }

    archivo
        << "algoritmo,n,patron,repeticion,tiempo_ms,"
        << "comparaciones,movimientos,intercambios,"
        << "profundidad_maxima,semilla,commit_hash\n";

    uint64_t ejecuciones = 0;

    for (int indiceN = 0;
         indiceN < static_cast<int>(TAMANOS.size());
         indiceN++) {

        int n = TAMANOS[indiceN];

        for (int indicePatron = 0;
             indicePatron < static_cast<int>(PATRONES.size());
             indicePatron++) {

            const string& patron = PATRONES[indicePatron];

            for (int rep = 0; rep < REPETICIONES; rep++) {

                // Una semilla diferente y reproducible por caso.
                uint64_t semilla =
                    SEMILLA_BASE
                    + static_cast<uint64_t>(indiceN) * 1000000ULL
                    + static_cast<uint64_t>(indicePatron) * 10000ULL
                    + static_cast<uint64_t>(rep);

                mt19937_64 generador(semilla);

                // Crear una sola entrada original.
                vector<int> original =
                    generarDatos(n, indicePatron, generador);

                // Preparar copias idénticas antes de cronometrar.
                vector<int> datosMerge = original;
                vector<int> datosQuick = original;

                Metricas metricasMerge;
                Metricas metricasQuick;

                double tiempoMerge;
                double tiempoQuick;

                // Alternar el orden de ejecución para reducir
                // el sesgo por ejecutar siempre el mismo primero.
                if (rep % 2 == 0) {
                    tiempoMerge = medir(
                        mergeSort, datosMerge, metricasMerge
                    );

                    tiempoQuick = medir(
                        quickSort, datosQuick, metricasQuick
                    );
                }
                else {
                    tiempoQuick = medir(
                        quickSort, datosQuick, metricasQuick
                    );

                    tiempoMerge = medir(
                        mergeSort, datosMerge, metricasMerge
                    );
                }

                // Validar fuera del intervalo cronometrado.
                if (!validar(datosMerge)
                    || !validar(datosQuick)
                    || datosMerge != datosQuick) {

                    cerr << "Error de ordenamiento: "
                         << "n=" << n
                         << ", patron=" << patron
                         << ", repeticion=" << rep << '\n';

                    return 2;
                }

                // Guardar los dos resultados del mismo caso.
                registrar(
                    archivo, "MergeSort", n, patron, rep + 1,
                    tiempoMerge, metricasMerge, semilla, commitHash
                );

                registrar(
                    archivo, "QuickSort", n, patron, rep + 1,
                    tiempoQuick, metricasQuick, semilla, commitHash
                );

                ejecuciones += 2;
            }

            cout << "Completado: n=" << n
                 << ", patron=" << patron << '\n';
        }
    }

    archivo.close();

    cout << "\nExperimento terminado correctamente.\n";
    cout << "Ejecuciones registradas: " << ejecuciones << '\n';
    cout << "Archivo: results/resultados.csv\n";

    return 0;
}