#include <iostream>
#include <unordered_map>
using namespace std;

struct Cell {
    int row;
    int column;
    double value;
};

typedef const tuple<int, int> coordinates;

struct key_hash {
    hash<string> hasher;
    size_t operator()(const string& k) const {
        return hasher(k);
    }
};

int main() {
    unordered_map<string, double, key_hash> t;
    t["hola"] = 5.2;
    
    // for (pair<const string, CellV2> c:t) {
    //     cout << c.first << endl;
    // }
    return 0;
}