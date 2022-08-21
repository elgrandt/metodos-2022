#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "metodos.h"
#include <chrono>

using namespace std;

//--------------------------------------------MAIN-------------------------------------------------------
int main(int argc, char** argv){

    // Nos quedamos con los parametros de entrada
    string input_file;
    double p;
    if (argc >= 2) {
        input_file = argv[1];
        p = argv[2];
    }else {
        cerr<<"Ingreso incorrecto de parametros, uso correcto es /TP1 txtInput txtOutput" <<endl;
        return 0;
    }

    // Leemos el input
    fstream fin (input_file);
    int cantPaginas = 0;
    int cantTotalLinks = 0;
    fin >> cantPaginas >> cantTotalLinks;

    vector<Link> links;
    for (int i = 0; i < cantTotalLinks; ++i) {
        Link unLink;
        fin >> unLink.paginaSaliente;
        fin >> unLink.paginaEntrante;
        links.push_back(unLink);
    }
    fin.close();


    // Ejecutamos el algoritmo
    
    
    ofstream fout (p);
    for (double rating: results){
        fout << rating << "\n";
    }
    fout.close();

    return 0;
}