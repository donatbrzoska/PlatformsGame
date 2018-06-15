#include "Player.hpp"

Player::Player()
{
    position = glm::vec3(0, 0, -5);
    relativeBottomPosition = glm::vec3(0, 1.f/5.f, 0);
    
    jumpHeight = 4;
    jumping = false;
    
    lookAtRelative = glm::vec3(0, 0, 1);
    lookAtAbsolute = position + lookAtRelative;
    
    //THE LOWER THE SLEEP TIME, THE HIGHER THE SPEED
    moveSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(1));
    jumpSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(8));
    
    stepSize = 0.02;
    
    horizontalRotation = 90; //looking to the front
    additionalHorizontalRotation = 0;
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

void Player::setRelativeBottomPosition(glm::vec3 position) {
    this->relativeBottomPosition = position;
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

void Player::updateAndSleep(std::chrono::nanoseconds t){
    updateLookAt();
    updateCamera();
    updatePuppet();
    std::this_thread::sleep_for(t);
}

void Player::moveForwardTask(){
    while(executeMoveForward) {
//    while(moveMap["forward"]->executeMove) {
        glm::vec3 lookAtRelativeH = glm::vec3(-cos(Util::degreesToRadians(horizontalRotation)), 0, sin(Util::degreesToRadians(horizontalRotation)));
        position += stepSize*lookAtRelativeH;
            
        puppet->moveForward();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
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
        
        puppet->moveForward();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
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
        glm::vec3 direction = glm::normalize(glm::cross(lookAtRelative, glm::vec3(0, 1, 0)));
        position += stepSize*direction;
        
//        puppet->moveRight();
        additionalHorizontalRotation = -45;
        puppet->moveForward();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
    additionalHorizontalRotation = 0;
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
        glm::vec3 direction = glm::normalize(glm::cross(glm::vec3(0, 1, 0), lookAtRelative));
        position += stepSize*direction;
        
//        puppet->moveLeft();
        additionalHorizontalRotation = 45;
        puppet->moveForward();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
    additionalHorizontalRotation = 0;
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
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
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
        if (collisionDetector->collision(bottomPosition())){
            position.y +=stepSize;
            break;
        }
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
}
void Player::moveDown(bool mode){
    if (mode==true) {
        if (!executeMoveDown) {
            executeMoveDown=true;
            moveDownThread = std::thread(&Player::moveDownTask, this);
            moveDownThread.detach();
        }
    } else {
        executeMoveDown=false;
    }
}

float function(float x){
//    return (5 - std::exp(x - 5));
    if (x<0)
        return std::exp(-x+0.5)+20;
    else
        return (-std::exp(x+0.5)+20);
}

float function2(float x) {
    if (x<=0)
        return -std::pow(2*x,2);
    else
        return std::pow(2*x,2);
}

float function3(float x, float jumpHeight){
    return -0.5*std::pow(x, 2) + jumpHeight;
}

void Player::jumpTask(){
    glm::vec3 start = position;
//    int iterations = 60;
    int iterations = 30*jumpHeight;
    float nst = std::sqrt(2*jumpHeight);
    
    int sleepTime = 8;
    jumpSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(sleepTime));
    
    int i;
    for (i=0; i<=iterations+1; i++) {       //plus 1 because afterwards we need to check, whether the ground is hit
        position.y = start.y + function3(-nst + i*(2*nst)/iterations, jumpHeight);
        
        if (collisionDetector->collision(bottomPosition())){    //if a collision occures during the jump, the position.y will be corrected up and the jump is done
            while (collisionDetector->collision(bottomPosition())){
                position.y = position.y + 0.01;
            }
            break;
        }
        
        updateAndSleep(jumpSleepTime);
    }
    //at this point, i is equal to (iterations + 2)
    
    //if there is no collision after the fall (which happens if we jump down to another platform), the player will just go on falling
    if (!collisionDetector->collision(bottomPosition())){
        bool noCollision = true;
        while (noCollision){
            position.y = start.y + function3(-nst + i*(2*nst)/iterations, jumpHeight);
            if (collisionDetector->collision(bottomPosition())) {   //if the collisionpoint is reached, the position.y is incremented to the point where no collision occures anymore
                while (collisionDetector->collision(bottomPosition())){
                    position.y = position.y + 0.01;
                }
                noCollision = false;
            }
//            Util::print(i);
            i++;
            updateAndSleep(jumpSleepTime);
        }
    } else {    //this happens when the y is the same like before the jump
//        position.y = start.y + function3(-nst + iterations*(2*nst)/iterations, jumpHeight);
        position.y = start.y;
        updateAndSleep(jumpSleepTime);
    }
    jumping = false;
}

void Player::jump(){
    if (!jumping) {
        jumping = true;
        jumpThread = std::thread(&Player::jumpTask, this);
        jumpThread.detach();
    }
}

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
    puppet->update(position, horizontalRotation+ additionalHorizontalRotation);
//    Util::print("something happens");
}
Player::~Player()
{
    //delete(camera);
}

