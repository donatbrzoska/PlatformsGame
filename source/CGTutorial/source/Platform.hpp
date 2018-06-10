#pragma once
#include <glm/gtc/matrix_transform.hpp>
//#include "CollisionDetector.hpp"

#include <math.h>
#include "objects.hpp"
#include "Util.hpp"
#include "MVP.hpp"


class Platform {
    void determineFromTo();
    glm::vec3 measures = glm::vec3(0.8, 0.15, 0.8);
    void drawFromTo(glm::mat4 Model);
public:
    glm::vec3 position;
    glm::vec3 from;
    glm::vec3 to;
    
//    static void initialize(CollisionDetector* collisionDetector);
    
    Platform(glm::vec3 position);
    void draw();
    ~Platform();
};
