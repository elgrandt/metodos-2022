# Métodos Numéricos: Trabajo Práctico Nro 3

Esta carpeta contiene el archivo tp3.cpp donde se encuentra la implementación del algoritmo solicitado para este trabajo.

## Integrantes

- Martinelli, Ezequiel (883/19) ezeaam@gmail.com
- Mauro, Andres (39/17) sebaastian_mauro@live.com
- Tasat, Dylan (29/17) dylantasat11@gmail.com

## Generación del ejecutable

Esto se hace como cualquier script de C++:

- En Linux: `g++ -std=c++11  -I ./eigen tp3.cpp -o tp3` 
- En Windows: `g++ -std=c++11  -I ./eigen tp3.cpp -o tp3.exe`

## Ejecución del script

Siguiendo las pautas del TP, la ejecución tiene la siguiente pinta:

- En Linux: `./tp3 archivo p algoritmo`
- En Windows: `./tp3.exe archivo p algoritmo`

Donde los algoritmos disponibles son:
- eg (Eliminacion Gaussiana)
- jb (Jacobi)
- gs (Gauss-Seidel)

Ejemplo: `./tp3 "../tests/test_15_segundos.txt" 0.9 jb`

## Experimentación

La experimentación se encuentra en el siguiente archivo: `python/experimentacion.ipynb`.
Para ejecutar la experimentacion de convergencia hace falta descomentar las lineas del codigo tp3.cpp desde la linea 104 hasta 110 y desde la 158 a la 164 (esto permitira correr solo ese experimento)
