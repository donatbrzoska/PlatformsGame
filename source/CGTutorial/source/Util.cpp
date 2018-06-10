#include "Util.hpp"

float Util::degreesToRadians(float d) { return d * (3.1415/180.); }

glm::mat4 Util::custom_rotate(glm::mat4 m, float angle, glm::vec3 n){
    float newAngle = degreesToRadians(angle);
    return glm::rotate(m, newAngle, n);
}

void Util::print(std::string s, glm::vec3 v) {
    std::cout << s << v.x << ", " << v.y << ", " << v.z << std::endl;
}

void Util::print(std::string s) {
    std::cout << s << std::endl;
}
void Util::print(int i) {
    std::cout << i << std::endl;
}
void Util::print(double f) {
    std::cout << f << std::endl;
}

void Util::drawCS(glm::mat4 Model) {
    
    double lineWidth = 0.007f;
    //    double lineLength = 0.8f;
    double lineLength = 2.f;
    
    glm::mat4 Save = Model;
    Model = glm::scale(Model, glm::vec3(lineLength, lineWidth, lineWidth));
    //    sendMVP();
    MVP::setModel(Model);
    drawCube();
    
    Model = Save;
    Model = glm::scale(Model, glm::vec3(lineWidth, lineLength, lineWidth));
    //    sendMVP();
    MVP::setModel(Model);
    drawCube();
    
    Model = Save;
    Model = glm::scale(Model, glm::vec3(lineWidth, lineWidth, lineLength));
    //    sendMVP();
    MVP::setModel(Model);
    drawCube();
    Model = Save;
}
