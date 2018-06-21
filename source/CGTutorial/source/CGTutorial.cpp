// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

// Achtung, die OpenGL-Tutorials nutzen glfw 2.7, glfw kommt mit einem veränderten API schon in der Version 3 

// Befindet sich bei den OpenGL-Tutorials unter "common"
#include "shader.hpp"

// Wuerfel und Kugel
#include "objects.hpp"

#include "objloader.hpp"

#include "texture.hpp"

#include <stb/stb_image.h>


#include <iostream>
#include "Obj3D.h"

//CUSTOM
bool secondLight = false;

//const char * vertexShader = "StandardShading.vertexshader";
//const char * fragmentShader = "StandardShading.fragmentshader";
//
//const char * vertexShaderWithSecondLight = "StandardShadingWithSecondLight.vertexshader";
//const char * fragmentShaderWithSecondLight = "StandardShadingWithSecondLight.fragmentshader";


//CHANGE THIS PATH RIGHT HERE TO YOUR PROJECT DIRECTORY PATH
std::string workingDirectory = "/Users/donatdeva/Documents/Studium/4. Semester/Computergrafik/Projekt/CurrentVersion";

std::string resourceDirectory = "/source/CGTutorial/resources";
std::string path = workingDirectory+resourceDirectory;

std::string vertShader(path + "/StandardShading.vertexshader");
const char * vertexShader = vertShader.c_str();

std::string fragShader(path + "/StandardShading.fragmentshader");
const char * fragmentShader = fragShader.c_str();


std::string vertShaderWSL(path + "/StandardShadingWithSecondLight.vertexshader");
const char * vertexShaderWithSecondLight = vertShaderWSL.c_str();

std::string fragShaderWSL(path + "/StandardShadingWithSecondLight.fragmentshader");
const char * fragmentShaderWithSecondLight = fragShaderWSL.c_str();



#include "Puppet.hpp"
Puppet puppet;

#include "Camera.hpp"
Camera camera;

#include "Player.hpp"
Player player;
double playerStepSize = 0.1;

#include "CollisionDetector.hpp"
CollisionDetector collisionDetector;

GLFWwindow* window;
#include "Mouse.hpp"
Mouse mouse = Mouse();

#include "MVP.hpp"

//#include "FrameTimer.hpp"
#include <chrono>
int FPS = 30;

#include <vector>

#include <thread>

glm::vec3 platformSize = glm::vec3(3, 0.6, 3);

//const char * texture = "purple_texture.bmp";

//std::string textures(path+"/purple_texture.bmp");
std::string textures(path+"/purple_texture.bmp");
const char * texture = textures.c_str();

std::string iceTextureImage(path+"/ice.bmp");
const char * iceTexture = iceTextureImage.c_str();

std::string woods1(path+"/wood1.bmp");
const char * wood1 = woods1.c_str();

std::string woods2(path+"/wood2.bmp");
const char * wood2 = woods2.c_str();

std::string woods3(path+"/wood3.bmp");
const char * wood3 = woods3.c_str();

std::string grasss(path+"/grass.bmp");
const char * grass = grasss.c_str();

std::string deepgrass(path+"/deepGrass.bmp");
const char * deepGrass = deepgrass.c_str();

void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_T:
            if(action == GLFW_PRESS)
                camera.switchViewMode();
            break;
        case GLFW_KEY_W:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveForward(true);
                //                player.move("forward", true);
            } else {
                player.moveForward(false);
//                player.move("forward", false);
            }
            break;
        case GLFW_KEY_A:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveLeft(true);
            } else {
                player.moveLeft(false);
            }
            break;
        case GLFW_KEY_S:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveBackward(true);
            } else {
                player.moveBackward(false);
            }
            break;
        case GLFW_KEY_D:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveRight(true);
            } else {
                player.moveRight(false);
            }
            break;
        case GLFW_KEY_Q:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveUp(true);
            } else {
                player.moveUp(false);
            }
            break;
        case GLFW_KEY_SPACE:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.jump();
            }
            break;
        case GLFW_KEY_LEFT_SHIFT:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveDown(true);
            } else {
                player.moveDown(false);
            }
            break;
        case GLFW_KEY_UP:
            puppet.moveForward();
            break;
        case GLFW_KEY_DOWN:
            puppet.moveBackward();
            break;
        case GLFW_KEY_LEFT:
            puppet.moveLeft();
            break;
        case GLFW_KEY_RIGHT:
            puppet.moveRight();
            break;
        default:
            break;
    }
}

