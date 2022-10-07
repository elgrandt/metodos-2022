# Métodos Numéricos: Trabajo Práctico Nro 2

Esta carpeta contiene el archivo tp2.cpp donde se encuentra la implementación del algoritmo solicitado para este trabajo.

## Integrantes

- Martinelli, Ezequiel (883/19) ezeaam@gmail.com
- Mauro, Andres (39/17) sebaastian_mauro@live.com
- Tasat, Dylan (29/17) dylantasat11@gmail.com

## Generación del ejecutable

Esto se hace como cualquier script de C++:

- En Linux: `g++ -c *.cpp && g++ *.o -o tp2`
- En Windows: `g++ -c *.cpp; g++ *.o -o tp2.exe`

## Ejecución del script

Siguiendo las pautas del TP, la ejecución tiene la siguiente pinta:

- En Linux: `./tp2 archivo #iteraciones tolerancia`
- En Windows: `./tp2.exe archivo #iteraciones tolerancia`

Ejemplo: `./tp2 ../tests/karateclub_matriz.txt 10 0.0001`

## Experimentación

<!-- La experimentación consta de dos fases:
- Generación de instancias: se hace ejecutando el notebook `python/generar-instancias.ipynb`.
- Ejecución de las instancias y generación de resultados: se hace ejecutando el notebook `python/experimentos.ipynb`.

Nota: esto puede tardar debido a los tests de 15 y 30 segundos, que tardan aproximadamente 2 minutos cada uno en nuestra implementación, para la medición de tiempos se ejecuta 5 veces el de 30 segundos por lo que tarda bastante. -->