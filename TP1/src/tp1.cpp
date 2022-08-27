#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <string>

using namespace std;

bool almost_equals(double a, double b) {
    return fabs(a - b) < 10e-6;
}

struct Link {
    int from;
    int to;
};

struct Cell {
    int row;
    int column;
    double value;
};

class SparseMatrix {
private:
    vector<Cell> cells;
    int width;
    int height;

public:

    SparseMatrix(int width, int height) {
        this->width = width;
        this->height = height;
    }
    
    double at(int row, int column) const {
        assert(row < this->width);
        assert(row >= 0);
        assert(column < this->height);
        assert(column >= 0);
        for (Cell cell: cells) {
            if (cell.row == row && cell.column == column) {
                return cell.value;
            }
        }
        return 0;
    }

    void blind_set(int row, int column, double value) {
        if (almost_equals(value, 0)) return;
        Cell new_cell;
        new_cell.row = row;
        new_cell.column = column;
        new_cell.value = value;
        this->cells.push_back(new_cell);
    }

    void set(int row, int column, double value) {
        int index;
        for (auto it = this->cells.begin(); it != this->cells.end(); ++it) {
            if (it->row == row && it->column == column) {
                if (almost_equals(value, 0)) {
                    this->cells.erase(it);
                } else {
                    it->value = value;
                }
                return;
            }
            index++;
        }
        if (almost_equals(value, 0)) return;
        Cell new_cell;
        new_cell.row = row;
        new_cell.column = column;
        new_cell.value = value;
        this->cells.push_back(new_cell);
    }

    int getWidth() {
        return this->width;
    }

    int getHeight() {
        return this->height;
    }

    friend std::ostream &operator<<(std::ostream &os, SparseMatrix const &m) {
        for (int row = 0; row < m.height; row++) {
            for (int col = 0; col < m.width; col++) {
                os << m.at(row, col) << " ";
            }
            os << endl;
        }
        return os;
    }

    SparseMatrix operator+(SparseMatrix const &a) {
        assert(this->width == a.width);
        assert(this->height == a.height);
        SparseMatrix response = SparseMatrix(this->width, this->height);
        vector<Cell> modifyingCells(this->cells.begin(), this->cells.end());
        modifyingCells.insert(modifyingCells.end(), a.cells.begin(), a.cells.end());
        
        while (modifyingCells.size() > 0) {
            Cell current = modifyingCells.back();
            modifyingCells.pop_back();
            Cell other = Cell();
            other.value = 0;
            for (int i = 0; i < modifyingCells.size(); i++) {
                Cell cell = modifyingCells[i];
                if (cell.row == current.row && cell.column == current.column) {
                    other = cell;
                    modifyingCells.erase(modifyingCells.begin() + i);
                    break;
                }
            }
            response.blind_set(current.row, current.column, current.value + other.value);
        }
        return response;
    }

    SparseMatrix operator-(SparseMatrix const &a) {
        assert(this->width == a.width);
        assert(this->height == a.height);
        SparseMatrix response = SparseMatrix(this->width, this->height);
        vector<Cell> modifyingCells(this->cells.begin(), this->cells.end());
        for (int i = 0; i < a.cells.size(); i++) {
            Cell cell = Cell();
            cell.row = a.cells[i].row;
            cell.column = a.cells[i].column;
            cell.value = -a.cells[i].value;
            modifyingCells.push_back(cell);
        }
        
        while (modifyingCells.size() > 0) {
            Cell current = modifyingCells.back();
            modifyingCells.pop_back();
            Cell other = Cell();
            other.value = 0;
            for (int i = 0; i < modifyingCells.size(); i++) {
                Cell cell = modifyingCells[i];
                if (cell.row == current.row && cell.column == current.column) {
                    other = cell;
                    modifyingCells.erase(modifyingCells.begin() + i);
                    break;
                }
            }
            response.blind_set(current.row, current.column, current.value + other.value);
        }
        return response;
    }

    SparseMatrix operator*(double const &a) {
        SparseMatrix response = SparseMatrix(this->width, this->height);
        for (Cell cell: cells) {
            Cell newCell = Cell();
            newCell.row = cell.row;
            newCell.column = cell.column;
            newCell.value = cell.value * a;
            response.cells.push_back(newCell);
        }
        return response;
    }

