#include "Camera.hpp"
#include <iostream>


Camera::Camera()
{
    thirdPerson = true;
    distanceFromPlayer = 7;
    relativePositionToPlayer = glm::vec3(0, 0, 0);
}

void Camera::update(glm::vec3 playerPosition, glm::vec3 lookAtRelative_Player) {
    if (thirdPerson){
        this->position = playerPosition - distanceFromPlayer*lookAtRelative_Player + relativePositionToPlayer;
        lookAtAbsolute = playerPosition;
    } else {
        this->position = playerPosition;
        lookAtAbsolute = playerPosition + lookAtRelative_Player;
    }
}

void Camera::switchViewMode() {
    thirdPerson = !thirdPerson;
}

bool Camera::getThirdPersonMode() {
    return thirdPerson;
}

Camera::~Camera()
{
}
