#include "Puppet.hpp"

Puppet::Puppet() {
    alpha = 180.0f;
    animationend = 15.0f;
    armwinkel = 0.0f;
    i = 0;
    beta = 180.0f;
    speed = 2.0f;
    speed2 = speed;
    jump = 0.0f;
    z_armwinkel = 0.0f;
 
    x_pos_human = 0.0f;
    y_pos_human = 0.0f;
    z_pos_human = 0.0f;
    inair = false;
    
    Model = glm::mat4(1.0f);
}

void Puppet::setStraight() {
    alpha = 180.0f;
    armwinkel = 40.0f;
    i = 0;
    beta = 180.0f;
    z_armwinkel = 0.0f;
}


void Puppet::moveForward(){
    z_pos_human += 0.2;
    z_armwinkel = 0;
    if (i == animationend) {
        speed *= -1;
        i = -animationend;
        speed2 *=-1;
    }
    if (armwinkel == 0) {
        speed2 *= -1;
    }
    armwinkel += speed2;
    beta -= speed;
    alpha += speed;
    i++;
}

void Puppet::moveBackward(){
    z_armwinkel = 0;
    if (i == animationend) {
        speed *= -1;
        i = -animationend;
        speed2 *=-1;
    }
    if (armwinkel == 0) {
        speed2 *= -1;
    }
    z_pos_human -= 0.2;
    armwinkel += speed2;
    beta -= speed;
    alpha += speed;
    i++;
}

void Puppet::moveLeft(){
    x_pos_human -= 0.2;
    if (i == animationend) {
        speed *= -1;
        i = -animationend;
    }
    armwinkel += speed/2;
    z_armwinkel += speed / 2;
    beta -= speed;
    alpha += speed;
    i++;
}

void Puppet::moveRight(){
    if (i == animationend) {
        speed *= -1;
        i = -animationend;
    }
    x_pos_human += 0.2;
    armwinkel += speed/2;
    z_armwinkel += speed / 2;
    beta -= speed;
    alpha += speed;
    i++;
}

void Puppet::setInAirMode(bool mode) {
//    if (inair == false) {
//        inair = true;
//    }
    inair = mode;
}

void Puppet::update(glm::vec3 position, float horizontalRotation) {
    this->position = position;
    this->horizontalRotation = horizontalRotation;
}