    SparseMatrix operator*(SparseMatrix const &a) {
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
    SparseMatrix multiply_rows(vector<double>& by) {
        assert(by.size() == this->height);
        SparseMatrix response = SparseMatrix(this->width, this->height);
        for (Cell cell: cells) {
            response.blind_set(cell.row, cell.column, cell.value * by[cell.row]);
        }
        return response;
    }

    // Multiplica los elementos de la columna i por el elemento i del vector recibido por parámetro (equivalente a multiplicar por matriz diagonal a derecha)
    SparseMatrix multiply_columns(vector<double>& by) {
        assert(by.size() == this->width);
        SparseMatrix response = SparseMatrix(this->width, this->height);
        for (Cell cell: cells) {
            response.blind_set(cell.row, cell.column, cell.value * by[cell.column]);
        }
        return response;
    }
};

void FiMinusFjK(SparseMatrix &A, int fila1, int fila2, double multiplicador) {
    for (int col = 0; col < A.getWidth(); col++) {
        A.set(fila1, col, A.at(fila1, col) - A.at(fila2, col) * multiplicador);
    }
}

void eliminacion_gaussiana(SparseMatrix &A, vector<double> &b) {
    for (int col = 0; col < A.getWidth(); col++) {
        for (int fila = col + 1; fila < A.getHeight(); fila++) {
            // Fx - c * Fy
            double diagonal = A.at(col, col);
            assert(!almost_equals(diagonal, 0));
            double c = A.at(fila, col) / diagonal;
            int Fx = fila;
            int Fy = col;
            if (!almost_equals(c, 0)) {
                FiMinusFjK(A, Fx, Fy, c);
                b[Fx] -= b[Fy] * c;
            }
        }
    }
}

double resolver_ecuacion_fila_i(SparseMatrix &A, vector<double> &b, int fila, vector<double> &respuestas){
    double solucion = b[fila];
    for (int columna = A.getWidth() - 1; columna > fila; columna--) {
        solucion -= A.at(fila, columna) * respuestas[columna];
    }
    return solucion/A.at(fila, fila);
}

vector<double> resolucion_matriz_triangular_superior(SparseMatrix &A, vector<double> &b){
    vector<double> respuestas = vector<double>(A.getHeight());
    for (int fila = A.getHeight() - 1; fila >= 0; fila--){
        respuestas[fila] = resolver_ecuacion_fila_i(A, b, fila, respuestas);
    }
    return respuestas;
}

void mostrar_vector(vector<double> &v) {
    cout << "[";
    for (int i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i != v.size() - 1)
            cout << ", ";
    }
    cout << "]" << endl;
}

void normalizar_vector(vector<double> &v) {
    double sum = 0;
    for (int i = 0; i < v.size(); i++) {
        sum += v[i];
    }
    for (int i = 0; i < v.size(); i++) {
        v[i] = v[i]/sum;
    }
}

//--------------------------------------------MAIN-------------------------------------------------------
int main(int argc, char** argv) {

    // Nos quedamos con los parametros de entrada
    string input_file;
    double p = 1;
    if (argc >= 2) {
        input_file = argv[1];
        p = stod(argv[2]);
    } else {
        cerr<<"Ingreso incorrecto de parametros, uso correcto es /TP1 txtInput p" <<endl;
        return 0;
    }

    // Leemos el input
    fstream fin (input_file);
    int cant_paginas = 0;
    int cant_total_links = 0;
    fin >> cant_paginas >> cant_total_links;

    vector<double> d(cant_paginas, 0);
    SparseMatrix pW = SparseMatrix(cant_paginas, cant_paginas);
    for (int i = 0; i < cant_total_links; ++i) {
        int from, to;
        fin >> from;
        fin >> to;
        pW.blind_set(to - 1, from - 1, p);
        d[from-1]++;
    }
    fin.close();
    // Hacemos Di = 1/Di
    for (int i = 0; i < cant_paginas; i++) {
        if (d[i] != 0)
            d[i] = 1.0/d[i];
    }
    
    SparseMatrix pWD = pW.multiply_columns(d);

    SparseMatrix identity(cant_paginas, cant_paginas);
    for (int i = 0; i < cant_paginas; i++) {
        identity.blind_set(i, i, 1);
    }

    SparseMatrix IpWD = identity - pWD;

    // Resolver sistemas de IpWD = (1, 1, ..., 1);
    cout << "W:" << endl;
    cout << pW;
    cout << "I - pWD:" << endl;
    cout << IpWD;
    vector<double> b(cant_paginas, 1);
    // Eliminación gaussiana
    eliminacion_gaussiana(IpWD, b);
    cout << "Matriz resultante de eliminacion gaussiana:" << endl;
    cout << IpWD;
    cout << "b resultante de eliminacion gaussiana: ";
    mostrar_vector(b);
    // Resolvemos la matriz triangular resultante de la eliminación gaussiana
    vector<double> respuestas = resolucion_matriz_triangular_superior(IpWD, b);
    cout << "Respuestas: ";
    mostrar_vector(respuestas);
    // Normalizamos el vector respuestas
    normalizar_vector(respuestas);
    cout << "Respuestas normalizadas: ";
    mostrar_vector(respuestas);
    
    
    ofstream fout (input_file + ".out");
    fout << p << "\n";
    for (double current_rating: respuestas){
        fout << current_rating << "\n";
    }
    fout.close();

    return 0;
}