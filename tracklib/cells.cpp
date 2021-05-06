#include "cells.h"
#include "types.h"

Cell::Cell(CellType type, Variant variant, Rotation rotation, State state) : type(type), variant(variant), rotation(rotation), state(state) {}
Cell::Cell(const Cell &other) : Cell(other.type, other.variant, other.rotation, other.state) {}
Cell::Cell(const Cell *other) : Cell(other->type, other->variant, other->rotation, other->state) {}

Cell::~Cell() {}

Cell &Cell::operator=(const Cell &other) {
    type = other.type;
    variant = other.variant;
    rotation = other.rotation;
    state = other.state;
    return *this;
}

Cell &Cell::operator=(const Cell *other) {
    type = other->type;
    variant = other->variant;
    rotation = other->rotation;
    state = other->state;
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
                leftBranch = Direction::EAST;
                rightBranch = Direction::SOUTH;
                break;
            case Variant::RIGHT_HAND:
                leftBranch = Direction::SOUTH;
                rightBranch = Direction::WEST;
                break;
            case Variant::SYMMETRIC:
                leftBranch = Direction::EAST;
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

    else if (type == CellType::TRACK) {
        if (variant == Variant::STRAIGHT) {
            switch (adjustedDirction) {
                case Direction::NORTH:
                case Direction::SOUTH:
                    return (adjustedDirction * Rotation::CW_180) * rotation;
                default:
                    throw InvalidDirectionException();
            }
        }

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

        else if (variant == Variant::CROSS) {
            return from * Rotation::CW_180;
        }

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

        else {
            return from * Rotation::CW_180; // TODO boing
        }
    }

    else if (type == CellType::SPRUNG_JUNC || type == CellType::LAZY_JUNC) {
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
            return from * Rotation::CW_180; // TODO boing
        }
    }

    else if (type == CellType::ALTERNATING_JUNC) {
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
            return from * Rotation::CW_180; // TODO boing
        }
    }

    else {
        return from * Rotation::CW_180; // TODO boing
    }
}
