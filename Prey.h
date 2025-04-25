#ifndef PREY_H
#define PREY_H
#include "Org.h"


/**
 * @class Prey
 * @brief Subclass of Organism. Prey can reproduce with enough energy.
 */
class Prey : public Organism
{
public:
    Prey(emp::Ptr<emp::Random> _random, double _energy = 0.0, int _type_id = 0)
        : Organism(_random, _energy, _type_id) {}


    /**
     * @brief Attempts reproduction if energy is above 10.
     * @return New Prey offspring or nullptr.
     */
    emp::Ptr<Organism> TryReproduce()  
    {
        if (GetEnergy() <= 10) return nullptr;

        emp::Ptr<Organism> offspring = new Organism(*this);
        offspring->SetEnergy(0);
        SetEnergy(GetEnergy() - 10);
        return offspring;
    }
};
#endif
