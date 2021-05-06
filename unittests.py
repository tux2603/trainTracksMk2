#!/usr/bin/env python

import unittest

from TrackLib import *

class EnumTest(unittest.TestCase):
    def test_equality(self):
        self.assertEqual(CellVariant.default, CellVariant.straight)
        self.assertEqual(CellRotation.none, CellRotation.counterclockwise_360)
        self.assertEqual(CellRotation.clockwise_90, CellRotation.counterclockwise_270)

    def test_rotations(self):
        self.assertEqual(CellRotation.clockwise_90 * CellRotation.clockwise_90, CellRotation.clockwise_180)
        self.assertEqual(CellRotation.clockwise_90 * CellRotation.counterclockwise_90, CellRotation.none)

class CellTest(unittest.TestCase):
    def test_cell_properties(self):
        cell = Cell(CellType.track, CellVariant.cross, CellRotation.clockwise_90, CellState.none)

        self.assertEqual(cell.type, CellType.track)
        self.assertEqual(cell.variant, CellVariant.cross)
        self.assertEqual(cell.rotation, CellRotation.clockwise_90)
        self.assertEqual(cell.state, CellState.none)

        cell.type = CellType.lazy_junction
        cell.variant = CellVariant.symmetric
        cell.rotation = CellRotation.counterclockwise_180
        cell.state = CellState.right_branch

        self.assertEqual(cell.type, CellType.lazy_junction)
        self.assertEqual(cell.variant, CellVariant.symmetric)
        self.assertEqual(cell.rotation, CellRotation.counterclockwise_180)
        self.assertEqual(cell.state, CellState.right_branch)

    def test_cell_rotation(self):
        cell = Cell(CellType.sprung_junction, CellVariant.left_hand, CellRotation.clockwise_180, CellState.zero)
        self.assertEqual(cell.rotation, CellRotation.clockwise_180)

        cell.rotate(CellRotation.clockwise_90)
        self.assertEqual(cell.rotation, CellRotation.clockwise_270)
        
        cell.rotate(CellRotation.counterclockwise_180)
        self.assertEqual(cell.rotation, CellRotation.clockwise_90)

    def test_tracks(self):
        # Test straight tracks
        cell = Cell(CellType.track, CellVariant.straight, CellRotation.none, CellState.none)
        self.assertEqual(cell.peak(Direction.north), Direction.south)
        self.assertEqual(cell.peak(Direction.south), Direction.north)
        self.assertRaises(InvalidDirectionException, lambda: cell.peak(Direction.east))
        self.assertRaises(InvalidDirectionException, lambda: cell.peak(Direction.west))
        self.assertEqual(cell.enter(Direction.north), Direction.south)
        self.assertEqual(cell.state, CellState.none)

        cell.rotation *= CellRotation.clockwise_90
        self.assertEqual(cell.peak(Direction.east), Direction.west)
        self.assertEqual(cell.peak(Direction.west), Direction.east)
        self.assertRaises(InvalidDirectionException, lambda: cell.peak(Direction.north))
        self.assertRaises(InvalidDirectionException, lambda: cell.peak(Direction.south))
        self.assertEqual(cell.enter(Direction.east), Direction.west)
        self.assertEqual(cell.state, CellState.none)

        cell.variant = CellVariant.curve
        cell.rotation = CellRotation.none
        self.assertEqual(cell.peak(Direction.north), Direction.east)
        self.assertEqual(cell.peak(Direction.east), Direction.north)
        cell.rotation *= CellRotation.clockwise_90
        self.assertEqual(cell.peak(Direction.east), Direction.south)
        self.assertEqual(cell.peak(Direction.south), Direction.east)
        cell.rotation *= CellRotation.clockwise_90
        self.assertEqual(cell.peak(Direction.south), Direction.west)
        self.assertEqual(cell.peak(Direction.west), Direction.south)
        cell.rotation *= CellRotation.clockwise_90
        self.assertEqual(cell.peak(Direction.west), Direction.north)
        self.assertEqual(cell.peak(Direction.north), Direction.west)

        cell.variant = CellVariant.cross
        cell.rotation = CellRotation.none
        self.assertEqual(cell.peak(Direction.north), Direction.south)
        self.assertEqual(cell.peak(Direction.south), Direction.north)
        self.assertEqual(cell.peak(Direction.east), Direction.west)
        self.assertEqual(cell.peak(Direction.west), Direction.east)

        cell.variant = CellVariant.double_curve
        self.assertEqual(cell.peak(Direction.north), Direction.east)
        self.assertEqual(cell.peak(Direction.east), Direction.north)
        self.assertEqual(cell.peak(Direction.south), Direction.west)
        self.assertEqual(cell.peak(Direction.west), Direction.south)

    def test_sprung_point(self):
        cell = Cell(CellType.sprung_junction, CellVariant.left_hand, CellRotation.none, CellState.left_branch)
        self.assertEqual(cell.peak(Direction.north), Direction.east)
        self.assertEqual(cell.peak(Direction.east), Direction.north)
        self.assertEqual(cell.peak(Direction.south), Direction.north)
        cell.state = CellState.right_branch
        self.assertEqual(cell.peak(Direction.north), Direction.south)
        cell.variant = CellVariant.right_hand
        self.assertEqual(cell.peak(Direction.north), Direction.west)
        self.assertEqual(cell.peak(Direction.west), Direction.north)
        self.assertEqual(cell.peak(Direction.south), Direction.north)
        cell.state = CellState.left_branch
        self.assertEqual(cell.peak(Direction.north), Direction.south)
        self.assertEqual(cell.peak(Direction.west), Direction.north)
        self.assertEqual(cell.peak(Direction.south), Direction.north)
        cell.variant = CellVariant.symmetric
        self.assertEqual(cell.peak(Direction.north), Direction.east)
        cell.state = CellState.right_branch
        self.assertEqual(cell.peak(Direction.north), Direction.west)
        cell.enter(Direction.north)
        self.assertEqual(cell.state, CellState.right_branch)
        cell.enter(Direction.east)
        self.assertEqual(cell.state, CellState.right_branch)
        cell.enter(Direction.west)
        self.assertEqual(cell.state, CellState.right_branch)


if __name__ == '__main__':
    unittest.main()