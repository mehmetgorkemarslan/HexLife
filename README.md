# HexLife: A Neural Evolution Simulation

HexLife is a high-performance life simulation built in C++ from the ground up, with zero external dependencies. The project simulates a world of hexagonal grids where neural-network-controlled entities survive, reproduce, and evolve through natural selection.

## Project Overview

The simulation consists of a procedurally generated hexagonal world. Entities (organisms) inhabit this world, each possessing a unique DNA string that defines their physical traits and the architecture of their neural network "brain." Through generations of survival and mating, these entities evolve complex behaviors to optimize their energy consumption and reproduction.

### Core Features
- **Hexagonal Grid System:** Implemented using Cube Coordinates for efficient spatial calculations.
- **Procedural Generation:** Custom-built noise algorithms to generate biomes (water, plains, forests).
- **Neural Network Brains:** Hand-written Feed-Forward Neural Networks (FFNN) without external ML libraries.
- **Genetic Evolution:** Implementation of crossover and mutation mechanisms to facilitate natural selection.
- **Zero-Dependency:** Focused on pure C++20 and STL for maximum performance and portability.

##  Technical Stack
- **Language:** C++20
- **Math:** Custom Hexagonal Math & Linear Algebra for Neural Networks.

## Getting Started

### Prerequisites
- A C++20 compatible compiler (GCC 10+, Clang 10+, or MSVC 2019+).
- CMake (3.15 or higher).

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make
./HexLife config.json
```
## Road Map
- [ ] Implement Cube Coordinate Hex System

- [ ] Procedural Map Generation (Perlin Noise)

- [ ] Entity Metabolism and DNA Structs

- [ ] Neural Network Controller Integration

- [ ] Statistics Tracking and Data Serialization

---
**Authors:**

Mehmet Görkem Arslan - https://github.com/mehmetgorkemarslan

Mustafa Kaan Çetin - https://github.com/MustafaKaanCetin