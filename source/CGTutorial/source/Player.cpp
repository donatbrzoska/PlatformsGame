#include "Player.hpp"

Player::Player()
{
    position = glm::vec3(0, 0, -5);
    relativeBottomPosition = glm::vec3(0, 1.f/5.f, 0);
    
    jumpHeight = 8;
    inAir = false;
    
    lookAtRelative = glm::vec3(0, 0, 1);
    lookAtAbsolute = position + lookAtRelative;
    
    //THE LOWER THE SLEEP TIME, THE HIGHER THE SPEED
    moveSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(1));
    jumpSleepTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::milliseconds(3));
    
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

void Player::setStepSize(float stepSize){
    this->stepSize = stepSize;
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

float function3(float x, float jumpHeight){
    //    return -0.5*std::pow(x, 2) + jumpHeight;
    return -(jumpHeight/8)*std::pow(x, 2) + jumpHeight;
}

void Player::jump_up(){
    glm::vec3 start = position;
    int iterations = 30*jumpHeight/2;
    float nst = std::sqrt(jumpHeight/(jumpHeight/8));
    //    float lastPosition = 0;
    for (int i=0; i<=iterations; i++){
        //        Util::print(function3(-nst + i*(nst)/iterations, jumpHeight));
        position.y = start.y + function3(-nst + i*(nst)/iterations, jumpHeight);
        //        float newPosition = function3(-nst + i*(nst)/iterations, jumpHeight);
        //        float distance = newPosition - lastPosition;
        //
        //        float madeDistance = 0;
        //        while (!collisionDetector->collision(bottomPosition()) & madeDistance<distance) {
        //            position.y = position.y+0.01;
        //            madeDistance = madeDistance+0.01;
        //        }
        //        lastPosition = lastPosition+distance;
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
    for (i=0; i<=iterations & !collision & position.y>-20 /*this last condition is for the checkFall call*/; i++){
        //        position.y = start.y + function3(i*(nst)/iterations, jumpHeight);
        float newRelativePosition = function3(i*(nst)/iterations, jumpHeight);
        float distanceToDo = lastRelativePosition-newRelativePosition;
        
        float distanceMade = 0;
        while (distanceMade<distanceToDo) {
            if (!collisionDetector->collision(bottomPosition())){
                position.y = position.y-0.01;
                distanceMade = distanceMade+0.01;
            } else {
                position.y = position.y+0.01;
                collision = true;
                Platform::setNewPlatformReady(true);
                break;
            }
        }
        lastRelativePosition = lastRelativePosition-distanceMade;
        updateAndSleep(jumpSleepTime);
    }
    
    //if no collision occurred, the player keeps falling
    if (!collision){
        for (i=i; !collision & position.y > -20; i++){
            float newRelativePosition = function3(i*(nst)/iterations, jumpHeight);
            float distanceToDo = lastRelativePosition-newRelativePosition;
            
            float distanceMade = 0;
            while (distanceMade<distanceToDo) {
                if (!collisionDetector->collision(bottomPosition())){
                    position.y = position.y-0.01;
                    distanceMade = distanceMade+0.01;
                } else {
                    position.y = position.y+0.01;
                    collision = true;
                    Platform::setNewPlatformReady(true);
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
    if(!inAir) {
        position.y = position.y - 0.05;
        if (!collisionDetector->collision(bottomPosition())){
            position.y = position.y+0.05;
            
//            glm::vec3 start = position;
//            int iterations = 30*jumpHeight;
//            float nst = std::sqrt(jumpHeight/(jumpHeight/8));
//            std::thread f = std::thread(&Player::jump_fall, this, start, nst, iterations/2);
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
//    while(moveMap["forward"]->executeMove) {
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
        
//        puppet->moveRight();
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
        checkFall();
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
//    Util::print("something happens");
}
Player::~Player()
{
    //delete(camera);
}

