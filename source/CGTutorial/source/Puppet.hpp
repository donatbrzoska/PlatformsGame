#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include "MVP.hpp"
#include "objects.hpp"
#include "Util.hpp"

class Puppet {
    glm::vec3 position;
    float horizontalRotation;
    
    
    glm::mat4 Model;
    
    void drawPart(double h, double s);
    void drawPart(double h, double s, double b);
    
public:
    float alpha;
    float animationend;
    float armwinkel;
    float i;
    float beta;
    float speed;
    float speed2;
    float jump;
    float z_armwinkel;
    
    float x_pos_human;
    float y_pos_human;
    float z_pos_human;
    bool inair;
    
    Puppet();
    
    void update(glm::vec3 position, float horizontalRotation);
    void drawPuppet();
    
    void setStraight();
    void moveLeft();
    void moveRight();
    void moveForward();
    void moveBackward();
    void setInAirMode(bool mode);
    ~Puppet();
};
