#ifndef MATRIZ_H
#define MATRIZ_H

#include "utils.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
using namespace std;

typedef pair<const Position, double> PositionValuePair;

class SparseMatrix {
private:
    unordered_map<Position, double, Position::HashFunction> cells;
    int width;
    int height;
public:

    SparseMatrix(int width, int height);
    SparseMatrix(const SparseMatrix& other);
    
    double at(int row, int column) const;

    void blind_set(int row, int column, double value);

    void set(int row, int column, double value);

    int getWidth();

    int getHeight();

    friend ostream& operator<<(ostream &os, SparseMatrix const &m) {
        for (int row = 0; row < m.height; row++) {
            for (int col = 0; col < m.width; col++) {
                // os << right << setw(10) << m.at(row, col) << " ";
                os << m.at(row, col) << " ";
            }
            os << endl;
        }
        return os;
    }

    SparseMatrix operator+(SparseMatrix const &a);

    SparseMatrix operator-(SparseMatrix const &a);

    SparseMatrix operator*(double const &a);

    SparseMatrix operator*(SparseMatrix const &a);

    // Multiplica los elementos de la fila i por el elemento i del vector recibido por parámetro (equivalente a multiplicar por matriz diagonal a izquierda)
    SparseMatrix multiply_rows(vector<double>& by);

    // Multiplica los elementos de la columna i por el elemento i del vector recibido por parámetro (equivalente a multiplicar por matriz diagonal a derecha)
    SparseMatrix multiply_columns(vector<double>& by);

    void Fi_minus_Fj_k(int fila1, int fila2, double multiplicador);

    double norm();

    SparseMatrix transpose();
};

#endif