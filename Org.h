#ifndef ORG_H
#define ORG_H
#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

/**
 * @class Organism
 * @brief Base class representing a generic organism with energy and type.
 */
class Organism
{
private:
    double energy;                     ///< Energy level of the organism.
    emp::Ptr<emp::Random> random;      ///< Pointer to RNG.
    int type_id;                       ///< 0 for Predator, 1 for Prey.


public:
    /** 
     * Constructor for Organism.
     * Creates an organism.
    */
    Organism(emp::Ptr<emp::Random> _random, double _energy = 0.0, int _type_id = 0)
        : energy(_energy), random(_random), type_id(_type_id) {}

    // Accessors and mutators
    void SetEnergy(double _in) { energy = _in; }
    void AddEnergy(double _in) { energy += _in; }
    int GetType() const { return type_id; }  
    double GetEnergy() const { return energy; }

    /**
     * @brief Increases energy by absorbing a fixed amount (plus passed-in bonus).
     */
    virtual void ConsumeEnergy(double _energy) { energy += (_energy) + 10; }  
    
    
    /**
     * @brief Attempts reproduction. Only works if energy is high enough.
     * @return New organism if successful, otherwise nullptr.
     */
    virtual emp::Ptr<Organism> TryReproduce()  
    {
        if (energy <= 100) return nullptr;

        emp::Ptr<Organism> offspring = new Organism(*this);
        offspring->energy = 0;
        energy -= 10;
        return offspring;
    }
};
#endif
