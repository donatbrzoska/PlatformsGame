#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "CollisionDetector.hpp"
#include "Puppet.hpp"
#include "Platform.hpp"

//#include <math.h>

#include <thread>
#include <math.h>
#include <chrono>
//#include <map>

class Player
{
    Camera* camera;
    Puppet* puppet;
    CollisionDetector* collisionDetector;
    glm::vec3 relativeBottomPosition;
    
    bool superSpeed;
    float stepSize;
    float jumpHeight;
//    float jumpSpeed;
    bool inAir;
    
    float horizontalRotation;
    float additionalHorizontalRotation;
    float verticalRotation;
    
    glm::vec3 lookAtRelative;
    glm::vec3 lookAtAbsolute;
    
    std::chrono::nanoseconds moveSleepTime;
    std::chrono::nanoseconds jumpSleepTime;
    
    void updateAndSleep(std::chrono::nanoseconds t);
    
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
    
    
    void checkFall();
    
    std::thread jumpThread;
    void jump_fall();
    void jump_up();
    void jumpTask();
    
//    std::thread fallThread;
//    void fallTask();
//    void fall();
    
    glm::vec3 bottomPosition();
    void updateLookAt();
    void updateCamera();
    void updatePuppet();
    void updatePuppet(float a);
    
public:
    glm::vec3 position;
    
    Player();
//    void initialize();
    void setCamera(Camera* camera);
    void setPuppet(Puppet* puppet);
    void setCollisionDetector(CollisionDetector* collisionDetector);
    void setPosition(glm::vec3 position);
    void setRelativeBottomPosition(glm::vec3 position);
    void setStepSize(float stepSize);
    
    
    void turn(std::string command, float angle);
    
    void moveForward(bool mode);
    void moveBackward(bool mode);
    void moveRight(bool mode);
    void moveLeft(bool mode);
    void moveUp(bool mode);
    void moveDown(bool mode);
    
    void jump();
    
    void setSuperSpeedMode(bool mode);
    bool getSuperSpeedMode();
    
//    void move(std::string cmd, bool mode);
    
    ~Player();
};
