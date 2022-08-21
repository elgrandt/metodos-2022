#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <chrono>

using namespace std;

struct Link {
    int from;
    int to;
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
    int cantPaginas = 0;
    int cantTotalLinks = 0;
    fin >> cantPaginas >> cantTotalLinks;

    vector<Link> links;
    for (int i = 0; i < cantTotalLinks; ++i) {
        Link current_link;
        fin >> current_link.from;
        fin >> current_link.to;
        links.push_back(current_link);
    }
    fin.close();


    // Ejecutamos el algoritmo
    vector<double> rating;
    
    
    ofstream fout (output_file);
    fout << p << "\n";
    for (double currentRating: rating){
        fout << currentRating << "\n";
    }
    fout.close();

    return 0;
}