void processMouseMove(){
    double diffX = mouse.getDifferenceX();
    double diffY = mouse.getDifferenceY();
    player.turn("h", diffX/5);
    player.turn("v", diffY/5);
}


// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;

//void sendMVP()
//{
//    // Our ModelViewProjection : multiplication of our 3 matrices
//    glm::mat4 MVP = Projection * View * Model;
//    // Send our transformation to the currently bound shader,
//    // in the "MVP" uniform, konstant fuer alle Eckpunkte
//    glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
//
//    //Uebung 6    ###################
//    glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
//    glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
//    glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
//    //#########
//}

//std::vector<Platform> createLevel(int num_platforms, std::vector<Platform> platforms,int delta){
//    if (num_platforms == 0){
//        return platforms;
//    }else {
//        platforms.push_back(Platform(Util::newPoint(platforms.back().position,0,delta,0,delta, false), platformSize));
//        return createLevel(num_platforms-1,platforms,delta+2);
//    }
//}
std::vector<Platform> createLevel(int num_platforms, std::vector<Platform> platforms,int minDeltaV,int minDeltaH,int distanceV,int distanceH){
    if (num_platforms == 0){
        return platforms;
    }else {
        platforms.push_back(Platform(Util::newPoint(glm::vec3(0,0,0), minDeltaV,minDeltaV+distanceV,minDeltaH,minDeltaH+distanceH, false ), platformSize));
        return createLevel(num_platforms-1,platforms,minDeltaV+distanceV,minDeltaH+distanceH,distanceV,distanceH);
    }
}

//GLuint loadTexture(const char * imagepath){
//
//    printf("Reading image %s\n", imagepath);
//
//    // Data read from the header of the BMP file
//    unsigned char header[54];
//    unsigned int dataPos;
//    unsigned int imageSize;
//    unsigned int width, height;
//    // Actual RGB data
//    unsigned char * data;
//
//    // Open the file
//    FILE * file = fopen(imagepath,"rb");
//    if (!file)                                {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}
//
//    // Read the header, i.e. the 54 first bytes
//
//    // If less than 54 bytes are read, problem
//    if ( fread(header, 1, 54, file)!=54 ){
//        printf("Not a correct BMP file\n");
//        return 0;
//    }
//
//
//    // Read the information about the image
//    dataPos    = *(int*)&(header[0x0A]);
//    imageSize  = *(int*)&(header[0x22]);
//    width      = *(int*)&(header[0x12]);
//    height     = *(int*)&(header[0x16]);
//
//    // Some BMP files are misformatted, guess missing information
//    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
//    if (dataPos==0)      dataPos=54; // The BMP header is done that way
//
//    // Create a buffer
//    unsigned char *image = stbi_load(imagepath,
//                                     &width,
//                                     &height,
//                                     &channels,
//                                     STBI_rgb_alpha);    //good
//
//    // Read the actual data from the file into the buffer
//    fread(data,1,imageSize,file);
//
//    // Everything is in memory now, the file wan be closed
//    fclose (file);
//
//    // Create one OpenGL texture
//    GLuint textureID;
//    glGenTextures(1, &textureID);
//
//    // "Bind" the newly created texture : all future texture functions will modify this texture
//    glBindTexture(GL_TEXTURE_2D, textureID);
//
//    // Give the image to OpenGL
//    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, image);
//
//    // OpenGL has now copied the data. Free our own version
//    delete [] data;
//
//    // Poor filtering, or ...
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//
//    // ... nice trilinear filtering.
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//    // Return the ID of the texture we just created
//    return textureID;
//}

