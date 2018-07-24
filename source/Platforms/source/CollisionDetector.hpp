#pragma once
#include <list>
#include <vector>
#include "Platform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

//keeps track of all platforms and so can tell, whether there would be collision with given points
class CollisionDetector {
    std::vector<Platform> platforms;
    bool coordinateLaysIn(glm::vec3 co, glm::vec3 from, glm::vec3 to);
public:
    //add a platform to keep track of
    void addPlatform(Platform platform);
    
    //check if a position is inside one of the platforms
    bool collision(glm::vec3 objectBottomPosition);
    
    //deletes all platforms it has
    void reset();
};