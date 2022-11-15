#include <iostream>
#include <Eigen/Sparse>
#include <vector>
#include <chrono>
#include <fstream>
#include <assert.h>
using namespace std;
using namespace Eigen;

typedef SparseMatrix<double> SpMat;

/********** Eliminación Gaussiana ***********/

bool almost_equals(double a, double b) {
    return fabs(a - b) < 10e-4;
}

void Fi_minus_Fj_k(SpMat& matriz, int fila1, int fila2, double multiplicador) {
    // Fi = Fi - k * Fj
    for (int col = 0; col < matriz.cols(); col++) {
        if (!almost_equals(matriz.coeff(fila2, col), 0)) {
            matriz.coeffRef(fila1, col) = matriz.coeff(fila1, col) - matriz.coeff(fila2, col) * multiplicador;
        }
    }
}

void eliminacion_gaussiana(SpMat &A, VectorXd &b) {
    for (int col = 0; col < A.cols(); col++) {
        if (col % 100 == 0) {
            // Dejado para indicar progreso del algoritmo.
            cout << "Eliminacion gaussiana, columna " << col << endl;
        }
        double diagonal = A.coeff(col, col);
        assert(!almost_equals(diagonal, 0));
        for (int fila = col + 1; fila < A.rows(); fila++) {
            // Fx - c * Fy
            double c = A.coeff(fila, col) / diagonal;
            int Fx = fila;
            int Fy = col;
            if (!almost_equals(c, 0)) {
                Fi_minus_Fj_k(A, Fx, Fy, c);
                b(Fx) -= b(Fy) * c;
            }
        }
    }
}

double resolver_ecuacion_fila_i(SpMat &A, VectorXd &b, int fila, VectorXd &respuestas){
    double solucion = b(fila);
    for (int columna = A.cols() - 1; columna > fila; columna--) {
        solucion -= A.coeff(fila, columna) * respuestas(columna);
    }
    return solucion/A.coeff(fila, fila);
}

VectorXd resolucion_matriz_triangular_superior(SpMat &A, VectorXd &b){
    VectorXd respuestas = VectorXd(A.rows());
    for (int fila = A.rows() - 1; fila >= 0; fila--){
        respuestas(fila) = resolver_ecuacion_fila_i(A, b, fila, respuestas);
    }
    return respuestas;
}

VectorXd resolver_con_eliminacion_gaussiana(SpMat& matriz, VectorXd& b) {
    // Eliminación gaussiana
    cout << "Iniciando eliminacion gaussiana" << endl;
    eliminacion_gaussiana(matriz, b);
    // Resolvemos la matriz triangular resultante de la eliminación gaussiana
    cout << "Iniciando resolucion sistema triangular" << endl;
    VectorXd respuestas = resolucion_matriz_triangular_superior(matriz, b);
    // Normalizamos el vector respuestas
    respuestas /= respuestas.sum();

    return respuestas;
}

/********** Algoritmos iterativos ***********/

SpMat getInverse(const SpMat& matriz) {
    SparseLU<SpMat> solver;
    solver.compute(matriz);
    SpMat I(matriz.rows(), matriz.cols());
    I.setIdentity();
    return solver.solve(I);
}

bool checkear_convergencia(VectorXd& current, VectorXd& previous, double tolerancia) {
    return fabs(fabs(current.normalized().dot(previous.normalized())) - 1) < tolerancia;
}

VectorXd resolver_algoritmo_iterativo(SpMat& T, VectorXd& c) {
    int max_iteraciones = 1000000;
    double tolerancia = 1e-10;

    VectorXd x = VectorXd::Random(T.rows()); // X inicial random

    bool convergio = false;
    for (int iteracion = 0; iteracion < max_iteraciones; iteracion++) {
        VectorXd prevX(x);
        x = (T * x) + c;
        // Impresión de iteraciones solo para el experimento de convergencia
        // ofstream fout ("iteraciones." + to_string(iteracion) + ".out", ios_base::app);
        // for (int i = 0; i < x.size(); i++) {
        //     fout << x(i) / x.sum() << "\n";
        // }
        // fout.close();
        if (checkear_convergencia(x, prevX, tolerancia)) {
            convergio = true;
            break;
        }
    }

    assert(convergio);

    // Normalizamos el vector respuestas
    x /= x.sum();

    return x;
}

VectorXd resolver_con_jacobi(SpMat& matriz, VectorXd& b) {
    SpMat D = SpMat(matriz.diagonal().asDiagonal());
    SpMat L = -(matriz.triangularView<Lower>() - D);
    SpMat U = -(matriz.triangularView<Upper>() - D);
    SpMat Dinv = getInverse(D);
    SpMat T = Dinv * (L + U);
    VectorXd c = Dinv * b;
    return resolver_algoritmo_iterativo(T, c);
}

VectorXd resolver_gauss_seidel(SpMat& matriz, VectorXd& b) {
    SpMat D = SpMat(matriz.diagonal().asDiagonal());
    SpMat L = -(matriz.triangularView<Lower>() - D);
    SpMat U = -(matriz.triangularView<Upper>() - D);
    SpMat DLinv = getInverse(D-L);
    SpMat T = DLinv * U;
    VectorXd c = DLinv * b;
    return resolver_algoritmo_iterativo(T, c);
}

/********** Ejecución principal ***********/
 
int main(int argc, char** argv) {
    auto start = chrono::steady_clock::now();

    // Nos quedamos con los parametros de entrada
    string input_file;
    string algorithm;
    double p = 1;
    if (argc != 4) {
        cerr << "Ingreso incorrecto de parametros, uso correcto es ./tp3 txtInput p algorithm\nAlgoritmos disponibles: eg (Eliminacion Gaussiana), jb (Jacobi), gs (Gauss-Seidel)." << endl;
        return -1;
    } else {
        input_file = argv[1];
        p = stod(argv[2]);
        algorithm = argv[3];
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

    SpMat D(cant_paginas, cant_paginas);
    SpMat pW = SpMat(cant_paginas, cant_paginas);
    for (int i = 0; i < cant_total_links; ++i) {
        int from, to;
        fin >> from;
        fin >> to;
        pW.insert(to - 1, from - 1) = p;
        D.coeffRef(from-1, from-1)++;
    }
    fin.close();
    // Hacemos Di = 1/Di
    for (int i = 0; i < cant_paginas; i++) {
        if (D.coeff(i, i) != 0)
            D.coeffRef(i, i) = 1.0/D.coeff(i, i);
    }
    
    SpMat pWD = pW * D;

    SpMat identity(cant_paginas, cant_paginas);
    for (int i = 0; i < cant_paginas; i++) {
        identity.insert(i, i) = 1;
    }

    SpMat IpWD = identity - pWD;
    VectorXd b = VectorXd::Ones(cant_paginas);

    VectorXd respuestas;
    if (algorithm == "eg") {
        respuestas = resolver_con_eliminacion_gaussiana(IpWD, b);
    } else if (algorithm == "jb") {
        respuestas = resolver_con_jacobi(IpWD, b);
    } else if (algorithm == "gs") {
        respuestas = resolver_gauss_seidel(IpWD, b);
    } else {
        cerr << "El algoritmo ingresado es incorrecto.\nAlgoritmos disponibles: eg (Eliminación Gaussiana), jb, (Jacobi) gs (Gauss-Seidel)." << endl;
        return -1;
    }
    
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
