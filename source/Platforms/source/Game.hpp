#pragma once

#include "Puppet.hpp"
#include "Camera.hpp"
#include "Player.hpp"
#include "CollisionDetector.hpp"
#include "Mouse.hpp"
#include "Resources.hpp"

//keeps track of all game objects
class Game {
public:
    static Puppet puppet;
    
    static Camera camera;
    
    static CollisionDetector collisionDetector;
    
    static Player player;
    static const glm::vec3 startPosition;
    static const double playerStepSize; // NECESSARY???
    
    static Mouse mouse;
    
    
    static const glm::vec3 platformSize;
    static std::vector<Platform> platforms;
    //Min max values for platforms
    static const float minH_;
    static const float maxH_;
    static float minH;
    static float maxH;
    static const float minY;
    static const float maxY;
    
    static const float minHSuperSpeed;
    static const float maxHSuperSpeed;
    
    
    static int activeTexture;
    
    static void initialize();
    
    static void changeTexture();

    static void newPlatform();
    
    static void toggleSuperSpeedMode();
    
    static void reset();
};
