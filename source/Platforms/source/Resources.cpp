#include "Resources.hpp"

void Resources::initialize(){
    #ifdef __APPLE__ || __MACH__
    path = workingDirectory+resourceDirectory;
    #else
        path = "";
    #endif
    
    vertShader = path + "StandardShading.vertexshader";
    vertexShader = vertShader.c_str();
    
    fragShader = path + "StandardShading.fragmentshader";
    fragmentShader = fragShader.c_str();
    
    vertShaderWSL = path + "StandardShadingWithSecondLight.vertexshader";
    vertexShaderWithSecondLight = vertShaderWSL.c_str();
    
    fragShaderWSL = path + "StandardShadingWithSecondLight.fragmentshader";
    fragmentShaderWithSecondLight = fragShaderWSL.c_str();
    
    for (int i=0; i<textureFiles.size(); i++) {
        textureLibrary.push_back(loadBMP_custom((path + textureFiles[i]).c_str()));
    }
}
