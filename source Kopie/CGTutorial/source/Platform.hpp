#pragma once
#include <glm/gtc/matrix_transform.hpp>
//#include "CollisionDetector.hpp"

#include <math.h>


class Platform {
    void determineFromTo();
    glm::vec3 measures = glm::vec3(2, 0.5, 2);
public:
    glm::vec3 position;
    glm::vec3 from;
    glm::vec3 to;
    
//    static void initialize(CollisionDetector* collisionDetector);
    
    Platform(glm::vec3 position);
};
