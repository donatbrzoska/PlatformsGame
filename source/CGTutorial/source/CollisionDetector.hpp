#pragma once
#include <list>
#include <vector>
#include "Platform.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <string>

class CollisionDetector {
//    std::list<Platform*> platforms;
    std::vector<Platform*> platforms;
//    glm::vec3 objectBottomPosition;
    bool coordinateLaysIn(glm::vec3 co, glm::vec3 from, glm::vec3 to);
public:
//    CollisionDetector();
    void addPlatform(Platform* platform);
    bool collision(glm::vec3 objectBottomPosition);
//    ~CollisionDetector();
};
