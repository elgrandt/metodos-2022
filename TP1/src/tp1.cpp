#include <iostream>
#include <fstream>
#include <vector>
#include <assert.h>
#include <cmath>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <set>
#include <chrono>
using namespace std;

bool almost_equals(double a, double b) {
    return fabs(a - b) < 10e-4;
}

struct Position {
    int row;
    int column;

    Position() {}
    Position(int row, int column): row(row), column(column) {}
    Position(const Position& other): row(other.row), column(other.column) {}

    bool operator==(const Position& other) const {
        return this->row == other.row && this->column == other.column;
    }

    void operator=(const Position& other) {
        this->row = other.row;
        this->column = other.column;
    }

    bool operator<(const Position& other) const {
        if (this->row < other.row) {
            return true;
        } else if (this->row == other.row && this->column < other.column) {
            return true;
        } else {
            return false;
        }
    }

    struct HashFunction {
        size_t operator()(const Position& position) const {
            size_t rowHash = std::hash<int>()(position.row);
            size_t colHash = std::hash<int>()(position.column) << 1;
            return rowHash ^ colHash;
        }
    };
};

typedef pair<const Position, double> PositionValuePair;

class SparseMatrix {
private:
    unordered_map<Position, double, Position::HashFunction> cells;
    int width;
    int height;
public:

    SparseMatrix(int width, int height): width(width), height(height) {}
    
    double at(int row, int column) const {
        assert(row < this->width);
        assert(row >= 0);
        assert(column < this->height);
        assert(column >= 0);
        double value = 0;
        if (this->cells.count(Position(row, column)) != 0) {
            value = this->cells.at(Position(row, column));
        }
        // Verificado que es horriblemente ineficiente esta implementación
        // try {
        //     value = this->cells.at(Position(row, column));
        // } catch(out_of_range e) {}
        return value;
    }

    void blind_set(int row, int column, double value) {
        if (almost_equals(value, 0)) return;
        this->cells[Position(row, column)] = value;
    }

    void set(int row, int column, double value) {
        if (almost_equals(value, 0)) {
            this->cells.erase(Position(row, column));
        } else {
            this->cells[Position(row, column)] = value;
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
                os << right << setw(10) << m.at(row, col) << " ";
            }
            os << endl;
        }
        return os;
    }

    SparseMatrix operator+(SparseMatrix const &a) {
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

    SparseMatrix operator-(SparseMatrix const &a) {
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

    SparseMatrix operator*(double const &a) {
        SparseMatrix response = SparseMatrix(this->width, this->height);
        for (PositionValuePair cell: cells) {
            response.blind_set(cell.first.row, cell.first.column, cell.second * a);
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
        for (PositionValuePair cell: cells) {
            response.blind_set(cell.first.row, cell.first.column, cell.second * by[cell.first.row]);
        }
        return response;
    }

    // Multiplica los elementos de la columna i por el elemento i del vector recibido por parámetro (equivalente a multiplicar por matriz diagonal a derecha)
    SparseMatrix multiply_columns(vector<double>& by) {
        assert(by.size() == this->width);
        SparseMatrix response = SparseMatrix(this->width, this->height);
        for (PositionValuePair cell: cells) {
            response.blind_set(cell.first.row, cell.first.column, cell.second * by[cell.first.column]);
        }
        return response;
    }

    void Fi_minus_Fj_k(int fila1, int fila2, double multiplicador) {
        // TODO Averiguar cómo se puede mejorar la eficiencia de esto
        for (int col = 0; col < this->width; col++) {
            if (this->cells.count(Position(fila2, col)) != 0) {
                this->set(fila1, col, this->at(fila1, col) - this->at(fila2, col) * multiplicador);
            }
        }
    }
};

// Deprecada por ineficiente :(
// void Fi_minus_Fj_k(SparseMatrix &A, int fila1, int fila2, double multiplicador) {
//     for (int col = 0; col < A.getWidth(); col++) {
//         A.set(fila1, col, A.at(fila1, col) - A.at(fila2, col) * multiplicador);
//     }
// }

void eliminacion_gaussiana(SparseMatrix &A, vector<double> &b) {
    for (int col = 0; col < A.getWidth(); col++) {
        if (col % 100 == 0) {
            // Dejado para indicar progreso del algoritmo.
            cout << "Eliminacion gaussiana, columna " << col << endl;
        }
        double diagonal = A.at(col, col);
        assert(!almost_equals(diagonal, 0));
        for (int fila = col + 1; fila < A.getHeight(); fila++) {
            // Fx - c * Fy
            double c = A.at(fila, col) / diagonal;
            int Fx = fila;
            int Fy = col;
            if (!almost_equals(c, 0)) {
                A.Fi_minus_Fj_k(Fx, Fy, c);
                // Fi_minus_Fj_k(A, Fx, Fy, c);
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
    auto start = chrono::steady_clock::now();

    // Nos quedamos con los parametros de entrada
    string input_file;
    double p = 1;
    if (argc < 3) {
        cerr<<"Ingreso incorrecto de parametros, uso correcto es ./tp1 txtInput p" <<endl;
        return 0;
    } else {
        input_file = argv[1];
        p = stod(argv[2]);
    }

    // Leemos el input
    fstream fin (input_file);
    if (fin.fail()) {
        cout << "Error al abrir el archivo: " << input_file << endl;
        return -1;
    }

    int cant_paginas = 0;
    int cant_total_links = 0;
    fin >> cant_paginas >> cant_total_links;
    cout << input_file << " " << p << endl;

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

    vector<double> b(cant_paginas, 1);
    // Eliminación gaussiana
    cout << "Iniciando eliminacion gaussiana" << endl;
    eliminacion_gaussiana(IpWD, b);
    // Resolvemos la matriz triangular resultante de la eliminación gaussiana
    cout << "Iniciando resolucion sistema triangular" << endl;
    vector<double> respuestas = resolucion_matriz_triangular_superior(IpWD, b);
    // Normalizamos el vector respuestas
    normalizar_vector(respuestas);
    
    string input_file_name = input_file.substr(input_file.find_last_of("/\\") + 1);
    ofstream fout (input_file_name + ".out");
    fout << p << "\n";
    for (double current_rating: respuestas){
        fout << current_rating << "\n";
    }
    fout.close();

    auto end = chrono::steady_clock::now();
    auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    cout << "Execution time (nanoseconds): " << nanoseconds << endl;

    return 0;
}