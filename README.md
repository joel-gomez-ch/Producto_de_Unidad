
# Comparación experimental de MergeSort y QuickSort

## Video de demostracion del proyecto:

----> https://youtu.be/SoTj-caOBTw?si=ljfVldpPOSwirQJ8 <----

## 1. Descripción del proyecto

Este proyecto compara experimentalmente el rendimiento de los algoritmos de ordenamiento **MergeSort** y **QuickSort**, implementados en C++17.

El objetivo es analizar su comportamiento ante diferentes tamaños y patrones de entrada, considerando el tiempo de ejecución, las comparaciones, los movimientos, los intercambios y la profundidad máxima de recursión.

El experimento utiliza entradas reproducibles y múltiples repeticiones para facilitar la comparación de ambos algoritmos bajo las mismas condiciones.

## 2. Autor

- **Estudiante:** Joel Joshet Gomez Chachaque
- **Curso:** Programación Avanzada
- **Institución:** Universidad Nacional Jorge Basadre Grohmann

## 3. Algoritmos seleccionados

### MergeSort
Algoritmo basado en la técnica de divide y vencerás. Divide el arreglo en partes más pequeñas, las ordena recursivamente y luego combina los resultados.

- Complejidad temporal: O(n log n) en el mejor, promedio y peor caso.
- Memoria auxiliar: O(n) para esta implementación.
- Estabilidad: puede ser estable según la implementación.

### QuickSort
Algoritmo basado en divide y vencerás. Divide el arreglo mediante una partición alrededor de un pivote y ordena recursivamente las partes resultantes.

- Complejidad temporal promedio: O(n log n).
- Complejidad temporal en el peor caso: O(n²).
- Profundidad de recursión: depende del equilibrio de las particiones.
- Estabilidad: generalmente no es estable.

## 4. Estructura del proyecto

```text
comparacion-ordenamientos/
├── include/
│   └── ordenamientos.h
├── src/
│   ├── main.cpp
│   ├── mergesort.cpp
│   ├── quicksort.cpp
│   └── experimento.cpp
├── scripts/
│   └── analizar_resultados.py
├── results/
│   ├── resultados.csv
│   └── estadisticas/
├── docs/
├── README.md
└── .gitignore
```

- `include/`: declaraciones de funciones y estructuras utilizadas.
- `src/`: implementaciones de los algoritmos y del experimento.
- `scripts/`: análisis estadístico y generación de gráficos.
- `results/`: resultados experimentales y estadísticas generadas.
- `docs/`: documentación complementaria del proyecto.

## 5. Requisitos de compilación y ejecución

### Requisitos

- Compilador compatible con C++17, como GCC/G++.
- Python 3 para el análisis estadístico.
- Bibliotecas de Python: `pandas` y `matplotlib`.
- Git Bash o una terminal compatible para ejecutar los comandos.

### Compilar el programa

Desde la raíz del proyecto, ejecutar:

```bash
g++ -std=c++17 -O2 -Iinclude src/mergesort.cpp src/quicksort.cpp src/experimento.cpp -o experimento.exe
```

### Ejecutar el experimento

En Git Bash, ejecutar:

```bash
.\experimento.exe "$(git rev-parse HEAD)"
```

El programa genera el archivo `results/resultados.csv` con las mediciones del experimento.

> Ejecuta el programa desde la raíz del repositorio para que las rutas de los archivos de salida coincidan con las esperadas.

### Analizar los resultados

Instalar las dependencias si aún no están disponibles:

```bash
python -m pip install pandas matplotlib
```

Luego ejecutar:

```bash
python scripts/analizar_resultados.py
```

El script analiza los resultados y genera las estadísticas y gráficos definidos en su implementación.

## 6. Diseño experimental

Se evaluaron tres tamaños de arreglo:

- 1 000 elementos.
- 10 000 elementos.
- 100 000 elementos.

Se utilizaron cinco patrones de entrada:

| Patrón | Descripción |
| Aleatorio | Valores generados aleatoriamente. |
| Ascendente | Elementos ordenados de menor a mayor. |
| Descendente | Elementos ordenados de mayor a menor. |
| Casi ordenado | Arreglo ordenado con algunos intercambios aleatorios. |
| Duplicados | Valores repetidos dentro de un rango reducido. |

Cada combinación de tamaño y patrón se ejecutó 30 veces para cada algoritmo.

En total, se realizaron **900 ejecuciones: 450 por algoritmo**.

Para mejorar la reproducibilidad, se utilizaron semillas deterministas y se proporcionó la misma entrada a ambos algoritmos en cada comparación.

### Métricas registradas

- `tiempo_ms`: tiempo de ejecución en milisegundos.
- `comparaciones`: comparaciones contabilizadas por la implementación.
- `movimientos`: movimientos de elementos contabilizados.
- `intercambios`: intercambios realizados.
- `profundidad_maxima`: profundidad máxima de recursión registrada.
- `semilla`: semilla utilizada para generar la entrada.
- `commit_hash`: identificador del commit asociado a la ejecución.

La medición del tiempo utiliza `std::chrono::steady_clock` y excluye la generación de datos, las copias, la validación y la escritura del archivo.

## 7. Resultados y conclusiones principales

1. Rendimiento: La comparación experimental permitió identificar diferencias de rendimiento entre MergeSort y QuickSort según el tamaño y el patrón de los datos, demostrando que la eficiencia práctica depende tanto del algoritmo como de su implementación.
2. Complejidad algorítmica: Aunque ambos algoritmos presentan un comportamiento promedio de O(n log⁡n ), sus tiempos de ejecución y cantidades de operaciones pueden variar, por lo que la complejidad teórica debe complementarse con mediciones experimentales.
3. Estabilidad y recursión: MergeSort puede mantener el orden relativo de elementos iguales, mientras que QuickSort generalmente no garantiza estabilidad. Además, la profundidad recursiva de ambos algoritmos influye en el uso de memoria, especialmente cuando QuickSort genera particiones desequilibradas.
4. Alcance del experimento: Las pruebas realizadas con distintos tamaños, patrones y repeticiones permitieron comparar ambos métodos bajo condiciones definidas; sin embargo, los resultados corresponden a las implementaciones y al entorno evaluados, por lo que no deben generalizarse a todas las situaciones.

Los resultados detallados se encuentran en `results/resultados.csv`, mientras que las estadísticas y los gráficos se almacenan en `results/`.

## 8. Reproducción y disponibilidad de los resultados

Para reproducir el experimento:

1. Clonar el repositorio.
2. Compilar el programa siguiendo las instrucciones anteriores.
3. Ejecutar el experimento desde la raíz del proyecto.
4. Analizar el CSV generado mediante el script de Python.

El archivo CSV permite revisar las mediciones individuales y verificar las comparaciones realizadas.

## 9. Video de demostración

**Ver video de demostración en YouTube** `[https://youtu.be/SoTj-caOBTw?si=ljfVldpPOSwirQJ8]`

El video presenta la ejecución del programa y una explicación de los resultados obtenidos.

## 10. Versión final del experimento

- **Repositorio:** [https://github.com/joel-gomez-ch/Producto_de_Unidad]

Para consultar el commit registrado en una ejecución, se puede utilizar:

```bash
git rev-parse HEAD
```

Este identificador permite relacionar los resultados con una versión concreta del código.

## 11. Referencias bibliográficas

Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2022). *Introduction to algorithms* (4th ed.). The MIT Press.

Hoare, C. A. R. (1962). Quicksort. *The Computer Journal, 5*(1), 10–16. https://doi.org/10.1093/comjnl/5.1.10