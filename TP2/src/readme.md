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

- En Linux: `./tp2 archivo cantidad #iteraciones tolerancia`
- En Windows: `./tp2.exe archivo cantidad #iteraciones tolerancia`

Ejemplo: `./tp2 ../tests/karateclub_matriz.txt 1 10 0.0001`

## Experimentación

La experimentación está divida por las secciones del TP en los siguientes archivos:
- Sección 1: `python/seccion1.ipynb`
- Sección 2: `python/seccion2.ipynb`
- Sección 3: `python/seccion3.ipynb`