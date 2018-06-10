#include "Platform.hpp"

//static CollisionDetector* collisionDetector;
//
//void Platform::initialize(CollisionDetector *collisionDetector){
//    ::collisionDetector=collisionDetector;
//}

void Platform::determineFromTo(){
    from = position - measures;
    to = position + measures;
}

Platform::Platform(glm::vec3 position){
    this->position = position;
//    collisionDetector->addPlatform(this);
    determineFromTo();
}

void Platform::drawFromTo(glm::mat4 Model){
        Model = glm::translate(glm::mat4(1.f), from);
        Util::drawCS(Model);
        Model = glm::translate(glm::mat4(1.f), to);
        Util::drawCS(Model);
}

void Platform::draw(){
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, position);
    Model = glm::scale(Model, measures);
    Util::drawCS(Model);
    //    sendMVP();
    MVP::setModel(Model);
    drawCube();
    
    
//    Model = glm::translate(glm::mat4(1.f), position);
//    Util::drawCS(Model);
}

Platform::~Platform(){}


