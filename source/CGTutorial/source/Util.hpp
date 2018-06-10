#pragma once
#include <iostream>
#include <string>
#include <glm/gtc/matrix_transform.hpp>
#include "objects.hpp"
#include "MVP.hpp"

class Util {
public:
    Util(){};
    static float degreesToRadians(float d);
    static glm::mat4 custom_rotate(glm::mat4 m, float angle, glm::vec3 n);
    
    static void print(std::string s, glm::vec3 v);
    static void print(std::string s);
    static void print(int i);
    static void print(double f);
    static void drawCS(glm::mat4 Model);
    ~Util(){};
};
