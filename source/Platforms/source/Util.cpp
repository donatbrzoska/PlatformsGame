#include "Util.hpp"

float Util::degreesToRadians(float d) { return d * (3.1415/180.); }

glm::vec3 Util::newPoint(glm::vec3 currentPoint, float minDistH, float maxDistH, float minDistV, float maxDistV, bool randomDirection){
    float new_x = random(minDistH, maxDistH);
    
    float ranN = 1;
    if (randomDirection & (random(-1,1)>0)){
        ranN = -1;
    }
    float new_z = ranN*random(minDistH, maxDistH);
    
    float new_y = random(minDistV, maxDistV);
    return glm::vec3(currentPoint.x + new_x, currentPoint.y + new_y, currentPoint.z + new_z);
}

//this only works, as long as you have some time passed between the random calls
float Util::random(float from, float to) {
    auto time = std::chrono::high_resolution_clock::now();
    float d = to-from;
    
    long l = time.time_since_epoch().count()%10;
    float f = l/10.f;
    
    return from+f*d;
}

glm::mat4 Util::custom_rotate(glm::mat4 m, float angle, glm::vec3 n){
#ifdef __APPLE__ || __MACH__
    float newAngle = degreesToRadians(angle);
#else
    float newAngle = angle;
#endif
    return glm::rotate(m, newAngle, n);
}

void Util::soundTask(std::string path){
    system(("afplay '" + path + "'").c_str());
}
void Util::playSound(std::string path){
#ifdef __APPLE__ || __MACH__
    std::thread soundThread = std::thread(&Util::soundTask, path);
    soundThread.detach();
#else
    std::cout << "no operating system other than macOS supports sound currently" << std::endl;
#endif
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
