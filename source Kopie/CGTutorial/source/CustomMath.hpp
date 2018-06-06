#pragma once
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class Util
{
public:
    static float degreesToRadians(float d);
    static void printVec3WithText(std::string s, glm::vec3 v);
    static void print(std::string s);
    static void print(int i);
    static void print(double f);
    Util();
    ~Util();
};
