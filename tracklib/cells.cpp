#include "cells.h"
#include "types.h"
#include <algorithm>

Cell::Cell(CellType type, Variant variant, Rotation rotation, State state) : type(type), variant(variant), rotation(rotation), state(state) {}
Cell::Cell(const Cell &other) : Cell(other.type, other.variant, other.rotation, other.state) {}
Cell::Cell(const Cell *other) : Cell(other->type, other->variant, other->rotation, other->state) {}

Cell::~Cell() {}

Cell &Cell::operator=(const Cell &other) {
    type     = other.type;
    variant  = other.variant;
    rotation = other.rotation;
    state    = other.state;
    return *this;
}

Cell &Cell::operator=(const Cell *other) {
    type     = other->type;
    variant  = other->variant;
    rotation = other->rotation;
    state    = other->state;
    return *this;
}

CellType Cell::getType() const {
    return type;
}

Variant Cell::getVariant() const {
    return variant;
}

Rotation Cell::getRotation() const {
    return rotation;
}

State Cell::getState() const {
    return state;
}

void Cell::setType(CellType type) {
    this->type = type;
}

void Cell::setVariant(Variant variant) {
    this->variant = variant;
}

void Cell::setRotation(Rotation rotation) {
    this->rotation = rotation;
}

void Cell::setState(State state) {
    this->state = state;
}

Cell &Cell::rotate(Rotation rotation) {
    this->rotation *= rotation;
    return *this;
}

Direction Cell::enter(Direction from) {
    Direction exitDirection = peak(from);

    Direction adjustedDirction = from * -rotation;

    if (type == CellType::ALTERNATING_JUNC) {
        if (adjustedDirction == Direction::NORTH)
            state = !state;
    }

    else if (type == CellType::LAZY_JUNC) {
        Direction leftBranch = Direction::INVALID, rightBranch = Direction::INVALID;

        switch (variant) {
            case Variant::LEFT_HAND:
                leftBranch  = Direction::EAST;
                rightBranch = Direction::SOUTH;
                break;
            case Variant::RIGHT_HAND:
                leftBranch  = Direction::SOUTH;
                rightBranch = Direction::WEST;
                break;
            case Variant::SYMMETRIC:
                leftBranch  = Direction::EAST;
                rightBranch = Direction::WEST;
                break;
            default:
                break;
        }

        if (adjustedDirction == leftBranch) {
            state = State::LEFT_BRANCH;
        }

        else if (adjustedDirction == rightBranch) {
            state = State::RIGHT_BRANCH;
        }
    }

    return exitDirection;
}

