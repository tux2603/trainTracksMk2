#include <cstdio>
#include <cstring>

#include "types.h"

#define DEFAULT_DIRECTION_ERROR "Train entered a cell from an invalid direction!"


Rotation operator*(const Rotation &a, const Rotation &b) {
    int sum = (int)a + (int)b;
    sum %= NUM_ROTATION_STATES;
    return (Rotation)sum;
}


Rotation &operator*=(Rotation &a, const Rotation &b) {
    int sum = (int)a + (int)b;
    sum %= NUM_ROTATION_STATES;
    a = (Rotation)sum;
    return a;
}

Rotation operator-(const Rotation &a) {
    int inverted = -(int) a + NUM_ROTATION_STATES;
    inverted %= NUM_ROTATION_STATES;
    return (Rotation)inverted;
}

Direction operator*(const Direction &a, const Rotation &b) {
    return a == Direction::INVALID ? Direction::INVALID : (Direction)((Rotation)a * b);
}

State operator!(const State &a) {
    return a == State::ZERO ? State::ONE : State::ZERO;
}



InvalidDirectionException::InvalidDirectionException() {
    errorMessage = new char[strlen(DEFAULT_DIRECTION_ERROR)];
    sprintf(errorMessage, "%s", DEFAULT_DIRECTION_ERROR);
}

InvalidDirectionException::~InvalidDirectionException() {
    delete[] errorMessage;
}

const char *InvalidDirectionException::what() const throw() {
    return errorMessage;
}