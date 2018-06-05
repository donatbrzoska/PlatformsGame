#include "Util.hpp"

float Util::degreesToRadians(float d) { return d * (3.1415/180); }

glm::mat4 Util::custom_rotate(glm::mat4 m, float angle, glm::vec3 n){
    float newAngle =(6.f/360.f)*angle;
    return glm::rotate(m, newAngle, n);
}

void Util::printVec3WithText(std::string s, glm::vec3 v) {
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


