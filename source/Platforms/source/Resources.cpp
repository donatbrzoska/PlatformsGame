#include "Resources.hpp"

//CHANGE THIS PATH RIGHT HERE TO YOUR PROJECT DIRECTORY PATH
std::string Resources::workingDirectory = "/Users/donatdeva/Documents/Studium/4. Semester/Computergrafik/Projekt/CurrentVersion";
std::string Resources::resourceDirectory = "/source/Platforms/resources/";

std::string Resources::path;

std::string Resources::vertShader;
const char * Resources::vertexShader;

std::string Resources::fragShader;
const char * Resources::fragmentShader;

std::string Resources::vertShaderWSL;
const char * Resources::vertexShaderWithSecondLight;

std::string Resources::fragShaderWSL;
const char * Resources::fragmentShaderWithSecondLight;

const std::vector<std::string> Resources::textureFiles = {
    //http://wallpaperswide.com/purple_texture-wallpapers.html
    "purple_texture.bmp",
    //https://creativemarket.com/ultrapro/556686-Abstract-blue-ice-texture
    "ice.bmp",
    //https://www.pexels.com/photo/full-frame-shot-of-snowflakes-326240/
    "ice2.bmp",
    //https://texturefabrik.files.wordpress.com/2017/03/texture_fabrik_cyan_textures_01.jpg
    "glasstexture.bmp",
    //http://biteinto.info/wp-content/uploads/2016/02/seamless-dark-wood-flooring-texture-and-darkwood-plank-faux-wood-rug-flooring-background-or-by-funlicious-20.jpg
    "wood3.bmp",
    //http://www.tech-lovers.com/wp-content/uploads/2014/12/Wood-Texture-Background-2.jpg
    "wood2.bmp",
    //http://paperlief.com/images/blue-bokeh-texture-wallpaper-1.jpg
    "blue_bokeh.bmp",
    //http://cdn.desktopwallpapers4.me/wallpapers/abstract/2560x1440/3/22123-blue-scratched-texture-2560x1440-abstract-wallpaper.jpg
    "blue_scratch.bmp",
    //http://informationcommunicationtechnology.com/wp-content/uploads/2018/06/texture-557.jpg
    "blue_black_texture.bmp",
    //https://isorepublic.com/wp-content/uploads/2017/07/texture-background-1062x708.jpg
    "black_cracked.bmp",
    //http://www.tudesign.cn/Uploads/Picture/2016-02-18/56c5a6deed0c2_water.jpg
    "texture_water.bmp",
    //http://www.oclanguagecenter.org/wp-content/uploads/cool-green-texture-wallpaper.jpg
    "green_texture.bmp",
    //http://squaretexture.com/uploads/product/98/thumbs/diy013_stone_texture_sandstone_limestone_620_product_preview.jpg
    "sandstone.bmp",
    //https://img1.cgtrader.com/items/255716/98c3c16d26/stone-wall-6-3d-model-max-obj-fbx.jpg
    "stone2.bmp",
};

std::string Resources::jumpSoundFile;
//const char * Resources::jumpSound;

std::string Resources::hitGroundSoundFile;
//const char * Resources::landSound;

std::vector<GLuint> Resources::textureLibrary;

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
    
    jumpSoundFile = path + "jump.mp3";
//    jumpSound = jumpSoundFile.c_str();
    
    hitGroundSoundFile = path + "hit_ground.mp3";
//    landSound = landSoundFile.c_str();
}
