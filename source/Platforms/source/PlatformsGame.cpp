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

#include <iostream>
#include "Obj3D.h"

//CUSTOM
bool secondLight = true;

//const char * vertexShader = "StandardShading.vertexshader";
//const char * fragmentShader = "StandardShading.fragmentshader";
//
//const char * vertexShaderWithSecondLight = "StandardShadingWithSecondLight.vertexshader";
//const char * fragmentShaderWithSecondLight = "StandardShadingWithSecondLight.fragmentshader";

#include "Resources.hpp"
#include <cstdlib>
#include "Game.hpp"
Game game;

GLFWwindow* window;

#include "MVP.hpp"

#include <vector>

#include <thread>

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
        case GLFW_KEY_C:
            if(action == GLFW_PRESS)
                Game::camera.switchViewMode();
            break;
        case GLFW_KEY_W:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveForward(true);
            } else {
                Game::player.moveForward(false);
            }
            break;
        case GLFW_KEY_A:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveLeft(true);
            } else {
                Game::player.moveLeft(false);
            }
            break;
        case GLFW_KEY_S:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveBackward(true);
            } else {
                Game::player.moveBackward(false);
            }
            break;
        case GLFW_KEY_D:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveRight(true);
            } else {
                Game::player.moveRight(false);
            }
            break;
        case GLFW_KEY_SPACE:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.jump();
                Game::newPlatform();
            }
            break;
        case GLFW_KEY_N:
            Game::reset();
            break;
        case GLFW_KEY_H:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::toggleSuperSpeedMode();
            }
            break;
        case GLFW_KEY_T:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::changeTexture();
            }
            break;
            
            
        case GLFW_KEY_ENTER:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveUp_(true);
            } else {
                Game::player.moveUp_(false);
            }
            break;
        case GLFW_KEY_RIGHT_SHIFT:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveDown_(true);
            } else {
                Game::player.moveDown_(false);
            }
            break;
        case GLFW_KEY_LEFT:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveLeft_(true);
            } else {
                Game::player.moveLeft_(false);
            }
            break;
        case GLFW_KEY_RIGHT:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveRight_(true);
            } else {
                Game::player.moveRight_(false);
            }
            break;
        case GLFW_KEY_UP:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveForward_(true);
            } else {
                Game::player.moveForward_(false);
            }
            break;
        case GLFW_KEY_DOWN:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                Game::player.moveBackward_(true);
            } else {
                Game::player.moveBackward_(false);
            }
            break;
        default:
            break;
    }
}

void processMouseMove(){
    double diffX = Game::mouse.getDifferenceX();
    double diffY = Game::mouse.getDifferenceY();
    Game::player.turn("h", diffX/5);
    Game::player.turn("v", diffY/5);
}


// Diese Drei Matrizen global (Singleton-Muster), damit sie jederzeit modifiziert und
// an die Grafikkarte geschickt werden koennen
glm::mat4 Projection;
glm::mat4 View;
glm::mat4 Model;
GLuint programID;

