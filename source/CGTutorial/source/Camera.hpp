#pragma once
//vec n
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include <math.h>
#include "Util.hpp"

//keeps track of where to look
class Camera
{
    bool thirdPerson;
public:
    float distanceFromPlayer;
    
    glm::vec3 relativePositionToPlayer;
    glm::vec3 position;
    glm::vec3 lookAtAbsolute;
    
    Camera();
    void update(glm::vec3 position, glm::vec3 lookAtRelative);
    void switchViewMode();
    bool getThirdPersonMode();
    ~Camera();
};
