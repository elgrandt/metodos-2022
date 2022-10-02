#include <iostream>
#include <fstream>
#include <assert.h>
#include <chrono>
#include <sstream>
#include <cmath>
#include "matriz.h"
using namespace std;

struct Resultado {
    double autovalor;
    SparseMatrix autovector;

    Resultado(double autovalor, SparseMatrix& autovector): autovalor(autovalor), autovector(autovector) {};
};

bool converged(SparseMatrix& current, SparseMatrix& previous, double tolerancia) {
    for (int i = 0; i < current.getHeight(); i++) {
        if (fabs(current.at(i, 0) - previous.at(i, 0)) > tolerancia)
            return false;
    }
    return true;
}

Resultado metodo_potencia(SparseMatrix& matriz, int iteraciones, double tolerancia) {
    // Generamos x0
    SparseMatrix x0(1, matriz.getWidth());
    for (int i = 0; i < x0.getHeight(); i++) {
        x0.blind_set(i, 0, 1 + (rand() % 9)); // Lleno x0 de numeros random entre 1 y 10
    }
    // Iteramos
    SparseMatrix previous = SparseMatrix(0, 0);
    for (int iter = 0; iter < iteraciones; iter++) {
        previous = SparseMatrix(x0);
        x0 = matriz * x0;
        x0 = x0 * (1.0 / x0.norm());
        // Comparo x0 con previous
        if (converged(x0, previous, tolerancia))
            break;
    }
    // Si no convergio probamos de nuevo
    if (!converged(x0, previous, tolerancia)) {
        return metodo_potencia(matriz, iteraciones, tolerancia);
    }
    // Calculamos el autovalor
    double autovalor = (x0.transpose() * matriz * x0).at(0, 0);
    // Generamos el resultado
    return Resultado(autovalor, x0);
}

int main(int argc, char** argv) {
    auto start = chrono::steady_clock::now();

    // Nos quedamos con los parametros de entrada
    string input_file;
    int numero_iteraciones = 1;
    double tolerancia = 0.1;
    if (argc != 4) {
        cerr << "Ingreso incorrecto de parametros, uso correcto es ./tp2 archivo #iteraciones tolerancia" << endl;
        return -1;
    } else {
        input_file = argv[1];
        numero_iteraciones = stoi(argv[2]);
        tolerancia = stod(argv[3]);
    }

    // Leemos el input
    fstream fin (input_file);
    if (fin.fail() || !fin.is_open()) {
        cerr << "Error al abrir el archivo: " << input_file << endl;
        return -1;
    }

    vector<vector<double>> vector_matriz = vector<vector<double>>();
    
    while (!fin.eof()) {
        string line_string;
        getline(fin, line_string);
        istringstream iss(line_string);

        vector<double> current_line = vector<double>();
        string current;
        while (iss >> current) {    
            current_line.push_back(stod(current));
        }
        if (current_line.size()) {
            vector_matriz.push_back(current_line);
        }
    }
    
    if (vector_matriz.size() == 0 || vector_matriz[0].size() == 0) {
        cerr << "Se leyo una matriz vacia" << endl;
        return -1;
    }
    SparseMatrix matriz(vector_matriz[0].size(), vector_matriz.size());
    for (int col = 0; col < vector_matriz.size(); col++) {
        for (int row = 0; row < vector_matriz[col].size(); row++) {
            matriz.blind_set(row, col, vector_matriz[col][row]);
        }
    }

    int seed = 1001;
    srand(seed);

    Resultado primer_resultado = metodo_potencia(matriz, numero_iteraciones, tolerancia);
    cout << primer_resultado.autovalor << endl;
    cout << primer_resultado.autovector;

    // int cant_paginas = 0;
    // int cant_total_links = 0;
    // fin >> cant_paginas >> cant_total_links;
    // cout << input_file << " " << p << endl;

    // vector<double> d(cant_paginas, 0);
    // SparseMatrix pW = SparseMatrix(cant_paginas, cant_paginas);
    // for (int i = 0; i < cant_total_links; ++i) {
    //     int from, to;
    //     fin >> from;
    //     fin >> to;
    //     pW.blind_set(to - 1, from - 1, p);
    //     d[from-1]++;
    // }
    // fin.close();
    // // Hacemos Di = 1/Di
    // for (int i = 0; i < cant_paginas; i++) {
    //     if (d[i] != 0)
    //         d[i] = 1.0/d[i];
    // }
    
    // SparseMatrix pWD = pW.multiply_columns(d);

    // SparseMatrix identity(cant_paginas, cant_paginas);
    // for (int i = 0; i < cant_paginas; i++) {
    //     identity.blind_set(i, i, 1);
    // }
    // cout << "Iniciando eliminacion gaussiana" << endl;
    // eliminacion_gaussiana(IpWD, b);
    // // Resolvemos la matriz triangular resultante de la eliminación gaussiana
    // cout << "Iniciando resolucion sistema triangular" << endl;
    // vector<double> respuestas = resolucion_matriz_triangular_superior(IpWD, b);
    // // Normalizamos el vector respuestas
    // normalizar_vector(respuestas);
    
    // string input_file_name = input_file.substr(input_file.find_last_of("/\\") + 1);
    // ofstream fout (input_file_name + ".out");
    // fout << p << "\n";
    // for (double current_rating: respuestas){
    //     fout << current_rating << "\n";
    // }
    // fout.close();

    // SparseMatrix IpWD = identity - pWD;

    // vector<double> b(cant_paginas, 1);
    // // Eliminación gaussiana
    // cout << "Iniciando eliminacion gaussiana" << endl;
    // eliminacion_gaussiana(IpWD, b);
    // // Resolvemos la matriz triangular resultante de la eliminación gaussiana
    // cout << "Iniciando resolucion sistema triangular" << endl;
    // vector<double> respuestas = resolucion_matriz_triangular_superior(IpWD, b);
    // // Normalizamos el vector respuestas
    // normalizar_vector(respuestas);
    
    // string input_file_name = input_file.substr(input_file.find_last_of("/\\") + 1);
    // ofstream fout (input_file_name + ".out");
    // fout << p << "\n";
    // for (double current_rating: respuestas){
    //     fout << current_rating << "\n";
    // }
    // fout.close();

    auto end = chrono::steady_clock::now();
    auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    cout << "Execution time (nanoseconds): " << nanoseconds << endl;

    return 0;
}