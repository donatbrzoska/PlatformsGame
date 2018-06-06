#include "Player.hpp"

Player::Player()
{
    position = glm::vec3(0, 0, -5);
    relativeBottomPosition = glm::vec3(0, 0, 0);
    
    lookAtRelative = glm::vec3(0, 0, 1);
    lookAtAbsolute = position + lookAtRelative;
    
    //THE LOWER THE SLEEP TIME, THE HIGHER THE SPEED
    sleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(2));
    
    stepSize = 0.02;
    
    horizontalRotation = 90; //looking to the front
    verticalRotation = 90;
}

void Player::setCamera(Camera* camera) {
    this->camera = camera;
}

void Player::setPuppet(Puppet *puppet) {
    this->puppet = puppet;
}

void Player::setCollisionDetector(CollisionDetector *collisionDetector) {
    this->collisionDetector = collisionDetector;
}

void Player::setPosition(glm::vec3 position) {
    this->position = position;
}

glm::vec3 Player::bottomPosition(){
    return position - relativeBottomPosition;
}

void Player::setStepSize(float stepSize){
    this->stepSize = stepSize;
}

void Player::turn(std::string command, float angle) {
    if (command=="h") {
        horizontalRotation += angle;
    }
    if (command=="v") {
        verticalRotation -= angle;
        if (verticalRotation <= 0 | verticalRotation >= 180) {
            verticalRotation += angle;
        }
    }
    updateLookAt();
    updateCamera();
    updatePuppet();
}

void Player::updateAndSleep(){
    updateLookAt();
    updateCamera();
    updatePuppet();
    std::this_thread::sleep_for(sleepTime);
}

void Player::moveForwardTask(){
        while(executeMoveForward) {
//    while(moveMap["forward"]->executeMove) {
        glm::vec3 lookAtRelativeH = glm::vec3(-cos(Util::degreesToRadians(horizontalRotation)), 0, sin(Util::degreesToRadians(horizontalRotation)));
        position += stepSize*lookAtRelativeH;
        updateAndSleep();
    }
}
void Player::moveForward(bool mode){
    if (mode==true) {
        if (!executeMoveForward) {
            executeMoveForward=true;
            moveForwardThread = std::thread(&Player::moveForwardTask, this);
            moveForwardThread.detach();
        }
    } else {
        executeMoveForward=false;
    }
}

void Player::moveBackwardTask(){
    while(executeMoveBackward) {
        glm::vec3 lookAtRelativeH = glm::vec3(-cos(Util::degreesToRadians(horizontalRotation)), 0, sin(Util::degreesToRadians(horizontalRotation)));
        position -= stepSize*lookAtRelativeH;
        updateAndSleep();
    }
}
void Player::moveBackward(bool mode){
    if (mode==true) {
        if (!executeMoveBackward) {
            executeMoveBackward=true;
            moveBackwardThread = std::thread(&Player::moveBackwardTask, this);
            moveBackwardThread.detach();
        }
    } else {
        executeMoveBackward=false;
    }
}

void Player::moveRightTask(){
    while(executeMoveRight) {
        glm::vec3 direction = glm::cross(lookAtRelative, glm::vec3(0, 1, 0));
        position += stepSize*direction;
        updateAndSleep();
    }
}
void Player::moveRight(bool mode){
    if (mode==true) {
        if (!executeMoveRight) {
            executeMoveRight=true;
            moveRightThread = std::thread(&Player::moveRightTask, this);
            moveRightThread.detach();
        }
    } else {
        executeMoveRight=false;
    }
}

void Player::moveLeftTask(){
    while(executeMoveLeft) {
        glm::vec3 direction = glm::cross(glm::vec3(0, 1, 0), lookAtRelative);
        position += stepSize*direction;
        updateAndSleep();
    }
}
void Player::moveLeft(bool mode){
    if (mode==true) {
        if (!executeMoveLeft) {
            executeMoveLeft=true;
            moveLeftThread = std::thread(&Player::moveLeftTask, this);
            moveLeftThread.detach();
        }
    } else {
        executeMoveLeft=false;
    }
}

void Player::moveUpTask(){
    while(executeMoveUp) {
        position.y += stepSize;
        updateAndSleep();
    }
}
void Player::moveUp(bool mode){
    if (mode==true) {
        if (!executeMoveUp) {
            executeMoveUp=true;
            moveUpThread = std::thread(&Player::moveUpTask, this);
            moveUpThread.detach();
        }
    } else {
        executeMoveUp=false;
    }
}

void Player::moveDownTask(){
    while(executeMoveDown) {
        position.y -= stepSize;
        updateAndSleep();
        if (collisionDetector->collision(bottomPosition())){
            position.y +=stepSize;
            break;
        }
    }
}
void Player::moveDown(bool mode){
    if (mode==true) {
        if (!executeMoveDown) {
            executeMoveDown=true;
//            moveOperation o = &Player::moveDownTask;
//            moveDownThread = std::thread(o, this);
            moveDownThread = std::thread(&Player::moveDownTask, this);
            moveDownThread.detach();
        }
    } else {
        executeMoveDown=false;
    }
}

//void Player::initialize(){
//    struct move* forward = new struct move;
//    forward->executeMove = false;
//    forward->task = &Player::moveForwardTask;
//    moveMap["forward"] = forward;
//}
//
//void Player::move(std::string cmd, bool mode){
//    struct move* m = moveMap[cmd];
//    if (mode == true) {
//        if (!m->executeMove) {
//            m->executeMove=true;
//            m->moveThread = std::thread(m->task, this);
//            m->moveThread.detach();
//        }
//    } else {
//        m->executeMove=false;
//    }
//}

void Player::updateLookAt() {
    //nach https://de.wikipedia.org/wiki/Kugelkoordinaten
    lookAtRelative.x = -sin(Util::degreesToRadians(verticalRotation)) * cos(Util::degreesToRadians(horizontalRotation));
    lookAtRelative.y = cos(Util::degreesToRadians(verticalRotation));
    lookAtRelative.z = sin(Util::degreesToRadians(verticalRotation)) * sin(Util::degreesToRadians(horizontalRotation));
    
    lookAtAbsolute.x = position.x + lookAtRelative.x;
    lookAtAbsolute.y = position.y + lookAtRelative.y;
    lookAtAbsolute.z = position.z + lookAtRelative.z;
}

void Player::updateCamera() {
    camera->update(position, lookAtRelative);
}

void Player::updatePuppet() {
    puppet->update(position, horizontalRotation);
}

Player::~Player()
{
    //delete(camera);
}

