# Train Tracks Mk 2

An improved implementation of computational train tracks as discussed in Ian Stewart's _The Magical Maze_.

## Basic Stuff

This is a project aiming to implement a generic Harvard architecture computer using nothing but model train tracks with three very special types of junctions.
In order to make design easier, these junctions and tracks will be laid out on a square grid, with no diagonal or otherwise bent tracks allowed. At any time,
there will only ever be one train on the track. This removes the possibility of any collisions between trains and makes deterministic behavior much easier
to achieve.

Variousutilities are planned on being developed, including a python GUI to help designing the train tracks, a tool to generate a minecraft datapack that
will simulate the train track, and probably a few others.

## Terminology

- **Alternating Junction**: A junction where the train should always enter from the base. Each time the train enters from the base, it will exit from
  the opposite arm as the last time, alternating back and forth between the two arms. The train entering from one of the arms is undefined behavior
  and should be avoided.
- **Arm**: See **Junction**.
- **Base**: See **Junction**.
- **Cell**: Any of the individual squares on the grid. It can be completely empty, contain straight or curved tracks, or any of the three types of junctions.
- **Junction**: A point where two branches of track join together. The two tracks coming off of the junction are called the **arms**, and the track where they
  join together is called the **base**. When a train enters either of the two arms of the junction, it will always exit through the base. When entering from
  the base it can exit through either of the two arms, depending on the junction's type and internal state
- **Lazy Junction**: A junction that simply directs the train down whichever arm it was last on. Whenever the train enters through on of the arms, the internal
  state is modified and the train exits from the base. Open entering the base, the junction will check its internal state and the train will exit on whichever
  arm it last entered from.
- **Sprung Junction**: A junction where the train is always directed down the same arm. When the train enters through the base, it will always exit from the
  same arm.
- **Track**: The track that the train runs on.
- **Train**: A generic model train. For the purposes of the simulation, the train is assumed to be a point mass and able to to turn on a dime.

## File Structure

Train track designs are stored in a compressed binary voxel format. The first 8 bytes in the file specify the dimensions of the design, x dimension in the first
four bytes and y dimension in the second four bytes. Remaining entries are all one byte long, stored in row-major order. If the most significant bit is a one,
there is a cell present at this location. If the most significant bit is 0, the cell and the following `n` cells are empty, where `n` is the value of the rest
of the bits. The mapping from bit vector to cell type is as follows

| 7   | 6 - 5     | 4 - 3        | 2 - 1    | 0              |
| --- | --------- | ------------ | -------- | -------------- |
| 1   | Cell Type | Cell Variant | Rotation | Internal State |

| 7   | 6 - 0           |
| --- | --------------- |
| 0   | Num Empty Cells |

### Cell Types

| Type                 | Value | Bit Values |
| -------------------- | ----- | ---------- |
| Track                | 0     | `0b00`     |
| Sprung Junction      | 1     | `0b01`     |
| Lazy Junction        | 2     | `0b10`     |
| Alternating Junction | 3     | `0b11`     |

### Cell Variants

| Variant              | Value | Bit Values |
| -------------------- | ----- | ---------- |
| No Variant           | 0     | `0b00`     |
| Left Hand Junction   | 0     | `0b00`     |
| Straight Track       | 0     | `0b00`     |
| Right Hand Junction  | 1     | `0b01`     |
| Curved Track         | 1     | `0b01`     |
| Symmetric Junction   | 2     | `0b10`     |
| Crossed Tracks       | 2     | `0b10`     |
| Double Curved Tracks | 3     | `0b11`     |

### Cell Rotations

| Rotation              | Value | Bit Values |
| --------------------- | ----- | ---------- |
| 0°                    | 0     | `0b00`     |
| 90° Clockwise         | 1     | `0b01`     |
| 90° Counterclockwise  | 3     | `0b11`     |
| 180° Clockwise        | 2     | `0b10`     |
| 180° Counterclockwise | 2     | `0b10`     |
| 270° Clockwise        | 3     | `0b11`     |
| 270° Counterclockwise | 1     | `0b01`     |
| 360° Clockwise        | 0     | `0b00`     |
| 360° Counterclockwise | 0     | `0b00`     |

### Internal States

| State        | Value | Bit Values |
| ------------ | ----- | ---------- |
| None         | 0     | `0b0`      |
| Direct Left  | 0     | `0b0`      |
| Direct Right | 1     | `0b1`      |
