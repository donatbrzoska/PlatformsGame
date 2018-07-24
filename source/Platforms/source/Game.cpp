#include "Game.hpp"
Puppet Game::puppet;

Camera Game::camera;

CollisionDetector Game::collisionDetector;

Player Game::player;

const glm::vec3 Game::startPosition = glm::vec3(0,5.3,0);
const double Game::playerStepSize = 0.1;

Mouse Game::mouse;


const glm::vec3 Game::platformSize = glm::vec3(4.5, 0.8, 4.5);

std::vector<Platform> Game::platforms;

const float Game::minH_ = 9.5;
const float Game::maxH_ = 16;
float Game::minH = minH_;
float Game::maxH = maxH_;
const float Game::minY = 3;
const float Game::maxY = 5.5;

const float Game::minHSuperSpeed = 18;
const float Game::maxHSuperSpeed = 29.5;


int Game::activeTexture = 4;

void Game::initialize(){
    //puppet = Puppet();
    player.setPuppet(&puppet);
    player.setCamera(&camera);
    player.setCollisionDetector(&collisionDetector);
    player.setRelativeBottomPosition(glm::vec3(0,4.5,0));
    
    //resets collision detector, sets player position, creates first platforms and tells them to the collision detector
    reset();
}

void Game::changeTexture(){
    activeTexture++;
    if (activeTexture == Resources::textureLibrary.size()){
        activeTexture=0;
    }
}

void Game::newPlatform(){
    Platform np = Platform(Util::newPoint(platforms.back().position, minH, maxH, minY, maxY, true), platformSize);
    platforms.push_back(np);
    //collisionDetector.addPlatform(np);
}

void Game::toggleSuperSpeedMode(){
    if (player.getSuperSpeedMode()) {
        player.setSuperSpeedMode(false);
        minH = minH_;
        maxH = maxH_;
        reset();
    } else {
        player.setSuperSpeedMode(true);
        minH = minHSuperSpeed;
        maxH = maxHSuperSpeed;
        reset();
    }
}

void Game::reset(){
//    collisionDetector.reset();
    platforms = {
        Platform(glm::vec3(0, -5, -20), glm::vec3(15, 0.8, 15)),
        Platform(glm::vec3(0, 0, 0), platformSize),
        Platform(Util::newPoint(glm::vec3(0,0,0), minH, maxH, minY, maxY, true), platformSize)
    };
//    for (int i=0; i<platforms.size(); i++){
//        collisionDetector.addPlatform(platforms[i]);
//    }
    collisionDetector.reset(&platforms);
    player.setPosition(startPosition);
}
