#include "Player.hpp"

static std::mutex _mutex;

Player::Player()
{
    position = glm::vec3(0, 0, -5);
    relativeBottomPosition = glm::vec3(0, 1.f/5.f, 0);
    inAir = false;
    stepSize = 0.02;
    
    lookAtRelative = glm::vec3(0, 0, 1);
    lookAtAbsolute = position + lookAtRelative;
    
     //looking to the front
    horizontalRotation = 90;
    additionalHorizontalRotation = 0;
    verticalRotation = 90;
    
    setSuperSpeedMode(false);
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

void Player::setStepSize(float stepSize){
    this->stepSize = stepSize;
}

void Player::setSuperSpeedMode(bool mode){
    if (mode) {
        jumpHeight = 16;
        moveSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(500000));
        jumpSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(2000000));
        superSpeed = true;
    } else {
        jumpHeight = 8;
        moveSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(1000000));
        jumpSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::nanoseconds(3000000));
        superSpeed = false;
    }
}

bool Player::getSuperSpeedMode() {
    return superSpeed;
}

glm::vec3 Player::bottomPosition(){
    return position - relativeBottomPosition;
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

float jumpFunction(float x, float jumpHeight){
    return -(jumpHeight/8)*std::pow(x, 2) + jumpHeight;
}

void Player::jump_up(){
    glm::vec3 start = position;
    int iterations = 30*jumpHeight/2;
    float nst = std::sqrt(jumpHeight/(jumpHeight/8));
    for (int i=0; i<=iterations; i++){
        position.y = start.y + jumpFunction(-nst + i*(nst)/iterations, jumpHeight);
        updateAndSleep(jumpSleepTime);
    }
}

void Player::jump_fall(){
    glm::vec3 start = position;
    int iterations = 30*jumpHeight/2;
    float nst = std::sqrt(jumpHeight/(jumpHeight/8));
    
    //in every iteration the distance that has to be done this iteration is calculated and then made in small steps within the while loop
    float lastRelativePosition = jumpHeight;
    bool collision = false;
    
    int i;
    for (i=0; (i<=iterations) & !collision & position.y>-20 /*this last condition is for the checkFall call*/; i++){
        //        position.y = start.y + function3(i*(nst)/iterations, jumpHeight);
        float newRelativePosition = jumpFunction(i*(nst)/iterations, jumpHeight);
        float distanceToDo = lastRelativePosition-newRelativePosition;
        
        float distanceMade = 0;
        while (distanceMade<distanceToDo) {
            if (!collisionDetector->collision(bottomPosition())){
                position.y = position.y-0.01;
                distanceMade = distanceMade+0.01;
            } else {
                position.y = position.y+0.01;
                collision = true;
                break;
            }
        }
        lastRelativePosition = lastRelativePosition-distanceMade;
        updateAndSleep(jumpSleepTime);
    }
    
    //if no collision occurred, the player keeps falling
    if (!collision){
        for (i=i; !collision & (position.y > -20); i++){
            float newRelativePosition = jumpFunction(i*(nst)/iterations, jumpHeight);
            float distanceToDo = lastRelativePosition-newRelativePosition;
            
            float distanceMade = 0;
            while (distanceMade<distanceToDo) {
                if (!collisionDetector->collision(bottomPosition())){
                    position.y = position.y-0.01;
                    distanceMade = distanceMade+0.01;
                } else {
                    position.y = position.y+0.01;
                    collision = true;
                    break;
                }
            }
            lastRelativePosition = lastRelativePosition-distanceMade;
            updateAndSleep(jumpSleepTime);
        }
    }
    inAir = false;
}

void Player::jumpTask(){
    puppet->inAir = true;
    jump_up();
    jump_fall();
    puppet->inAir = false;
}

void Player::jump(){
    if (!inAir) {
        inAir = true;
        jumpThread = std::thread(&Player::jumpTask, this);
        jumpThread.detach();
    }
}

void Player::checkFall(){
    //this is required, else too many threads can access this and cause the puppet to sink in the platform
    std::unique_lock<std::mutex> lock(_mutex);
    
    if(!inAir) {
        position.y = position.y - 0.05;
        if (!collisionDetector->collision(bottomPosition())){
            position.y = position.y + 0.05;
            inAir = true;
            std::thread f = std::thread(&Player::jump_fall, this);
            f.detach();
        } else {
            position.y = position.y + 0.05;
        }
    }
}

void Player::moveForwardTask(){
    while(executeMoveForward) {
        glm::vec3 lookAtRelativeH = glm::vec3(-cos(Util::degreesToRadians(horizontalRotation)), 0, sin(Util::degreesToRadians(horizontalRotation)));
        position += stepSize*lookAtRelativeH;
            
        puppet->moveForward();
        checkFall();
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
        checkFall();
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
        
        additionalHorizontalRotation = -45;
        puppet->moveForward();
        checkFall();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
    additionalHorizontalRotation = 0;
}
void Player::moveRight(bool mode){
    if (mode==true) {
        if (!executeMoveRight /*& !executeMoveLeft*/) {
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
        
        additionalHorizontalRotation = 45;
        puppet->moveForward();
        checkFall();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
    additionalHorizontalRotation = 0;
}
void Player::moveLeft(bool mode){
    if (mode==true) {
        if (!executeMoveLeft /*&!executeMoveRight*/) {
            executeMoveLeft=true;
            moveLeftThread = std::thread(&Player::moveLeftTask, this);
            moveLeftThread.detach();
        }
    } else {
        executeMoveLeft=false;
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
    puppet->update(position, horizontalRotation+additionalHorizontalRotation);
}



//SIMPLE METHODS FROM OLD VERSION MAKES US ABLE TO FLY

void Player::moveForwardTask_(){
    while(executeMoveForward_) {
        glm::vec3 lookAtRelativeH = glm::vec3(-cos(Util::degreesToRadians(horizontalRotation)), 0, sin(Util::degreesToRadians(horizontalRotation)));
        position += stepSize*lookAtRelativeH;
        
        puppet->moveForward();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
}
void Player::moveForward_(bool mode){
    if (mode==true) {
        if (!executeMoveForward_) {
            executeMoveForward_=true;
            moveForwardThread_ = std::thread(&Player::moveForwardTask_, this);
            moveForwardThread_.detach();
        }
    } else {
        executeMoveForward_=false;
    }
}

void Player::moveBackwardTask_(){
    while(executeMoveBackward_) {
        glm::vec3 lookAtRelativeH = glm::vec3(-cos(Util::degreesToRadians(horizontalRotation)), 0, sin(Util::degreesToRadians(horizontalRotation)));
        position -= stepSize*lookAtRelativeH;
        
        puppet->moveForward();
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
}
void Player::moveBackward_(bool mode){
    if (mode==true) {
        if (!executeMoveBackward_) {
            executeMoveBackward_=true;
            moveBackwardThread_ = std::thread(&Player::moveBackwardTask_, this);
            moveBackwardThread_.detach();
        }
    } else {
        executeMoveBackward_=false;
    }
}

void Player::moveRightTask_(){
    while(executeMoveRight_) {
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
void Player::moveRight_(bool mode){
    if (mode==true) {
        if (!executeMoveRight_ /*& !executeMoveLeft*/) {
            executeMoveRight_=true;
            moveRightThread_ = std::thread(&Player::moveRightTask_, this);
            moveRightThread_.detach();
        }
    } else {
        executeMoveRight_=false;
    }
}

void Player::moveLeftTask_(){
    while(executeMoveLeft_) {
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
void Player::moveLeft_(bool mode){
    if (mode==true) {
        if (!executeMoveLeft_ /*&!executeMoveRight*/) {
            executeMoveLeft_=true;
            moveLeftThread_ = std::thread(&Player::moveLeftTask_, this);
            moveLeftThread_.detach();
        }
    } else {
        executeMoveLeft_=false;
    }
}
void Player::moveUpTask_(){
    while(executeMoveUp_) {
        position.y += stepSize;
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
}
void Player::moveUp_(bool mode){
    if (mode==true) {
        if (!executeMoveUp_) {
            executeMoveUp_=true;
            moveUpThread_ = std::thread(&Player::moveUpTask_, this);
            moveUpThread_.detach();
        }
    } else {
        executeMoveUp_=false;
    }
}

void Player::moveDownTask_(){
    while(executeMoveDown_) {
        position.y -= stepSize;
        updateAndSleep(moveSleepTime);
    }
    puppet->setStraight();
}
void Player::moveDown_(bool mode){
    if (mode==true) {
        if (!executeMoveDown_) {
            executeMoveDown_=true;
            moveDownThread_ = std::thread(&Player::moveDownTask_, this);
            moveDownThread_.detach();
        }
    } else {
        executeMoveDown_=false;
    }
}
Player::~Player()
{
    //delete(camera);
}

