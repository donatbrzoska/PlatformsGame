#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "CollisionDetector.hpp"
#include "Puppet.hpp"

//#include <math.h>

#include <thread>
#include <chrono>
#include <map>

class Player
{
    Camera* camera;
    Puppet* puppet;
    CollisionDetector* collisionDetector;
    glm::vec3 relativeBottomPosition;
    
    
    float stepSize;
    float horizontalRotation;
    float verticalRotation;
    
    glm::vec3 lookAtRelative;
    glm::vec3 lookAtAbsolute;
    
    std::chrono::nanoseconds sleepTime;
    
    void updateAndSleep();
    
//    typedef void (Player::*moveOperation)();
    
//    struct move {
//        std::thread moveThread;
//        bool executeMove;
//        moveOperation task;
//    };
//
//    std::map<std::string, move*> moveMap;
    
    std::thread moveForwardThread;
    bool executeMoveForward;
    void moveForwardTask();
    
    std::thread moveBackwardThread;
    bool executeMoveBackward;
    void moveBackwardTask();
    
    std::thread moveRightThread;
    bool executeMoveRight;
    void moveRightTask();
    
    std::thread moveLeftThread;
    bool executeMoveLeft;
    void moveLeftTask();
    
    std::thread moveUpThread;
    bool executeMoveUp;
    void moveUpTask();
    
    std::thread moveDownThread;
    bool executeMoveDown;
    void moveDownTask();
    
    glm::vec3 bottomPosition();
    void updateLookAt();
    void updateCamera();
    void updatePuppet();
    
public:
    glm::vec3 position;
    
    Player();
//    void initialize();
    void setCamera(Camera* camera);
    void setPuppet(Puppet* puppet);
    void setCollisionDetector(CollisionDetector* collisionDetector);
    void setPosition(glm::vec3 position);
    void setStepSize(float stepSize);
    
    
    void turn(std::string command, float angle);
    
    void moveForward(bool mode);
    void moveBackward(bool mode);
    void moveRight(bool mode);
    void moveLeft(bool mode);
    void moveUp(bool mode);
    void moveDown(bool mode);
    
//    void move(std::string cmd, bool mode);
    
    ~Player();
};
