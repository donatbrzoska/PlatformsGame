#include "Mouse.hpp"

#include <iostream>

Mouse::Mouse()
{
}

double Mouse::getDifferenceX(){
    update();
    //if (oldPosX != 0)   //first call will not calculate difference of 0 and position
        return oldPosX-posX;
    //else
    //    return posX;
}

double Mouse::getDifferenceY(){
    //if (oldPosY != 0)
        return oldPosY-posY;
    //else
    //    return posY;
}

void Mouse::setWindow(GLFWwindow *window) {
    this->window = window;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Mouse::update(){
    oldPosX = posX;
    oldPosY = posY;
    glfwGetCursorPos(window, &posX, &posY);
    //std::cout << "posx: " << posX << " posy: " << posY << std::endl;
}

Mouse::~Mouse()
{
}
