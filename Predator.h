#ifndef PREDATOR_H
#define PREDATOR_H
#include "Org.h"

/**
 * @class Predator
 * @brief Subclass of Organism. Predators die if energy is too low.
 */
class Predator : public Organism
{
public:
    Predator(emp::Ptr<emp::Random> _random, double _energy = 0.0, int _type_id = 0)
        : Organism(_random, _energy, _type_id) {}

    /**
     * @brief Checks if predator should die based on energy threshold.
     */
    bool NeedsToDie()  {
        return GetEnergy() < 10;
    }
};
#endif
