#pragma once
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <iostream>

class MVP {
public:
    static void initialize(GLuint programID);
    static void setModel(glm::mat4 Model);
    static void setView(glm::mat4 View);
};
