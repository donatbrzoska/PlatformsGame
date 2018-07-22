#include "Mouse.hpp"

#include <iostream>

Mouse::Mouse()
{
}

double Mouse::getDifferenceX(){
    update();
     return oldPosX-posX;
}

double Mouse::getDifferenceY(){
    return oldPosY-posY;
}

void Mouse::setWindow(GLFWwindow *window) {
    this->window = window;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Mouse::update(){
    oldPosX = posX;
    oldPosY = posY;
    glfwGetCursorPos(window, &posX, &posY);
}

Mouse::~Mouse()
{
}
