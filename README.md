[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/-GCLf3vW)


# Ecological Dynamics Simulation

This simulation models a simple artificial ecosystem with two species: **Predators** and **Prey**. Each organism type has its own behaviors and interactions that affect population dynamics over time.

## Species Description

### 🐭 Prey
- **Role**: Prey serve as the food source for predators.
- **Energy System**: They begin with a small amount of energy and gain energy over time.
- **Reproduction**: If a prey organism accumulates enough energy, it can reproduce by creating a copy of itself with reduced energy.
- **Vulnerability**: Prey lose energy when consumed by predators and cannot defend themselves.

### 🐺 Predator
- **Role**: Predators hunt prey to gain energy and survive.
- **Energy System**: They start with a certain amount of energy and lose energy each time step.
- **Feeding**: When a predator encounters a prey in a neighboring cell, it consumes the prey and gains energy.
- **Mortality**: If a predator's energy drops below a threshold (10), it dies and is removed from the simulation.

## Interactions

- **Predation**: When a predator and prey are adjacent, the predator consumes the prey and replaces its position on the grid.
- **Movement**: Both species move randomly within the grid-based world.
- **Reproduction**: Organisms reproduce if their energy exceeds a certain threshold:
  - Prey reproduce when energy > 10.
  - Predators reproduce when energy > 100 (shared behavior logic from base `Organism` class).
- **Death**: 
  - Prey can be eaten by predators.
  - Predators die if their energy drops too low.

## Running the Simulation

- Use the **Toggle** button to start or pause the simulation.
- Use the **Step** button to manually advance one time step.

The ecosystem is visualized using a grid:
- **Black cells**: Prey
- **Red cells**: Predators
- **Green cells**: Empty

# Author: 

Isha Patel
