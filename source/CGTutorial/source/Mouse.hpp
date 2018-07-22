#pragma once
// Include GLFW
#include <GLFW/glfw3.h>

//keeps track of mouse movements
//(after telling it its window to observe of course)
class Mouse
{
    GLFWwindow* window;
    double oldPosX;
    double oldPosY;
    double posX;
    double posY;
    void update();
    
public:
    
    Mouse();
    void setWindow(GLFWwindow* window);
    double getDifferenceX();
    double getDifferenceY();
    ~Mouse();
};
