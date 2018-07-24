#pragma once
#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "texture.hpp"

//keeps track of all resource files and textures
class Resources {
    public:
    //CHANGE THIS PATH RIGHT HERE TO YOUR PROJECT DIRECTORY PATH
    std::string workingDirectory = "/Users/donatdeva/Documents/Studium/4. Semester/Computergrafik/Projekt/CurrentVersion";
    std::string resourceDirectory = "/source/Platforms/resources/";
    std::string path;
    
    std::string vertShader;
    const char * vertexShader;
    
    std::string fragShader;
    const char * fragmentShader;
    
    std::string vertShaderWSL;
    const char * vertexShaderWithSecondLight;
    
    std::string fragShaderWSL;
    const char * fragmentShaderWithSecondLight;
    
    
    
    const std::vector<std::string> textureFiles = {
        "purple_texture.bmp",
        "ice.bmp",
        "ice2.bmp",
        "wood1.bmp",
        "deepGrass.bmp",
    };
    std::vector<GLuint> textureLibrary;
    
    void initialize();
};
