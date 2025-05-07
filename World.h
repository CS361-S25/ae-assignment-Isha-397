#ifndef WORLD_H
#define WORLD_H
#pragma once
#include "emp/Evolve/World.hpp"
#include "emp/math/random_utils.hpp"
#include "emp/math/Random.hpp"
#include "Org.h"
class Prey;
class Predator;


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

    /**
     * @brief Check if the organism at index `idx` is dead and remove it if so.
     * @param idx Index of the organism to check.
     * @return true if the organism was removed, false otherwise.
     */
    bool CheckAndRemoveIfDead(size_t idx) {
        if (IsOccupied(idx) && pop[idx]->ShouldDie()) {
            RemoveOrgAt(idx);
            return true;
        }
        return false;
    }
    
    /**
    * @brief Attempts reproduction for organism at specified position
    * @param idx Grid position index of parent organism
    * @note Creates offspring and places in random adjacent empty cell.
    *       Deletes offspring if no space available.
    */
    void TryReproduceAt(size_t idx) {
        if (!IsOccupied(idx)) return;
        emp::Ptr<Organism> baby = pop[idx]->TryReproduce();
        if (baby) {
            size_t baby_pos = GetRandomNeighborPos(idx).GetIndex();
            if (!IsOccupied(baby_pos)) {
                AddOrgAt(baby, baby_pos);
            } else {
                delete baby;
            }
        }
    }
    
    /**
    * @brief Handles organism interaction or movement between cells
    * @param from Source grid position index
    * @param to Target grid position index 
    * @return New position index of organism after interaction/movement
    * @details If target occupied, triggers interaction. If empty, moves organism.
    *          Returns original position if organism dies during interaction.
    */
    size_t HandleInteractionOrMove(size_t from, size_t to) {
        emp::Ptr<Organism> org = pop[from];
        if (IsOccupied(to)) {
            org->InteractAt(*this, from, to);
            if (!IsOccupied(from) || pop[from]->ShouldDie()) {
                RemoveOrgAt(from);
                return from;
            }
            return (pop[to] == org) ? to : from;
        } else {
            AddOrgAt(org, to);
            pop[from] = nullptr;
            return to;
        }
    }
    

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
		
		// First pass: energy consumption 
        for (auto idx : schedule) {
            if (!IsOccupied(idx)) continue;

            if (pop[idx]->GetType() == 1) {  
                pop[idx]->ConsumeEnergy(10);  // Prey absorb energy from environment
            }
            if (pop[idx]->GetType() == 0) {
                pop[idx]->ConsumeEnergy(.001);   // Predators absorb less
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
    *       Handles movement, predation (if types differ), and death by low energy.
    */
    void MoveOrganism(int i) {
        if (!IsOccupied(i)) return;
        emp::Ptr<Organism> org = pop[i];
        org->Move();
        if (CheckAndRemoveIfDead(i)) return;
    
        size_t new_pos = GetRandomNeighborPos(i).GetIndex();
        size_t final_pos = HandleInteractionOrMove(i, new_pos);
        TryReproduceAt(i);
        CheckAndRemoveIfDead(final_pos);
    }
    
};
#endif
