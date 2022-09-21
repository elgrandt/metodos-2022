# Métodos Numéricos: Trabajo Práctico Nro 1

Esta carpeta contiene el archivo tp1.cpp donde se encuentra la implementación del algoritmo solicitado para este trabajo.

## Integrantes

- Martinelli, Ezequiel (883/19) ezeaam@gmail.com
- Mauro, Andres (39/17) sebaastian_mauro@live.com
- Tasat, Dylan (29/17) dylantasat11@gmail.com

## Generación del ejecutable

Esto se hace como cualquier script de C++, con la excepción que hay que agregarle el flag `-lstdc++`, no sabemos la razón exacta de la necesidad de este flag pero sin él la compilacion va a fallar.

- En Linux: `gcc tp1.cpp -o tp1 -lstdc++`
- En Windows: `gcc tp1.cpp -o tp1.exe -lstdc++`

## Ejecución del script

Siguiendo las pautas del TP, la ejecución tiene la siguiente pinta:

- En Linux: `./tp1 archivo p`
- En Windows: `./tp1.exe archivo p`

## Experimentación

La experimentación consta de dos fases:
- Generación de instancias: se hace ejecutando el notebook `python/generar-instancias.ipynb`.
- Ejecución de las instancias y generación de resultados: se hace ejecutando el notebook `python/experimentos.ipynb`.

Nota: esto puede tardar debido a los tests de 15 y 30 segundos, que tardan aproximadamente 2 minutos cada uno en nuestra implementación, para la medición de tiempos se ejecuta 5 veces el de 30 segundos por lo que tarda bastante.