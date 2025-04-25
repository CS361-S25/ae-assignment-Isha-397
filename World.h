#ifndef WORLD_H
#define WORLD_H
#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "Org.h"
#include "Prey.h"
#include "Predator.h"


/**
 * @class OrgWorld
 * @brief A customized evolutionary world that manages interactions between organisms in a grid.
 */
class OrgWorld : public emp::World<Organism>
{
private:
    emp::Random &random;
    emp::Ptr<emp::Random> random_ptr;
    int species;

public:
	/**
     * @brief Constructor for OrgWorld.
     * @param _random Reference to a random number generator.
     */
    OrgWorld(emp::Random &_random)
        : emp::World<Organism>(_random), random(_random)
    {
        random_ptr.New(_random);
    }

	/**
     * @brief Update all organisms: energy consumption, reproduction, and movement.
     */
    void Update()  
    {	
		// Randomize update order for fairness
        auto schedule = emp::GetPermutation(random, GetSize());
		
		// First pass: energy consumption for prey
        for (auto idx : schedule) {
            if (!IsOccupied(idx)) continue;

            if (pop[idx]->GetType() == 1) {  
                pop[idx]->ConsumeEnergy(10);  // Prey absorb energy from environment
            }
        }

		// Second pass: reproduction and movement
        schedule = emp::GetPermutation(random, GetSize());
        for (auto idx : schedule) {
            if (!IsOccupied(idx)) continue;

			// Attempt reproduction
            if (auto offspring = pop[idx]->TryReproduce()) {  
                auto new_pos = GetRandomNeighborPos(idx).GetIndex();
                AddOrgAt(offspring, new_pos);
            }
			// Attempt movement and interaction
            MoveOrganism(idx); 
        }
    }

	/**
     * @brief Replace organism at position `to` with the one from `from`.
	 * @param from the oragnism at index i to extract.
     * @param to y-coordiante of the grid cell to find neighbors of.
     * @return the total number of alive neighbors for the grid cell.
    */
    void ReplaceOrganism(int from, int to)  
    {
        auto org = pop[from];
        pop[from] = nullptr;
        RemoveOrgAt(to);
        AddOrgAt(org, to);
    }

	/**
 * @brief Moves the organism at index i to a neighboring cell.
 *        Handles movement, predation (if types differ), and death by low energy.
 */
    void MoveOrganism(int i)  
    {
		// Select a random neighboring position to potentially move into
        emp::WorldPosition j = GetRandomNeighborPos(i);

		// CASE 1: Target cell is occupied
        if (pop[j.GetIndex()] != nullptr)
        {
            if (pop[i]->GetType() != pop[j.GetIndex()]->GetType()) {
				// If two different types meet, simulate predation
                if (pop[i]->GetType() == 0) {
					// Predator eats prey
                    pop[i]->ConsumeEnergy(1.5);
                    ReplaceOrganism(i, j.GetIndex());  
                }
				// If neighbor is a predator, it eats the current prey
                if (pop[j.GetIndex()]->GetType() == 0) {
                    pop[j.GetIndex()]->ConsumeEnergy(1.5); // Gain energy from prey
                    ReplaceOrganism(j.GetIndex(), i);  // Move predator into prey's spot
                }
            }
        }

		// CASE 2: Target cell is empty
        if (pop[j.GetIndex()] == nullptr)
        {
			// Move organism to the empty neighbor cell
            emp::Ptr<Organism> organism = pop[i];
            pop[i] = nullptr;
            AddOrgAt(organism, j.GetIndex());
        }

		// Remove predator if energy too low
        if (pop[i]->GetType() == 0 && pop[i].DynamicCast<Predator>()->NeedsToDie())
        {
            RemoveOrgAt(i); 
        }
    }
	
};
#endif