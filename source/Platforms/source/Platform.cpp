#include "Platform.hpp"

void Platform::determineFromTo(){
    from = position - measures;
    to = position + measures;
}

Platform::Platform(glm::vec3 position, glm::vec3 measures){
    this->position = position;
    this->measures = measures;
    determineFromTo();
}

//just for debugging purposes
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
//    Util::drawCS(Model);
    //    sendMVP();
    MVP::setModel(Model);
    drawCube();
}

Platform::~Platform(){}