int main(void)
{
    Game::initialize();
	// Initialise GLFW-Bibliothek (kann unter anderem Fenster oeffnen)
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		exit(EXIT_FAILURE);
	}

	// Fehler werden auf stderr ausgegeben, s. o.
	//error_callback Funktion wird fuer Fehlerfall uebergeben
	glfwSetErrorCallback(error_callback);

    #ifdef __APPLE__ || __MACH__
        // Die folgenden vier Zeilen sind nštig auf dem Mac
        // Au§erdem mŸssen die zu ladenden Dateien bei der aktuellen Projektkonfiguration
        // unter DerivedData/Build/Products/Debug (oder dann Release) zu finden sein
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    
	// Open a window and create its OpenGL context
	// glfwWindowHint vorher aufrufen, um erforderliche Resourcen festzulegen
    window = glfwCreateWindow(1024, // Breite (default: 1024)
		768,  // Hoehe
		"Platforms", // Ueberschrift
		NULL,  // windowed mode
		NULL); // shared window
    //tell the window to the mouse
    Game::mouse.setWindow(window);
    
    
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

	//OPEN GL SETUP UNTIL HERE


	// Auf Keyboard-Events reagieren
	glfwSetKeyCallback(window, key_callback);
    

	// Set background color
	// red, green, blue, intensity -> 1 is intense
	// Lediglich Definition der Loeschfarbe
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

    
    // Bind our texture in Texture Unit 0 //multiple textures also possible //put in loop, if textures change
    glActiveTexture(GL_TEXTURE0);
    //    glBindTexture(GL_TEXTURE_2D, Texture);
    
    // Set our "myTextureSampler" sampler to user Texture Unit 0
    glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

    Resources::initialize();
    
	//CUSTOM
	if (!secondLight) {
		programID = LoadShaders(Resources::vertexShader, Resources::fragmentShader);
	}
	else {
		programID = LoadShaders(Resources::vertexShaderWithSecondLight, Resources::fragmentShaderWithSecondLight);
	}

    //tell the programID to the external MVP Class
    MVP::initialize(programID);
    
    
	// Shader auch benutzen !
	glUseProgram(programID);
    
	//korrekte Ueberlappungsprojektion ermoeglichen
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
    
    
////    Obj3D teapot("teapot.obj");
//    std::string teapotObj(Resources::path+"/teapot.obj");
//    const char * teapotPath = teapotObj.c_str();
//    Obj3D teapot(teapotPath);
    
    
    Game::player.moveForward(true);
    Game::player.moveForward(false);
    
	// Eventloop
	while (!glfwWindowShouldClose(window))
	{
        
	    // Clear the screen | Ueberlappungsprojektion++
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        View = glm::lookAt(Game::camera.position, Game::camera.lookAtAbsolute, glm::vec3(0,1,0));
        //tell the view to the external MVP Class
        MVP::setView(View);
        
        //reset player position if he falls in void
        if (Game::player.position.y < -15) {
//            Game::player.setPosition(Game::startPosition);
            Game::reset();
        }
        
        //light follows the player
        glm::vec3 lightPos = Game::player.position + glm::vec3(0,50,0);    //x positiv -> links, z negativ -> vorne
        glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);
        
        //second light
        if (secondLight) {
            glm::vec3 lightPos2 = Game::player.position + Game::player.getLookAtRelativeH()*50.f;
//            Util::print("light pos2:", lightPos2);
            glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace2"), lightPos2.x, lightPos2.y, lightPos2.z);
            //std::cout << "put second light";
        }
        
//        glm::mat4 Save = glm::mat4(1.f);
        
        
        glBindTexture(GL_TEXTURE_2D, Resources::textureLibrary[4]);
//        glBindTexture(GL_TEXTURE_2D, Wood2);
        
        //draw puppet only if third person mode is enabled
        if(Game::camera.getThirdPersonMode()){
            Game::puppet.drawPuppet();
            
            //"SPHERE-Player"
//            Model = Save;
//            Model = glm::translate(Model, player.position);
//            Model = glm::scale(Model, glm::vec3(1.f/5.f, 1.f/5.f, 1.f/5.f));
//            MVP::setModel(Model);
//            drawSphere(20, 20);
        }

        
//        glBindTexture(GL_TEXTURE_2D, IceTexture);
//        glBindTexture(GL_TEXTURE_2D, DeepGrass);
        glBindTexture(GL_TEXTURE_2D, Resources::textureLibrary[Game::activeTexture]);
        
        //draw platforms
        for (int i=0; i<Game::platforms.size(); i++){
            Game::platforms[i].draw();
        }
        
        const char * s = std::string("Platforms - Score: " + std::to_string(Game::collisionDetector.score)).c_str();
        glfwSetWindowTitle(window, s);
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, Texture);
//        ////"Teapot"
//        Model = Save;
//        Model = glm::translate(Model, glm::vec3(15, 0.0, 15));
//        Model = glm::scale(Model, glm::vec3(1.0 / 100.0, 1.0 / 100.0, 1.0 / 100.0));
//        Model = glm::rotate(Model, Util::degreesToRadians(-100), glm::vec3(1,0,0));
////        Model = Util::custom_rotate(Model, 270, glm::vec3(0,1,0));
////        sendMVP();
//        MVP::setModel(Model);
//        teapot.display();
//        Model = Save;


        // Swap buffers -> Bild anzeigen
        glfwSwapBuffers(window);
        
		// Poll for and process events -> Aufruf der entsprechenden callback Funktionen
        glfwPollEvents();
        processMouseMove();
	}

	glDeleteProgram(programID);

	//Texturen aus Speicher loeschen
    for (int i=0; i<Resources::textureLibrary.size(); i++){
        glDeleteTextures(1, &Resources::textureLibrary[i]);
    }

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
	return 0;
}
