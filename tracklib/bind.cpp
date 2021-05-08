#include <iostream>
#include <pybind11/pybind11.h>

#include "cells.h"
#include "types.h"


namespace py = pybind11;

PYBIND11_MODULE(TrackLib, m) {



    // #######################################################################
    // #####              Expose the cell description enums              #####
    // #######################################################################



    py::enum_<Direction>(m, "Direction")
            .value("north", Direction::NORTH)
            .value("south", Direction::SOUTH)
            .value("east", Direction::EAST)
            .value("west", Direction::WEST)
            .def("__mul__", [](const Direction &a, const Rotation &b) {
                return a * b;
            });

    py::enum_<CellType>(m, "CellType")
            .value("none", CellType::NONE)
            .value("track", CellType::TRACK)
            .value("sprung_junction", CellType::SPRUNG_JUNC)
            .value("lazy_junction", CellType::LAZY_JUNC)
            .value("alternating_junction", CellType::ALTERNATING_JUNC);

    py::enum_<Variant>(m, "CellVariant")
            .value("default", Variant::DEFAULT)
            .value("straight", Variant::STRAIGHT)
            .value("curve", Variant::CURVE)
            .value("cross", Variant::CROSS)
            .value("double_curve", Variant::DOUBLE_CURVE)
            .value("left_hand", Variant::LEFT_HAND)
            .value("right_hand", Variant::RIGHT_HAND)
            .value("symmetric", Variant::SYMMETRIC);

    py::enum_<Rotation>(m, "CellRotation")
            .value("none", Rotation::NONE)
            .value("clockwise_90", Rotation::CW_90)
            .value("clockwise_180", Rotation::CW_180)
            .value("clockwise_270", Rotation::CW_270)
            .value("clockwise_360", Rotation::CW_360)
            .value("counterclockwise_90", Rotation::CCW_90)
            .value("counterclockwise_180", Rotation::CCW_180)
            .value("counterclockwise_270", Rotation::CCW_270)
            .value("counterclockwise_360", Rotation::CCW_360)
            .def("__mul__", [](const Rotation &a, const Rotation &b) {
                return a * b;
            });

    py::enum_<State>(m, "CellState")
            .value("zero", State::ZERO)
            .value("none", State::NONE)
            .value("default", State::DEFAULT)
            .value("left_branch", State::LEFT_BRANCH)
            .value("one", State::ONE)
            .value("right_branch", State::RIGHT_BRANCH);


    py::register_exception<InvalidDirectionException>(m, "InvalidDirectionException", PyExc_ValueError);
    py::register_exception<InvalidVariantException>(m, "InvalidVariantException", PyExc_ValueError);



    // #######################################################################
    // #####                    Expose the cell class                    #####
    // #######################################################################



    auto pyCell = py::class_<Cell>(m, "Cell");
    pyCell.def(py::init<CellType, Variant, Rotation, State>());
    pyCell.def("rotate", &Cell::rotate);
    pyCell.def("enter", &Cell::enter);
    pyCell.def("peak", &Cell::peak);
    pyCell.def_property("type", &Cell::getType, &Cell::setType);
    pyCell.def_property("variant", &Cell::getVariant, &Cell::setVariant);
    pyCell.def_property("rotation", &Cell::getRotation, &Cell::setRotation);
    pyCell.def_property("state", &Cell::getState, &Cell::setState);
    // pyCell.def(py::init<const Cell &>());

    auto pyCellGrid = py::class_<CellGrid>(m, "CellGrid");
    pyCellGrid.def(py::init<int, int>());
    pyCellGrid.def_property("height", &CellGrid::getHeight, &CellGrid::setHeight);
    pyCellGrid.def_property("width", &CellGrid::getWidth, &CellGrid::setWidth);
    pyCellGrid.def_property(
            "size",
            [](CellGrid &self) {
                return py::make_tuple(self.getWidth(), self.getHeight());
            },
            [](CellGrid &self, std::tuple<int, int> s) {
                self.setWidth(std::get<0>(s));
                self.setHeight(std::get<1>(s));
            });
}
