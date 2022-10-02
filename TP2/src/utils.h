#ifndef UTILS_H
#define UTILS_H
#include <iostream>

bool almost_equals(double a, double b);

struct Position {
    int row;
    int column;

    Position();
    Position(int row, int column);
    Position(const Position& other);

    bool operator==(const Position& other) const;

    void operator=(const Position& other);

    bool operator<(const Position& other) const;

    struct HashFunction {
        size_t operator()(const Position& position) const;
    };
};

#endif