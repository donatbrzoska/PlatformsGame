#pragma once
#include <iostream>
#include <GL/glew.h>
#include <vector>
#include "texture.hpp"

//keeps track of all resource files and textures
class Resources {
    public:
    static std::string workingDirectory;
    static std::string resourceDirectory;
    static std::string path;
    
    static std::string vertShader;
    static const char * vertexShader;
    
    static std::string fragShader;
    static const char * fragmentShader;
    
    static std::string vertShaderWSL;
    static const char * vertexShaderWithSecondLight;
    
    static std::string fragShaderWSL;
    static const char * fragmentShaderWithSecondLight;
    
    
    static const std::vector<std::string> textureFiles;
    static std::vector<GLuint> textureLibrary;
    
    static void initialize();
};