Direction Cell::peak(Direction from) const {
    Direction adjustedDirction = from * -rotation;

    if (type == CellType::NONE) {
        throw InvalidDirectionException();
    }

    // Straight tracks will simply send the train down them, with no branches or internal states to worry about whatsoever
    else if (type == CellType::TRACK) {
        // One straight track running from top to bottom
        if (variant == Variant::STRAIGHT) {
            switch (adjustedDirction) {
                case Direction::NORTH:
                case Direction::SOUTH:
                    return (adjustedDirction * Rotation::CW_180) * rotation;
                default:
                    throw InvalidDirectionException();
            }
        }

        // A single 90 degree curve from the top to the right
        else if (variant == Variant::CURVE) {
            switch (adjustedDirction) {
                case Direction::NORTH:
                    return Direction::EAST * rotation;
                case Direction::EAST:
                    return Direction::NORTH * rotation;
                default:
                    throw InvalidDirectionException();
            }
        }

        // Two straight tracks that cross over each other, one top to bottom and the other left to right
        else if (variant == Variant::CROSS) {
            return from * Rotation::CW_180;
        }

        // Two 90 degree curves in opposite corners of the cell, one going from top to right,
        //  the other going from left to bottom
        else if (variant == Variant::DOUBLE_CURVE) {
            switch (adjustedDirction) {
                case Direction::NORTH:
                    return Direction::EAST * rotation;
                case Direction::EAST:
                    return Direction::NORTH * rotation;
                case Direction::SOUTH:
                    return Direction::WEST * rotation;
                case Direction::WEST:
                    return Direction::SOUTH * rotation;
                default:
                    throw InvalidDirectionException();
            }
        }

        // Just in case the cell got into an invalid state, throw a fit about it
        else {
            throw InvalidVariantException();
        }
    }

    // Sprung junctions have two arms that connect to a common base. Entering through the base will always
    //  direct the train down the same arm, determined by the state of the junctions
    // Lazy junction will behave in the same way, except that the state of the junction will be modified
    //  when the train enters through one of the arms
    else if (type == CellType::SPRUNG_JUNC || type == CellType::LAZY_JUNC) {
        // The base of the junction is at the top, with the two arms branching off to the right and bottom
        if (variant == Variant::LEFT_HAND) {
            switch (adjustedDirction) {
                case Direction::NORTH:
                    return (state == State::LEFT_BRANCH ? Direction::EAST : Direction::SOUTH) * rotation;
                case Direction::EAST:
                    return Direction::NORTH * rotation;
                case Direction::SOUTH:
                    return Direction::NORTH * rotation;
                default:
                    throw InvalidDirectionException();
            }
        }

        // Base of the junction at the top, arms to the left and bottom
        else if (variant == Variant::RIGHT_HAND) {
            switch (adjustedDirction) {
                case Direction::NORTH:
                    return (state == State::LEFT_BRANCH ? Direction::SOUTH : Direction::WEST) * rotation;
                case Direction::WEST:
                    return Direction::NORTH * rotation;
                case Direction::SOUTH:
                    return Direction::NORTH * rotation;
                default:
                    throw InvalidDirectionException();
            }
        }

        // Base of the junction at the top, arms to the right and left
        else if (variant == Variant::SYMMETRIC) {
            switch (adjustedDirction) {
                case Direction::NORTH:
                    return (state == State::LEFT_BRANCH ? Direction::EAST : Direction::WEST) * rotation;
                case Direction::EAST:
                    return Direction::NORTH * rotation;
                case Direction::WEST:
                    return Direction::NORTH * rotation;
                default:
                    throw InvalidDirectionException();
            }
        }

        else {
            throw InvalidVariantException();
        }
    }

    // An alternating junction will alternate which of its two arms it sends the train down. A train entering
    //  through one of the arms is undefined behaviour and will raise an exception
    else if (type == CellType::ALTERNATING_JUNC) {
        // Variants are the same as in the other two junction types
        if (variant == Variant::LEFT_HAND) {
            if (adjustedDirction == Direction::NORTH)
                return (state == State::LEFT_BRANCH ? Direction::EAST : Direction::SOUTH) * rotation;
            else
                throw InvalidDirectionException();
        }

        else if (variant == Variant::RIGHT_HAND) {
            if (adjustedDirction == Direction::NORTH)
                return (state == State::LEFT_BRANCH ? Direction::SOUTH : Direction::WEST) * rotation;
            else
                throw InvalidDirectionException();
        }

        else if (variant == Variant::SYMMETRIC) {
            if (adjustedDirction == Direction::NORTH)
                return (state == State::LEFT_BRANCH ? Direction::EAST : Direction::WEST) * rotation;
            else
                throw InvalidDirectionException();
        }

        else {
            throw InvalidVariantException();
        }
    }

    else {
        throw InvalidVariantException();
    }
}


CellGrid::CellGrid(int width, int height) : width(width), height(height) {
    cells = new Cell *[width];
    for (int i = 0; i < width; ++i) {
        cells[i] = new Cell[height];
    }
}

CellGrid::CellGrid(const CellGrid &other) : CellGrid(other.width, other.height) {
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            cells[x][y] = Cell(other.cells[x][y]);
        }
    }
}

CellGrid::~CellGrid() {}

int CellGrid::getWidth() const { return width; }
int CellGrid::getHeight() const { return height; }

void CellGrid::setWidth(int width) {
    rebuildCells(width, height);
    this->width = width;
}

void CellGrid::setHeight(int height) {
    rebuildCells(width, height);
    this->height = height;
}

void CellGrid::rebuildCells(int newWidth, int newHeight) {
    Cell **newCells = new Cell *[newWidth];
    for (int i = 0; i < newWidth; ++i) {
        newCells[i] = new Cell[newHeight];
    }

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            newCells[x][y] = Cell(cells[x][y]);
        }
    }

    for (int i = 0; i < width; ++i) {
        delete[] cells[i];
    }

    delete[] cells;

    cells = newCells
}