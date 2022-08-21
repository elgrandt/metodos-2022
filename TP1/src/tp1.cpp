#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <chrono>
#include <set>

using namespace std;

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
        if (row > this->width || column > this->height || row < 0 || column < 0) {
            cerr << "Index fuera de rango " << row << "," << column << endl;
            throw "Index fuera de rango";
        }
        for (Cell cell: cells) {
            if (cell.row == row && cell.column == column) {
                return cell.value;
            }
        }
        return 0;
    }

    void set(int row, int column, double value) {
        int index;
        for (auto it = this->cells.begin(); it != this->cells.end(); ++it) {
            if (it->row == row && it->column == column) {
                if (value == 0) {
                    this->cells.erase(it);
                } else {
                    it->value = value;
                }
                return;
            }
            index++;
        }
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
    }
};

//--------------------------------------------MAIN-------------------------------------------------------
int main(int argc, char** argv) {

    // Nos quedamos con los parametros de entrada
    string input_file;
    string output_file;
    double p = 1;
    if (argc >= 2) {
        input_file = argv[1];
        output_file = argv[2];
    } else {
        cerr<<"Ingreso incorrecto de parametros, uso correcto es /TP1 txtInput txtOutput" <<endl;
        return 0;
    }

    // Leemos el input
    fstream fin (input_file);
    int cant_paginas = 0;
    int cant_total_links = 0;
    fin >> cant_paginas >> cant_total_links;

    SparseMatrix W = SparseMatrix(cant_paginas, cant_paginas);
    for (int i = 0; i < cant_total_links; ++i) {
        int from, to;
        fin >> from;
        fin >> to;
        W.set(from - 1, to - 1, 1);
    }
    fin.close();

    cout << W;
    cout << "<<<<<<<<<<<<<<<<<<<" << endl;
    W.set(0, 2, 0.4);
    W.set(0, 3, 0);
    cout << W;


    // Ejecutamos el algoritmo
    vector<double> rating;
    
    
    ofstream fout (output_file);
    fout << p << "\n";
    for (double current_rating: rating){
        fout << current_rating << "\n";
    }
    fout.close();

    return 0;
}