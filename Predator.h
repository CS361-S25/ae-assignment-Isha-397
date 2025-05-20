#ifndef PREDATOR_H
#define PREDATOR_H

#include "Org.h"
#include "World.h"

/**
 * @class Predator
 * @brief Represents a predator organism that hunts prey and requires energy to survive.
 */
class Predator : public Organism {
public:
    /**
     * @brief Construct a new Predator organism
     * @param _random Shared random number generator
     * @param _energy Starting energy (default 50.0)
     */
    Predator(emp::Ptr<emp::Random> _random, double _energy = 50.0)
        : Organism(_random, _energy, 0) {}
    
    /**
     * @brief Check if predator should die from energy depletion
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
        if (GetEnergy() <= 50) return nullptr;
        SetEnergy(GetEnergy() - 25);
        return emp::Ptr<Organism>(new Predator(random, 0));
    }
    
    /**
     * @brief Deduct movement energy cost
     */
    void Move() override {
        SetEnergy(GetEnergy() - 0.6);
    }

    /**
     * @brief Handle interaction with world at target position
     * @param world Reference to simulation world
     * @param from Current position index
     * @param to Target position index
     */
    void InteractAt(OrgWorld &world, size_t from, size_t to) override {
        if (world.IsOccupied(to)) {
            HandlePreyInteraction(world, from, to);
        } else {
            world.ReplaceOrganism(from, to); // Move to empty space
        }
    }

private:
    /**
     * @brief Handle interaction when prey is detected
     * @param world Reference to simulation world
     * @param from Current position index
     * @param to Target position index
     */
    void HandlePreyInteraction(OrgWorld &world, size_t from, size_t to) {
        Organism &other = *(world.GetOrgPtr(to));
        if (other.GetType() == 1) {  // Prey detected
            AddEnergy(17);
            other.SetEnergy(0);  // Kill prey
            world.ReplaceOrganism(from, to);  // Move to prey's position
        }
    }
};

#endif