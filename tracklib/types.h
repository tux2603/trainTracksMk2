#ifndef __TYPES_GUARD_
#define __TYPES_GUARD_

#include <exception>

#define NUM_DIRECTIONS 4
#define NUM_CELL_TYPES 4
#define NUM_CELL_VARIANTS 4
#define NUM_ROTATION_STATES 4
#define NUM_CELL_STATES 2

enum class Direction {
    INVALID = -1,
    NORTH   = 0,
    EAST    = 1,
    SOUTH   = 2,
    WEST    = 3
};

enum class CellType {
    NONE             = -1,
    TRACK            = 0,
    SPRUNG_JUNC      = 1,
    LAZY_JUNC        = 2,
    ALTERNATING_JUNC = 3
};

enum class Variant {
    DEFAULT      = 0,
    STRAIGHT     = 0,
    CURVE        = 1,
    CROSS        = 2,
    DOUBLE_CURVE = 3,
    LEFT_HAND    = 0,
    RIGHT_HAND   = 1,
    SYMMETRIC    = 2
};

enum class Rotation {
    NONE    = 0,
    CW_90   = 1,
    CW_180  = 2,
    CW_270  = 3,
    CW_360  = 0,
    CCW_90  = 3,
    CCW_180 = 2,
    CCW_270 = 1,
    CCW_360 = 0
};

enum class State {
    ZERO         = 0,
    NONE         = 0,
    DEFAULT      = 0,
    LEFT_BRANCH  = 0,
    ONE          = 1,
    RIGHT_BRANCH = 1
};

Rotation  operator*(const Rotation &a, const Rotation &b);
Rotation &operator*=(Rotation &a, const Rotation &b);
Rotation  operator-(const Rotation &a);
Direction operator*(const Direction &a, const Rotation &b);
State     operator!(const State &a);

class InvalidDirectionException : public std::exception {
  private:
    char *errorMessage;

  public:
    InvalidDirectionException();
    ~InvalidDirectionException();
    const char *what() const throw();
};

class InvalidVariantException : public std::exception {
  private:
    char *errorMessage;

  public:
    InvalidVariantException();
    ~InvalidVariantException();
    const char *what() const throw();
};

#endif