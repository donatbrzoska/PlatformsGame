#include "Puppet.hpp"

Puppet::Puppet() {
    alpha = 180.0f;
    animationend = 150.0f;
    armwinkel = 0.0f;
    i = 0;
    beta = 180.0f;
    speed = 0.25f;
    speed2 = speed;
    jump = 0.0f;
    z_armwinkel = 0.0f;
    
    inAir = false;
}

void Puppet::setStraight() {
    alpha = 180.0f;
    armwinkel = 0.0f;
    i = 0;
    beta = 180.0f;
    z_armwinkel = 0.0f;
    speed = 0.25f;
    speed2 = speed;
}


void Puppet::moveForward(){
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
    armwinkel += speed2;
    beta -= speed;
    alpha += speed;
    i++;
}

void Puppet::moveLeft(){
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
    armwinkel += speed/2;
    z_armwinkel += speed / 2;
    beta -= speed;
    alpha += speed;
    i++;
}

void Puppet::setInAirMode(bool mode) {
    inAir = mode;
}



void Puppet::drawPart(double h, double s) {
    glm::mat4 Save = Model;
    
    Model = glm::translate(Model, glm::vec3(0, h / 2, 0));
    Model = glm::scale(Model, glm::vec3(s, h / 2, s));
    //    sendMVP();
    MVP::setModel(Model);
    drawSphere(10, 10);
    Model = Save;
}

void Puppet::drawPart(double h, double s, double b) {
    glm::mat4 Save = Model;
    
    Model = glm::translate(Model, glm::vec3(b, h / 2, 0));
    Model = glm::scale(Model, glm::vec3(s, h / 2, s));
    //    sendMVP();
    MVP::setModel(Model);
    drawSphere(10, 10);
    Model = Save;
}

void Puppet::drawPuppet(){
    glm::vec3 yVector = glm::vec3(0,1,0);
    
    double fall = 0.001;
    double jumpwinkel = 0.0f;
    
    Model = glm::mat4(1.f);
    glm::mat4 Save = Model;
    
    
    Model = glm::translate(Model, position);
    Model = Util::custom_rotate(Model, horizontalRotation, yVector);
    Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
    
    
    //jumping
    if (/*puppet.*/inAir == true) {
        /*puppet.*/setStraight();
//        /*puppet.*/position.y += 3;
//        /*puppet.*/inair = false;
        jumpwinkel = 180;
    }
    
    //drawing human
    double length = 2.0;
    double ancle = 0.5;
    double thickness = 0.3;
    double gap = 1;
    
    //head
    drawPart(2, 1, 0.5);
    Model = glm::translate(Model, glm::vec3(0.0, -3, 0.0));
    
    //core
    drawPart(3, 1, 0.5);
    Model = glm::translate(Model, glm::vec3(0.0, -1.5, 0.0));
    
    //gap = 1.75;
    
    //becken
    drawPart(1.5, 1.5 / 2, 0.5);
    Model = glm::translate(Model, glm::vec3(0.0, 0.5, 0.0));
    
    
    //left leg
    gap = -0.1;
    Model = Util::custom_rotate(Model, /*puppet.*/alpha, glm::vec3(1.0, 0.0, 0.0));
    Model = Util::custom_rotate(Model, /*puppet.*/z_armwinkel, glm::vec3(0.0, 0.0, 1.0));
    drawPart(ancle, ancle / 2, gap);
    Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
    
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    Model = Util::custom_rotate(Model, -/*puppet.*/armwinkel, glm::vec3(1.0, 0.0, 0.0));
    
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    
    drawPart(ancle, ancle / 2, gap);
    Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
    Model = glm::translate(Model, glm::vec3(0.0, (-ancle - length) * 2, 0));
    Model = Util::custom_rotate(Model, -/*puppet.*/alpha, glm::vec3(1.0, 0.0, 0.0));
    Model = Util::custom_rotate(Model, -/*puppet.*/z_armwinkel, glm::vec3(0.0, 0.0, 1.0));
    
    
    Model = Save;
    Model = glm::translate(Model, position);
    Model = Util::custom_rotate(Model, horizontalRotation, yVector);
    
    Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    
    //right leg
    gap = 1;
    
    Model = Util::custom_rotate(Model, -/*puppet.*/alpha, glm::vec3(1.0, 0.0, 0.0));
    Model = Util::custom_rotate(Model, -/*puppet.*/z_armwinkel, glm::vec3(0.0, 0.0, 1.0));
    
    //.Gelenk
    drawPart(ancle, ancle / 2, gap);
    Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
    
    //.Oberschenkel
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    
    Model = Util::custom_rotate(Model, -/*puppet.*/armwinkel, glm::vec3(1.0, 0.0, 0.0));
    
    //.Schienbein
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    
    //.Fuss
    drawPart(ancle, ancle / 2, gap);
    
    
    
    
    Model = Save;
    Model = glm::translate(Model, position);
    Model = Util::custom_rotate(Model, horizontalRotation, yVector);
    Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
    
    //left arm
    gap = -0.6;
    Model = Util::custom_rotate(Model, jumpwinkel, glm::vec3(1.0, 0.0, 0.0));
    
    Model = Util::custom_rotate(Model, /*puppet.*/beta, glm::vec3(1.0, 0.0, 0.0));
    drawPart(ancle, ancle / 2, gap);
    Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
    
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    Model = Util::custom_rotate(Model, /*puppet.*/armwinkel, glm::vec3(1.0, 0.0, 0.0));
    
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    
    drawPart(ancle, ancle / 2, gap);
    Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
    Model = glm::translate(Model, glm::vec3(0.0, (-ancle - length) * 2, 0));
    Model = Util::custom_rotate(Model, -/*puppet.*/beta, glm::vec3(1.0, 0.0, 0.0));
    
    
    Model = Save;
    Model = glm::translate(Model, position);
    Model = Util::custom_rotate(Model, horizontalRotation, yVector);
    
    Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
    
    //right arm
    gap = 1.6;
    
    Model = Util::custom_rotate(Model, jumpwinkel, glm::vec3(1.0, 0.0, 0.0));
    
    Model = Util::custom_rotate(Model, -/*puppet.*/beta, glm::vec3(1.0, 0.0, 0.0));
    drawPart(ancle, ancle / 2, gap);
    Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
    
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    Model = Util::custom_rotate(Model, /*puppet.*/armwinkel, glm::vec3(1.0, 0.0, 0.0));
    
    drawPart(length, thickness, gap);
    Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
    
    drawPart(ancle, ancle / 2, gap);
}

void Puppet::update(glm::vec3 position, float horizontalRotation) {
    this->position = position;
    this->horizontalRotation = horizontalRotation-90;
}

Puppet::~Puppet(){
    
}
