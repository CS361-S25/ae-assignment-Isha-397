#ifndef PREY_H
#define PREY_H

#include "Org.h"
#include "World.h"

/**
 * @class Prey
 * @brief Represents a prey organism that can move, reproduce, and be consumed by predators.
 */
class Prey : public Organism {
public:
    /**
     * @brief Construct a new Prey organism
     * @param _random Shared random number generator
     * @param _energy Starting energy (default 0.0)
     */
    Prey(emp::Ptr<emp::Random> _random, double _energy = 0.0)
        : Organism(_random, _energy, 1) {}
    
    /**
     * @brief Check if prey should die (energy depleted)
     * @return true if energy ≤ 0
     */
    bool ShouldDie() const override { 
        return GetEnergy() <= 0;
    }
    
    /**
     * @brief Attempt to reproduce if sufficient energy
     * @return Pointer to offspring or nullptr
     */
    emp::Ptr<Organism> TryReproduce() override {
        if (GetEnergy() <= 20) return nullptr;
        SetEnergy(GetEnergy() - 11);
        return emp::Ptr<Organism>(new Prey(random, 0));
    }
    
    /**
     * @brief Deduct movement energy cost
     */
    void Move() override { 
        SetEnergy(GetEnergy() - 0.2); 
    }

    /**
     * @brief Handle interaction with world at target position
     * @param world Reference to simulation world
     * @param from Current position index
     * @param to Target position index
     */
    void InteractAt(OrgWorld &world, size_t from, size_t to) override {
        if (!world.IsOccupied(to)) {
            world.ReplaceOrganism(from, to); // Move to empty space
        } 
        else if (world.GetOrgPtr(to)->GetType() == 0) {
            SetEnergy(0); // Die if interacting with predator
        }
    }
};

#endif