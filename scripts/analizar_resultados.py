from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt


# =========================================================
# 1. CONFIGURACIÓN
# =========================================================

BASE = Path(__file__).resolve().parent.parent
ARCHIVO_ENTRADA = BASE / "results" / "resultados.csv"
CARPETA_SALIDA = BASE / "results" / "estadisticas"
CARPETA_GRAFICOS = CARPETA_SALIDA / "graficos"

CARPETA_GRAFICOS.mkdir(parents=True, exist_ok=True)

ALGORITMOS = ["MergeSort", "QuickSort"]
TAMANOS = [1000, 10000, 100000]

PATRONES = [
    "aleatorio",
    "ascendente",
    "descendente",
    "casi_ordenado",
    "duplicados",
]

METRICAS = [
    "tiempo_ms",
    "comparaciones",
    "movimientos",
    "intercambios",
    "profundidad_maxima",
]

COLUMNAS = [
    "algoritmo",
    "n",
    "patron",
    "repeticion",
    "tiempo_ms",
    "comparaciones",
    "movimientos",
    "intercambios",
    "profundidad_maxima",
    "semilla",
    "commit_hash",
]


# =========================================================
# 2. CARGA Y VALIDACIÓN
# =========================================================

def cargar_datos():
    if not ARCHIVO_ENTRADA.exists():
        raise FileNotFoundError(
            f"No se encontró el archivo: {ARCHIVO_ENTRADA}"
        )

    df = pd.read_csv(ARCHIVO_ENTRADA)

    faltantes = set(COLUMNAS) - set(df.columns)

    if faltantes:
        raise ValueError(
            f"Faltan columnas en el CSV: {sorted(faltantes)}"
        )

    if len(df) != 900:
        raise ValueError(
            f"Se esperaban 900 registros, pero hay {len(df)}."
        )

    if df[COLUMNAS].isnull().any().any():
        raise ValueError("Hay valores vacíos en el CSV.")

    if not df["algoritmo"].isin(ALGORITMOS).all():
        raise ValueError("Se encontró un algoritmo desconocido.")

    if not df["n"].isin(TAMANOS).all():
        raise ValueError("Se encontró un tamaño inesperado.")

    if not df["patron"].isin(PATRONES).all():
        raise ValueError("Se encontró un patrón desconocido.")

    columnas_numericas = [
        "n",
        "repeticion",
        "tiempo_ms",
        "comparaciones",
        "movimientos",
        "intercambios",
        "profundidad_maxima",
        "semilla",
    ]

    for columna in columnas_numericas:
        df[columna] = pd.to_numeric(
            df[columna], errors="raise"
        )

    if (df["tiempo_ms"] < 0).any():
        raise ValueError("Hay tiempos negativos.")

    if (df[METRICAS[1:]] < 0).any().any():
        raise ValueError("Hay métricas negativas.")

    if df.duplicated(
        ["algoritmo", "n", "patron", "repeticion"]
    ).any():
        raise ValueError("Hay ejecuciones duplicadas.")

    # Comprobar que existan 30 repeticiones por combinación.
    conteos = df.groupby(
        ["algoritmo", "n", "patron"]
    ).size()

    if len(conteos) != 30:
        raise ValueError(
            "No están presentes todas las combinaciones."
        )

    if not (conteos == 30).all():
        raise ValueError(
            "Alguna combinación no tiene 30 repeticiones."
        )

    # Comprobar que ambos algoritmos utilizaron las mismas semillas.
    claves = ["n", "patron", "repeticion"]

    merge = (
        df[df["algoritmo"] == "MergeSort"]
        .set_index(claves)["semilla"]
        .sort_index()
    )

    quick = (
        df[df["algoritmo"] == "QuickSort"]
        .set_index(claves)["semilla"]
        .sort_index()
    )

    if not merge.equals(quick):
        raise ValueError(
            "Las semillas de ambos algoritmos no coinciden."
        )

    print("Validación correcta: 900 registros.")
    print("Todas las combinaciones tienen 30 repeticiones.")
    print("Las semillas coinciden entre algoritmos.")

    return df


# =========================================================
# 3. ESTADÍSTICAS DESCRIPTIVAS
# =========================================================

def calcular_estadisticas(df):
    agrupacion = df.groupby(
        ["algoritmo", "n", "patron"]
    )[METRICAS]

    resumen = agrupacion.agg(
        ["mean", "median", "std", "min", "max"]
    )

    # Convertir las columnas múltiples en nombres simples.
    resumen.columns = [
        f"{metrica}_{estadistica}"
        for metrica, estadistica in resumen.columns
    ]

    resumen = resumen.reset_index()

    ruta = CARPETA_SALIDA / "resumen_estadistico.csv"
    resumen.to_csv(ruta, index=False, float_format="%.6f")

    print(f"Resumen guardado en: {ruta}")

    return resumen


