#include "emp/math/Random.hpp"
#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"
#include "World.h"
#include "Org.h"
#include "Prey.h"
#include "Predator.h"



emp::web::Document doc{"target"};


/**
 * @class AEAnimator
 * @brief Web-based animation class for simulating an artificial ecosystem of predators and prey.
 */
class AEAnimator : public emp::web::Animate
{
    //create world
    emp::Random random{5};
    OrgWorld world{random};

    // Grid and canvas configuration
    const int num_h_boxes = 100;
    const int num_w_boxes = 100;
    const double RECT_SIDE = 5;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};

    emp::web::Canvas canvas{width, height, "canvas"};

public:
    /**
     * @brief Constructor for AEAnimator.
     * Initializes canvas, buttons, and adds organisms to the world.
     */

    AEAnimator()
    {
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");

        // Add explanatory text to the webpage
        doc << "<div id='description' style='font-family: Arial, sans-serif; padding: 20px; color: #333;'>" 
            << "<h1>Ecological Dynamics Simulation</h1>"
            << "<p>Welcome to this basic simulation of an ecological system. In this demonstration, you will observe two species interacting with each other: Predators and Prey.</p>"
            << "<p><strong>Predators</strong> are organisms that hunt other creatures for sustenance. They begin with a set number of points, which represent their energy levels. As they hunt and consume Prey, their points increase. However, if their points drop too low, they will die.</p>"
            << "<p><strong>Prey</strong> are organisms that are hunted by Predators. Prey can reproduce if they accumulate enough points, but they are vulnerable to Predators. Prey will lose points when they are eaten.</p>"
            << "<p>The simulation represents a simple ecosystem where Predators and Prey coexist. You will see how the balance of the ecosystem changes over time as organisms move, interact, and reproduce. Each organism's behavior is driven by their energy levels and species-specific logic.</p>"
            << "<p>Click 'Step' to advance one time step and observe the changes. Use 'Toggle' to pause and restart the simulation.</p>"
            << "</div>";


        // Add Predator and Prey to the world
        Prey *prey = new Prey(&random, 2.5, 1);
        Predator *predator = new Predator(&random, 2.5, 0);

        world.AddOrgAt(predator, 100);
        world.AddOrgAt(prey, 10);

        world.SetPopStruct_Grid(num_w_boxes, num_h_boxes);
    }

    /**
     * @brief Called each frame during animation.
     * Clears canvas, updates world, and draws grid cells based on organism type.
     */
    void DoFrame() 
    {
        canvas.Clear();

        world.Update();
        std::string color;
        int org_num = 0;

            for (int x = 0; x < num_w_boxes; ++x) {
                for (int y = 0; y < num_h_boxes; ++y) {
                    // If occupied, draw in organism color; otherwise draw green for empty
                    if (world.IsOccupied(org_num)) {
                        // Set color: red for predator, black for prey
                        color = (world.GetOrg(org_num).GetType() == 0) ? "red" : "black";

                        // Draw the organism with its corresponding color
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, color, color);
                    } else {
                        // Draw empty cell as green
                        canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "green", "green");
                    }
                ++org_num;
            }
        }
    }
       
};

AEAnimator animator;

int main()
{
    //Have animator call DoFrame once to start
    animator.Step();
}