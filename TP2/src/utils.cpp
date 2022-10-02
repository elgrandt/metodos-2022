#include "utils.h"
#include <cmath>

bool almost_equals(double a, double b) {
    return fabs(a - b) < 10e-4;
}

Position::Position() {}
Position::Position(int row, int column): row(row), column(column) {}
Position::Position(const Position& other): row(other.row), column(other.column) {}

bool Position::operator==(const Position& other) const {
    return this->row == other.row && this->column == other.column;
}

void Position::operator=(const Position& other) {
    this->row = other.row;
    this->column = other.column;
}

bool Position::operator<(const Position& other) const {
    if (this->row < other.row) {
        return true;
    } else if (this->row == other.row && this->column < other.column) {
        return true;
    } else {
        return false;
    }
}

size_t Position::HashFunction::operator()(const Position& position) const {
    size_t rowHash = std::hash<int>()(position.row);
    size_t colHash = std::hash<int>()(position.column) << 1;
    return rowHash ^ colHash;
}