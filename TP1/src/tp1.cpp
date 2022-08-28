#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <string>
#include <unordered_map>

using namespace std;

bool almost_equals(double a, double b) {
    return fabs(a - b) < 10e-6;
}

typedef pair<int, int> Key;

struct KeyHash {
    hash<string> hasher;
    size_t operator()(const Key& k) const {
        return hasher(to_string(k.first) + "," + to_string(k.second));
    }
};

struct Link {
    int from;
    int to;
};

// struct Cell {
//     int row;
//     int column;
//     double value;
// };

class SparseMatrix {
private:
    unordered_map<Key, double, KeyHash> cells;
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
        try {
            this->cells.at(Key(row, column));
        } catch(out_of_range e) {
            return 0;
        }
    }

    void blind_set(int row, int column, double value) {
        if (almost_equals(value, 0)) return;
        this->cells[Key(row, column)] = value;
    }

    void set(int row, int column, double value) {
        if (almost_equals(value, 0)) {
            this->cells.erase(Key(row, column));
        } else {
            this->blind_set(row, column, value);
        }
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

    // SparseMatrix operator+(SparseMatrix const &a) {
    //     assert(this->width == a.width);
    //     assert(this->height == a.height);
    //     SparseMatrix response = SparseMatrix(this->width, this->height);
    //     vector<Cell> modifyingCells(this->cells.begin(), this->cells.end());
    //     modifyingCells.insert(modifyingCells.end(), a.cells.begin(), a.cells.end());
        
    //     while (modifyingCells.size() > 0) {
    //         Cell current = modifyingCells.back();
    //         modifyingCells.pop_back();
    //         Cell other = Cell();
    //         other.value = 0;
    //         for (int i = 0; i < modifyingCells.size(); i++) {
    //             Cell cell = modifyingCells[i];
    //             if (cell.row == current.row && cell.column == current.column) {
    //                 other = cell;
    //                 modifyingCells.erase(modifyingCells.begin() + i);
    //                 break;
    //             }
    //         }
    //         response.blind_set(current.row, current.column, current.value + other.value);
    //     }
    //     return response;
    // }

    SparseMatrix operator-(SparseMatrix const &a) {
        assert(this->width == a.width);
        assert(this->height == a.height);
        SparseMatrix response = SparseMatrix(this->width, this->height);
        vector<pair<Key, double>> modifyingCells = vector<pair<Key, double>>();
        for (const pair<const Key, double> c: this->cells) {
            modifyingCells.push_back(pair<Key, double>(c));
        }
        for (const pair<const Key, double> c: a.cells) {
            modifyingCells.push_back(pair<Key, double>(c));
        }
        
        while (modifyingCells.size() > 0) {
            pair<Key, double> current = modifyingCells.back();
            modifyingCells.pop_back();
            double value = 0;
            for (int i = 0; i < modifyingCells.size(); i++) {
                pair<Key, double> key = modifyingCells[i];
                if (key.first.first == current.first.first && key.first.second == current.first.second) {
                    value = key.second;
                    modifyingCells.erase(modifyingCells.begin() + i);
                    break;
                }
            }
            response.blind_set(current.first.first, current.first.second, current.second + value);
        }
        return response;
    }

    SparseMatrix operator*(double const &a) {
        SparseMatrix response = SparseMatrix(this->width, this->height);
        for (pair<const Key, double> cell: cells) {
            response.blind_set(cell.first.first, cell.first.second, cell.second * a);
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
        for (pair<const Key, double> cell: cells) {
            response.blind_set(cell.first.first, cell.first.second, cell.second * by[cell.first.first]);
        }
        return response;
    }

    // Multiplica los elementos de la columna i por el elemento i del vector recibido por parámetro (equivalente a multiplicar por matriz diagonal a derecha)
    SparseMatrix multiply_columns(vector<double>& by) {
        assert(by.size() == this->width);
        SparseMatrix response = SparseMatrix(this->width, this->height);
        for (pair<const Key, double> cell: cells) {
            response.blind_set(cell.first.first, cell.first.second, cell.second * by[cell.first.first]);
        }
        return response;
    }

    // void FiMinusFjK(int fila1, int fila2, double multiplicador) {
    //     for (int i = 0; i < this->cells.size(); i++) {
    //         if (this->cells[i].row == fila1) {
    //             this->cells[i].value -= multiplicador * this->at(fila2, this->cells[i].column);
    //         }
    //     }
    // }
};

void FiMinusFjK(SparseMatrix &A, int fila1, int fila2, double multiplicador) {
    for (int col = 0; col < A.getWidth(); col++) {
        A.set(fila1, col, A.at(fila1, col) - A.at(fila2, col) * multiplicador);
    }
}

void eliminacion_gaussiana(SparseMatrix &A, vector<double> &b) {
    for (int col = 0; col < A.getWidth(); col++) {
        cout << "Eliminacion gaussiana, columna " << col << endl;
        for (int fila = col + 1; fila < A.getHeight(); fila++) {
            // Fx - c * Fy
            double diagonal = A.at(col, col);
            assert(!almost_equals(diagonal, 0));
            double c = A.at(fila, col) / diagonal;
            int Fx = fila;
            int Fy = col;
            if (!almost_equals(c, 0)) {
                // A.FiMinusFjK(Fx, Fy, c);
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
    cout << "Iniciando eliminacion gaussiana" << endl;
    eliminacion_gaussiana(IpWD, b);
    cout << "Matriz resultante de eliminacion gaussiana:" << endl;
    cout << IpWD;
    cout << "b resultante de eliminacion gaussiana: ";
    mostrar_vector(b);
    // Resolvemos la matriz triangular resultante de la eliminación gaussiana
    cout << "Iniciando resolucion sistema triangular" << endl;
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