#include "CollisionDetector.hpp"

bool CollisionDetector::coordinateLaysIn(glm::vec3 co, glm::vec3 from, glm::vec3 to) {
    if (co.x <= to.x & co.x >= from.x)
        if (co.y <= to.x & co.y >= from.y)
            if (co.z <= to.z & co.z <= from.z)
                return true;
    return false;
}

bool CollisionDetector::collision(glm::vec3 objectBottomPosition){
    bool collision = false;
    for (std::list<Platform*>::iterator li = platforms.begin(); li!=platforms.end(); li++){
        if (coordinateLaysIn(objectBottomPosition, (*li)->from, (*li)->to)) {
            collision = true;
        }
    }
    return collision;
}

void CollisionDetector::addPlatform(Platform* platform) {
    std::list<Platform*>::iterator it = platforms.end();
    platforms.insert(it, platform);
}