int main(void)
{
//    glm::vec3 v1(1.0f, 0.0f, 1.0f);
//    glm::vec3 v2(0.0f, 0.0f, 1.0f);
//    glm::vec3 v3(1.0f, 0.0f, 0.0f);
//    glm::vec3 n1 = v2-v1;
//    glm::vec3 n2 = v3-v1;
//    glm::vec3 n = glm::cross(n1, n2);
//    Util::print("normale: ", n);
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    puppet = Puppet();
    player.setPuppet(&puppet);
    player.setCamera(&camera);
    player.setCollisionDetector(&collisionDetector);
    player.setPosition(glm::vec3(0,4.5,0));
    player.setRelativeBottomPosition(glm::vec3(0,4.5,0));
    
//    std::vector<Platform> platforms = {
//        Platform(glm::vec3(0, 0, 0), platformSize)
//        ,Platform(glm::vec3(10, -4, -8), platformSize),
//        Platform(glm::vec3(27, -8, -10), platformSize),
//        Platform(glm::vec3(32, -13, -14), platformSize),
//        Platform(glm::vec3(39, -17, -9), platformSize),
//        Platform(glm::vec3(48, -20, -17), platformSize),
//        Platform(glm::vec3(-8, 5, 8), platformSize),
//        Platform(Util::newPoint(glm::vec3(0,0,0), 8, 12, 3, 5, false), platformSize)
//    };

    
    std::vector<Platform> platforms = { Platform(glm::vec3(0, 0, 0), platformSize)};
    platforms = createLevel(400,platforms, 9, 1.5, 8.5, 4);
    
    for (int i=0; i<platforms.size(); i++){
        collisionDetector.addPlatform(platforms[i]);
    }
    
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
//    player.initialize();
//    player.setStepSize(playerStepSize);
//    FrameTimer::initialize(FPS);
    
	// Initialise GLFW-Bibliothek (kann unter anderem Fenster oeffnen)
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	//error_callback Funktion wird fuer Fehlerfall uebergeben
	glfwSetErrorCallback(error_callback);

    
    // Die folgenden vier Zeilen sind nštig auf dem Mac ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // Au§erdem mŸssen die zu ladenden Dateien bei der aktuellen Projektkonfiguration
    // unter DerivedData/Build/Products/Debug (oder dann Release) zu finden sein
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
	// Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
    window = glfwCreateWindow(1024, // Breite (default: 1024)
		768,  // Hoehe
		"Platforms", // Ueberschrift
		NULL,  // windowed mode
		NULL); // shared window
    
    mouse.setWindow(window); //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    
//Fensterpointer == null -> terminieren
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make the window's context current (wird nicht automatisch gemacht)
	// -> mehrere Fenster -> immer wieder aufrufen mit entsprechendem Fenster
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	// GLEW ermöglicht Zugriff auf OpenGL-API > 1.1
	glewExperimental = true; // Needed for core profile

	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	//BIS HIER OPEN GL VORBEREITUNG


	// Auf Keyboard-Events reagieren
	glfwSetKeyCallback(window, key_callback);
    
//    glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

	// Dark blue background
	// red, green, blue, intensity -> 1 is intense
	// Lediglich Definition der Loeschfarbe
	//glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Create and compile our GLSL program from the shaders
	//original ##########
	//programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");
	//############
	//Uebung 6 ######################

	//CUSTOM
	if (!secondLight) {
		programID = LoadShaders(vertexShader, fragmentShader);
	}
	else {
		programID = LoadShaders(vertexShaderWithSecondLight, fragmentShaderWithSecondLight);
		//std::cout << "second Shader loaded";
	}
	//

    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    MVP::initialize(programID);
    
	//#########

	// Shader auch benutzen !
	glUseProgram(programID);

	//korrekte Ueberlappungsprojektion ermoeglichen
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);


