#include "CollisionDetector.hpp"

bool CollisionDetector::coordinateLaysIn(glm::vec3 co, glm::vec3 from, glm::vec3 to) {
    bool laysIn = false;
//    Util::print("co: ", co);
//    Util::print("From :", from);
//    Util::print("To :", to);
    if (co.x <= to.x & co.x >= from.x)
        if (co.y <= to.y & co.y >= from.y)
            if (co.z <= to.z & co.z >= from.z)
                laysIn =  true;
//    Util::print(laysIn);
//    std::cout << std::endl;
    return laysIn;
}

bool CollisionDetector::collision(glm::vec3 objectBottomPosition){
    bool collision = false;
//    for (std::list<Platform*>::iterator li = platforms.begin(); li!=platforms.end(); li++){
//        if (coordinateLaysIn(objectBottomPosition, (*li)->from, (*li)->to)) {
//            collision = true;
//        }
//    }
    for (int i=0; i<platforms.size(); i++) {
//        std::cout << i << std::endl;;
        if (coordinateLaysIn(objectBottomPosition, platforms[i]->from, platforms[i]->to)) {
            collision = true;
        }
    }
    return collision;
}

void CollisionDetector::addPlatform(Platform* platform) {
//    std::list<Platform*>::iterator it = platforms.end();
//    platforms.insert(it, platform);
    platforms.insert(platforms.end()++, platform);
}
