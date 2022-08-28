#include <iostream>
#include <unordered_map>
using namespace std;

struct Position {
    int row;
    int column;

    Position() {}
    Position(int row, int column): row(row), column(column) {}
    Position(const Position& other): row(other.row), column(other.column) {}

    bool operator==(const Position& other) const {
        return this->row == other.row && this->column == other.column;
    }

    struct HashFunction {
        size_t operator()(const Position& position) const {
            size_t rowHash = std::hash<int>()(position.row);
            size_t colHash = std::hash<int>()(position.column) << 1;
            return rowHash ^ colHash;
        }
    };
};

int main() {
    unordered_map<Position, double, Position::HashFunction> values;
    values[Position(0, 0)] = 5.2;
    try {
        cout << values.at(Position(0, 1)) << endl;
    } catch(out_of_range e) {
        cout << "Valor intexistente" << endl;
    }
    
    for (auto &current:values) {
        cout << "(" << current.first.row << ", " << current.first.column << "): " << current.second << endl;
    }
    return 0;
}