# =========================================================
# 4. COMPARACIÓN EMPAREJADA DE TIEMPOS
# =========================================================

def calcular_comparacion_emparejada(df):
    claves = ["n", "patron", "repeticion"]

    tiempos = df.pivot(
        index=claves,
        columns="algoritmo",
        values="tiempo_ms",
    ).reset_index()

    if tiempos[ALGORITMOS].isnull().any().any():
        raise ValueError(
            "Faltan tiempos para alguna ejecución emparejada."
        )

    # Razón de tiempos de cada pareja.
    # Un valor > 1 indica que QuickSort fue más rápido.
    tiempos["razon_Merge_sobre_Quick"] = (
        tiempos["MergeSort"] / tiempos["QuickSort"]
    )

    tiempos["diferencia_Merge_menos_Quick_ms"] = (
        tiempos["MergeSort"] - tiempos["QuickSort"]
    )

    comparacion = tiempos.groupby(
        ["n", "patron"]
    ).agg(
        media_tiempo_MergeSort=("MergeSort", "mean"),
        media_tiempo_QuickSort=("QuickSort", "mean"),
        media_diferencia_ms=(
            "diferencia_Merge_menos_Quick_ms", "mean"
        ),
        mediana_razon=(
            "razon_Merge_sobre_Quick", "median"
        ),
        media_razon=(
            "razon_Merge_sobre_Quick", "mean"
        ),
    ).reset_index()

    ruta = CARPETA_SALIDA / "comparacion_emparejada.csv"
    comparacion.to_csv(
        ruta, index=False, float_format="%.6f"
    )

    print(f"Comparación emparejada guardada en: {ruta}")

    return comparacion


# =========================================================
# 5. GRÁFICOS DE TIEMPO
# =========================================================

def graficar_tiempos(resumen):
    for patron in PATRONES:
        datos = resumen[resumen["patron"] == patron]

        fig, ax = plt.subplots(figsize=(8, 5))

        for algoritmo in ALGORITMOS:
            parte = datos[
                datos["algoritmo"] == algoritmo
            ].sort_values("n")

            ax.errorbar(
                parte["n"],
                parte["tiempo_ms_mean"],
                yerr=parte["tiempo_ms_std"].fillna(0),
                marker="o",
                capsize=4,
                label=algoritmo,
            )

        ax.set_title(f"Tiempo de ejecución: {patron}")
        ax.set_xlabel("Número de elementos (n)")
        ax.set_ylabel("Tiempo medio (ms)")
        ax.set_xticks(TAMANOS)
        ax.set_xticklabels(
            [f"{n:,}" for n in TAMANOS]
        )
        ax.legend()
        ax.grid(True, alpha=0.3)
        fig.tight_layout()

        ruta = CARPETA_GRAFICOS / f"tiempo_{patron}.png"
        fig.savefig(ruta, dpi=300)
        plt.close(fig)

    print("Se generaron cinco gráficos de tiempo.")


# =========================================================
# 6. GRÁFICO DE COMPARACIONES
# =========================================================

def graficar_comparaciones(resumen):
    for patron in PATRONES:
        datos = resumen[resumen["patron"] == patron]

        fig, ax = plt.subplots(figsize=(8, 5))

        for algoritmo in ALGORITMOS:
            parte = datos[
                datos["algoritmo"] == algoritmo
            ].sort_values("n")

            ax.plot(
                parte["n"],
                parte["comparaciones_mean"],
                marker="o",
                label=algoritmo,
            )

        ax.set_title(f"Comparaciones: {patron}")
        ax.set_xlabel("Número de elementos (n)")
        ax.set_ylabel("Comparaciones medias")
        ax.set_xticks(TAMANOS)
        ax.set_xticklabels(
            [f"{n:,}" for n in TAMANOS]
        )
        ax.legend()
        ax.grid(True, alpha=0.3)
        fig.tight_layout()

        ruta = CARPETA_GRAFICOS / (
            f"comparaciones_{patron}.png"
        )
        fig.savefig(ruta, dpi=300)
        plt.close(fig)

    print("Se generaron cinco gráficos de comparaciones.")


# =========================================================
# 7. PROGRAMA PRINCIPAL
# =========================================================

def main():
    CARPETA_SALIDA.mkdir(parents=True, exist_ok=True)

    df = cargar_datos()

    resumen = calcular_estadisticas(df)

    calcular_comparacion_emparejada(df)

    graficar_tiempos(resumen)

    graficar_comparaciones(resumen)

    print("\nProcesamiento estadístico terminado.")
    print(f"Archivos generados en: {CARPETA_SALIDA}")


if __name__ == "__main__":
    main()