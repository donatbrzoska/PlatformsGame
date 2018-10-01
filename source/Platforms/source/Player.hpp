#pragma once
#include <string>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.hpp"
#include "CollisionDetector.hpp"
#include "Puppet.hpp"
#include "Platform.hpp"
#include "Resources.hpp"

#include <thread>
#include <math.h>
#include <chrono>

#include <mutex>

//keeps track of all data concerning the player (position, speed, speedMode, ...)
//invokes puppet to animate
//invokes updates on camera
//moves player
class Player
{
    Camera* camera;
    Puppet* puppet;
    CollisionDetector* collisionDetector;
    glm::vec3 relativeBottomPosition;
    
    bool superSpeed;
    float stepSize;
    float jumpHeight;
    bool inAir;
    
    float horizontalRotation;
    float additionalHorizontalRotation;
    float verticalRotation;
    
    glm::vec3 lookAtRelative;
    glm::vec3 lookAtAbsolute;
    
    std::chrono::nanoseconds moveSleepTime;
    std::chrono::nanoseconds jumpSleepTime;
    
    void updateAndSleep(std::chrono::nanoseconds t);
    
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
    
    void checkFall();
    
    std::thread jumpThread;
    void jump_fall();
    void jump_up();
    void jumpTask();
    
    glm::vec3 bottomPosition();
    void updateLookAt();
    void updateCamera();
    void updatePuppet();
    void updatePuppet(float a);
    
    
    
    //SIMPLE METHODS FROM OLD VERSION MAKES US ABLE TO FLY
    std::thread moveForwardThread_;
    bool executeMoveForward_;
    void moveForwardTask_();
    
    std::thread moveBackwardThread_;
    bool executeMoveBackward_;
    void moveBackwardTask_();
    
    std::thread moveRightThread_;
    bool executeMoveRight_;
    void moveRightTask_();
    
    std::thread moveLeftThread_;
    bool executeMoveLeft_;
    void moveLeftTask_();
    
    std::thread moveUpThread_;
    bool executeMoveUp_;
    void moveUpTask_();
    
    std::thread moveDownThread_;
    bool executeMoveDown_;
    void moveDownTask_();
    
    
public:
    glm::vec3 position;
    
    Player();
    void setCamera(Camera* camera);
    void setPuppet(Puppet* puppet);
    void setCollisionDetector(CollisionDetector* collisionDetector);
    void setPosition(glm::vec3 position);
    void setRelativeBottomPosition(glm::vec3 position);
    void setStepSize(float stepSize);
    
    glm::vec3 getLookAtRelativeH();
    
    void turn(std::string command, float angle);
    
    void moveForward(bool mode);
    void moveBackward(bool mode);
    void moveRight(bool mode);
    void moveLeft(bool mode);
    
    void jump();
    
    void setSuperSpeedMode(bool mode);
    bool getSuperSpeedMode();
    
    
    
    void moveForward_(bool mode);
    void moveBackward_(bool mode);
    void moveRight_(bool mode);
    void moveLeft_(bool mode);
    void moveUp_(bool mode);
    void moveDown_(bool mode);
    ~Player();
};
