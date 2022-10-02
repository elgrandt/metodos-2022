#include "matriz.h"
#include "utils.h"
#include <assert.h>
#include <cmath>

SparseMatrix::SparseMatrix(int width, int height): width(width), height(height) {}
    
double SparseMatrix::at(int row, int column) const {
    assert(row < this->height);
    assert(row >= 0);
    assert(column < this->width);
    assert(column >= 0);
    double value = 0;
    if (this->cells.count(Position(row, column)) != 0) {
        value = this->cells.at(Position(row, column));
    }
    return value;
}

void SparseMatrix::blind_set(int row, int column, double value) {
    if (almost_equals(value, 0)) return;
    this->cells[Position(row, column)] = value;
}

void SparseMatrix::set(int row, int column, double value) {
    if (almost_equals(value, 0)) {
        this->cells.erase(Position(row, column));
    } else {
        this->cells[Position(row, column)] = value;
    }
}

int SparseMatrix::getWidth() {
    return this->width;
}

int SparseMatrix::getHeight() {
    return this->height;
}

SparseMatrix SparseMatrix::operator+(SparseMatrix const &a) {
    assert(this->width == a.width);
    assert(this->height == a.height);
    SparseMatrix response = SparseMatrix(this->width, this->height);
    for (auto &cell: this->cells) {
        response.blind_set(cell.first.row, cell.first.column, cell.second);
    }
    for (auto &cell: a.cells) {
        response.set(cell.first.row, cell.first.column, response.at(cell.first.row, cell.first.column) + cell.second);
    }
    return response;
}

SparseMatrix SparseMatrix::operator-(SparseMatrix const &a) {
    assert(this->width == a.width);
    assert(this->height == a.height);
    SparseMatrix response = SparseMatrix(this->width, this->height);
    for (auto &cell: this->cells) {
        response.blind_set(cell.first.row, cell.first.column, cell.second);
    }
    for (auto &cell: a.cells) {
        response.set(cell.first.row, cell.first.column, response.at(cell.first.row, cell.first.column) - cell.second);
    }
    return response;
}

SparseMatrix SparseMatrix::operator*(double const &a) {
    SparseMatrix response = SparseMatrix(this->width, this->height);
    for (PositionValuePair cell: cells) {
        response.blind_set(cell.first.row, cell.first.column, cell.second * a);
    }
    return response;
}

SparseMatrix SparseMatrix::operator*(SparseMatrix const &a) {
    assert(this->width == a.height);
    SparseMatrix response = SparseMatrix(a.width, this->height);
    for (int row = 0; row < this->height; row++) {
        for (int column = 0; column < a.height; column++) {
            double producto_escalar = 0;
            for (int i = 0; i < this->width; i++) {
                producto_escalar += this->at(row, i) * a.at(i, column);
            }
            response.blind_set(row, column, producto_escalar);
        }
    }
    return response;
}

// Multiplica los elementos de la fila i por el elemento i del vector recibido por parámetro (equivalente a multiplicar por matriz diagonal a izquierda)
SparseMatrix SparseMatrix::multiply_rows(vector<double>& by) {
    assert(by.size() == this->height);
    SparseMatrix response = SparseMatrix(this->width, this->height);
    for (PositionValuePair cell: cells) {
        response.blind_set(cell.first.row, cell.first.column, cell.second * by[cell.first.row]);
    }
    return response;
}

// Multiplica los elementos de la columna i por el elemento i del vector recibido por parámetro (equivalente a multiplicar por matriz diagonal a derecha)
SparseMatrix SparseMatrix::multiply_columns(vector<double>& by) {
    assert(by.size() == this->width);
    SparseMatrix response = SparseMatrix(this->width, this->height);
    for (PositionValuePair cell: cells) {
        response.blind_set(cell.first.row, cell.first.column, cell.second * by[cell.first.column]);
    }
    return response;
}

void SparseMatrix::Fi_minus_Fj_k(int fila1, int fila2, double multiplicador) {
    // TODO Averiguar cómo se puede mejorar la eficiencia de esto
    for (int col = 0; col < this->width; col++) {
        if (this->cells.count(Position(fila2, col)) != 0) {
            this->set(fila1, col, this->at(fila1, col) - this->at(fila2, col) * multiplicador);
        }
    }
}

double SparseMatrix::norm() {
    assert(this->width == 1);
    double sum = 0;
    for (int i = 0; i < this->height; i++) {
        double value = this->at(i, 0);
        sum += value * value;
    }
    return sqrt(sum);
}

SparseMatrix SparseMatrix::transpose() {
    SparseMatrix result(this->height, this->width);
    for (PositionValuePair cell: cells) {
        result.cells[Position(cell.first.column, cell.first.row)] = cell.second;
    }
    return result;
}