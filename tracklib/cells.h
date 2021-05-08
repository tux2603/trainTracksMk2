#ifndef __CELLS_GAURD_
#define __CELLS_GAURD_

#include "types.h"

class Cell {
  public:
    Cell(CellType type = CellType::NONE, Variant variant = Variant::DEFAULT, Rotation rotation = Rotation::NONE, State state = State::DEFAULT);
    Cell(const Cell &other);
    Cell(const Cell *other);
    ~Cell();

    Cell &operator=(const Cell &other);
    Cell &operator=(const Cell *other);

    CellType getType() const;
    Variant  getVariant() const;
    Rotation getRotation() const;
    State    getState() const;
    void     setType(CellType type);
    void     setVariant(Variant variant);
    void     setRotation(Rotation rotation);
    void     setState(State state);

    Cell &rotate(Rotation rotation);
    Cell &operator*(Rotation rotation);

    Direction enter(Direction from);
    Direction peak(Direction from) const;

  private:
    CellType type;
    Variant  variant;
    Rotation rotation;
    State    state;
};

class CellGrid {
  public:
    CellGrid(int width, int height);
    CellGrid(const CellGrid &other);
    ~CellGrid();

    int  getHeight() const;
    int  getWidth() const;
    void setHeight(int height);
    void setWidth(int width);

  private:
    int width, height;
    Cell **cells;
    
    void rebuildCells(int newWidth, int newHeight);
};

#endif