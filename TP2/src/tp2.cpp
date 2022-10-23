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
    return fabs(fabs((previous.transpose() * current).at(0, 0)) - 1) < tolerancia;
}

Resultado metodo_potencia(SparseMatrix& matriz, int iteraciones, double tolerancia) {
    // Generamos x0
    SparseMatrix x0(1, matriz.getWidth());
    srand(446);
    for (int i = 0; i < x0.getHeight(); i++) {
        x0.blind_set(i, 0, 1 + (rand() % 9)); // Lleno x0 de numeros random entre 1 y 10
    }
    x0 = x0 * (1.0 / x0.norm());
    // Iteramos
    SparseMatrix previous = SparseMatrix(0, 0);
    for (int iter = 0; iter < iteraciones; iter++) {
        previous = SparseMatrix(x0);
        x0 = matriz * x0;
        x0 = x0 * (1.0 / x0.norm());
        // Comparo x0 con previous
        if (converged(x0, previous, tolerancia)) {
            ofstream fout ("iteraciones.out", ios_base::app);
            fout << iter << "\n";
            fout.close();
            cout << "Convergio en la iteracion " << iter << endl;
            break;
        }
    }
    // Si no convergio probamos de nuevo
    if (!converged(x0, previous, tolerancia)) {
        throw runtime_error("No converge en la cantidad de iteraciones requeridas.");
    }
    // Calculamos el autovalor
    double autovalor = ((x0.transpose() * matriz * x0).at(0, 0) / (x0.transpose() * x0).at(0, 0));
    // Generamos el resultado
    return Resultado(autovalor, x0);
}

int main(int argc, char** argv) {
    auto start = chrono::steady_clock::now();

    // Nos quedamos con los parametros de entrada
    string input_file;
    int cantidad_a_calcular;
    int numero_iteraciones = 1;
    double tolerancia = 0.1;
    if (argc != 5) {
        throw runtime_error("Ingreso incorrecto de parametros, uso correcto es ./tp2 archivo cantidad #iteraciones tolerancia");
    } else {
        input_file = argv[1];
        cantidad_a_calcular = stoi(argv[2]);
        numero_iteraciones = stoi(argv[3]);
        tolerancia = stod(argv[4]);
    }

    // Leemos el input
    fstream fin (input_file);
    if (fin.fail() || !fin.is_open()) {
        throw runtime_error("Error al abrir el archivo");
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
        throw runtime_error("Se leyo una matriz vacia");
    }
    SparseMatrix matriz(vector_matriz[0].size(), vector_matriz.size());
    for (int row = 0; row < vector_matriz.size(); row++) {
        for (int col = 0; col < vector_matriz[row].size(); col++) {
            matriz.blind_set(row, col, vector_matriz[row][col]);
        }
    }

    string input_file_name = input_file.substr(input_file.find_last_of("/\\") + 1);

    SparseMatrix matriz_actual(matriz);
    vector<Resultado> resultados;

    for (int i = 0; i < cantidad_a_calcular; i++) {

        Resultado resultado = metodo_potencia(matriz_actual, numero_iteraciones, tolerancia);
        resultados.push_back(resultado);
        matriz_actual = matriz_actual - ( resultado.autovector * resultado.autovector.transpose() ) * resultado.autovalor;
    }
    
    ofstream fout (input_file_name + ".autovalores.out");
    for (Resultado resultado: resultados) {
        fout << resultado.autovalor << "\n";
    }
    fout.close();
    
    ofstream fout2 (input_file_name + ".autovectores.out");
    for (int linea = 0; linea < resultados[0].autovector.getHeight(); linea++) {
        for (int resultado = 0; resultado < resultados.size(); resultado++) {
            fout2 << resultados[resultado].autovector.at(linea, 0);
            if (resultado != resultados.size() -1) {
                fout2 << " ";
            }
        }
        fout2 << "\n";
    }
    fout2.close();

    auto end = chrono::steady_clock::now();
    auto nanoseconds = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
    cout << "Execution time (nanoseconds): " << nanoseconds << endl;

    return 0;
}