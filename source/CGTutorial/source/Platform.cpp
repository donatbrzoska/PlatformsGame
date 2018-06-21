#include "Platform.hpp"

//static CollisionDetector* collisionDetector;
//
//void Platform::initialize(CollisionDetector *collisionDetector){
//    ::collisionDetector=collisionDetector;
//}

static bool newPlatformReady = false;

bool Platform::getNewPlatformReady() {
    if (newPlatformReady) {
        newPlatformReady = false;
        return true;
    } else {
        return false;
    }
}

void Platform::setNewPlatformReady(bool rdy){
    newPlatformReady = rdy;
}

void Platform::determineFromTo(){
    from = position - measures;
    to = position + measures;
}

Platform::Platform(glm::vec3 position, glm::vec3 measures){
    this->position = position;
    this->measures = measures;
//    collisionDetector->addPlatform(this);
    determineFromTo();
}

//void Platform::drawFromTo(glm::mat4 Model){
//        Model = glm::translate(glm::mat4(1.f), from);
//        Util::drawCS(Model);
//        Model = glm::translate(glm::mat4(1.f), to);
//        Util::drawCS(Model);
//}

void Platform::draw(){
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, position);
    Model = glm::scale(Model, measures);
//    Util::drawCS(Model);
    //    sendMVP();
    MVP::setModel(Model);
    drawCube();
    
    
//    Model = glm::translate(glm::mat4(1.f), position);
//    Util::drawCS(Model);
}

Platform::~Platform(){}


