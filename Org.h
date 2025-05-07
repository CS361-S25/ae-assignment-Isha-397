#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class OrgWorld;  // Forward declaration

/**
 * @class Organism
 * @brief Abstract base class representing a biological organism in the ecosystem.
 * 
 * Provides core functionality for energy management, reproduction, movement,
 * and interactions. Derived classes should implement specific behaviors.
 */
class Organism {
protected:
    double energy;                ///< Current energy level of the organism
    emp::Ptr<emp::Random> random; ///< Shared random number generator
    int type_id;                  ///< Organism type (0=Predator, 1=Prey)

public:
    /**
     * @brief Construct a new Organism
     * @param _random Pointer to random number generator
     * @param _energy Initial energy level (default 0.0)
     * @param _type_id Organism type identifier (default 0)
     */
    Organism(emp::Ptr<emp::Random> _random, 
             double _energy = 0.0, 
             int _type_id = 0)
        : energy(_energy), random(_random), type_id(_type_id) {}

    virtual ~Organism() = default;

    // ======================
    //  Energy Management
    // ======================
    
    /**
     * @brief Set the organism's energy level
     * @param _in New energy value
     */
    void SetEnergy(double _in) { energy = _in; }
    
    /**
     * @brief Add to the organism's energy
     * @param _in Energy to add
     */
    void AddEnergy(double _in) { energy += _in; }
    
    /**
     * @brief Get current energy level
     * @return double Current energy
     */
    double GetEnergy() const { return energy; }
    
    /**
     * @brief Consume energy from environment
     * @param _bonus Additional energy bonus
     */
    virtual void ConsumeEnergy(double _bonus) { 
        energy += _bonus + 10.0; 
    }

    // ======================
    //  Type Information
    // ======================
    
    /**
     * @brief Get organism type
     * @return int Type identifier
     */
    int GetType() const { return type_id; }
    
    /**
     * @brief Get random number generator
     * @return emp::Ptr<emp::Random> 
     */
    emp::Ptr<emp::Random> GetRandomPtr() const { return random; }

    // ======================
    //  Pure Virtual Methods
    // ======================
    
    /**
     * @brief Check if organism should die
     * @return true if organism should die
     */
    virtual bool ShouldDie() const = 0;
    
    /**
     * @brief Attempt reproduction
     * @return Pointer to offspring or nullptr
     */
    virtual emp::Ptr<Organism> TryReproduce() = 0;
    
    /**
     * @brief Perform movement action
     */
    virtual void Move() = 0;
    
    /**
     * @brief Handle interaction with another organism
     * @param world Reference to the world
     * @param from Current position
     * @param to Target position
     */
    virtual void InteractAt(OrgWorld &world, 
                          size_t from, 
                          size_t to) = 0;
};

#endif