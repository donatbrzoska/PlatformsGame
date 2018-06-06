#include "Platform.hpp"

//static CollisionDetector* collisionDetector;
//
//void Platform::initialize(CollisionDetector *collisionDetector){
//    ::collisionDetector=collisionDetector;
//}

Platform::Platform(glm::vec3 position){
    this->position = position;
//    collisionDetector->addPlatform(this);
    determineFromTo();
}

void Platform::determineFromTo(){
    glm::vec3 halfMeasures = glm::vec3(measures.x/2, measures.y/2, measures.z/2);
    from = position - halfMeasures;
    to = position + halfMeasures;
}