//    Obj3D teapot("teapot.obj");
    std::string teapotObj(path+"/teapot.obj");
    const char * teapotPath = teapotObj.c_str();
    Obj3D teapot(teapotPath);

//    glm::vec3 lightPos = glm::vec3(-1, 4, -1);
//    glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

	//second light
	if (secondLight) {
		glm::vec3 lightPos2 = glm::vec3(5, 80, -7);	//x positiv -> links, z negativ -> vorne
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace2"), lightPos2.x, lightPos2.y, lightPos2.z);
		//std::cout << "put second light";
	}
	//##########

	// Load the texture
    //GLuint Texture = loadBMP_custom("mandrill.bmp");
    GLuint Texture = loadBMP_custom(texture);       //good
    GLuint IceTexture = loadBMP_custom(iceTexture);     //good
    GLuint Wood1 = loadBMP_custom(wood1);           //good
    GLuint Wood2 = loadBMP_custom(wood2);
    GLuint Wood3 = loadBMP_custom(wood3);
    GLuint Grass = loadBMP_custom(grass);
    GLuint DeepGrass = loadBMP_custom(deepGrass);   //good

    
    
	// Bind our texture in Texture Unit 0 //multiple textures also possible //put in loop, if textures change
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

    
    
    //PUPPET ############################
    // Eventloop
    
    
    player.moveForward(true);
    player.moveForward(false);
    
	// Eventloop
	while (!glfwWindowShouldClose(window))
	{
//        if (FrameTimer::newFrame()) {
        
			// Clear the screen | Ueberlappungsprojektion++
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

			// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units (Es wird alles von 0.1 bis 100 Einheiten angezeigt)
//            Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
            View = glm::lookAt(camera.position, camera.lookAtAbsolute, glm::vec3(0,1,0));
        MVP::setView(View);
        // Model matrix : an identity matrix (model will be at the origin)
        
        
        glm::vec3 lightPos = player.position + glm::vec3(0,20,0);
        glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
        
        glm::mat4 Save = glm::mat4(1.f);
        
        
        glBindTexture(GL_TEXTURE_2D, Wood1);
//        glBindTexture(GL_TEXTURE_2D, *image);
        
        if(camera.getThirdPersonMode()){
            puppet.drawPuppet();
            
            
            //"SPHERE-Player"
//            Model = Save;
//            Model = glm::translate(Model, player.position);
//            Model = glm::scale(Model, glm::vec3(1.f/5.f, 1.f/5.f, 1.f/5.f));
//            MVP::setModel(Model);
//            drawSphere(20, 20);
        }

        //        if (Platform::getNewPlatformReady()){
        //            Platform np(Util::newPoint(glm::vec3(0,0,0), 2, 20, 3, 5, false), platformSize);
        //            platforms.push_back(np);
        //            collisionDetector.addPlatform(np);
        //        }
        
//        glBindTexture(GL_TEXTURE_2D, IceTexture);
        
        for (int i=0; i<platforms.size(); i++){
            platforms[i].draw();
        }
        
        
        
//        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        ////"Teapot"
        Model = Save;
        Model = glm::translate(Model, glm::vec3(15, 0.0, 15));
        Model = glm::scale(Model, glm::vec3(1.0 / 100.0, 1.0 / 100.0, 1.0 / 100.0));
        Model = glm::rotate(Model, Util::degreesToRadians(-100), glm::vec3(1,0,0));
//        Model = Util::custom_rotate(Model, 270, glm::vec3(0,1,0));
//        sendMVP();
        MVP::setModel(Model);
        teapot.display();
        Model = Save;


        // Swap buffers -> Bild anzeigen
        glfwSwapBuffers(window);
		//}
		// Poll for and process events -> Aufruf der entsprechenden callback Funktionen
        glfwPollEvents();
        processMouseMove();
//        }
//        std::chrono::nanoseconds ns(42000000L);
//        std::this_thread::sleep_for(ns);
	}

	glDeleteProgram(programID);

	//Texturen aus Speicher loeschen
	glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}

/*
7
1.5 
2.5
2
*/
