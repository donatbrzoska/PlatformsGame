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



#include <iostream>
#include "Obj3D.h"

//CUSTOM
bool secondLight = true;

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

//#include "FrameTimer.hpp"
#include <chrono>
int FPS = 30;


#include <thread>



//PUPPET ####################################
float alpha = 180.0f;
float animationend = 15.0f;
float armwinkel = 0.0f;
float i = 0;
float beta = 180.0f;
float speed = 2.0f;
float jump = 0.0f;
float z_armwinkel = 0.0f;

float x_pos_human = 0.0f;
float y_pos_human = 0.0f;
float z_pos_human = 0.0f;
bool inair = false;


void setstraight() {
    alpha = 180.0f;
    armwinkel = 40.0f;
    i = 0;
    beta = 180.0f;
    z_armwinkel = 0.0f;
}

float speed2 = speed;

//const char * texture = "purple_texture.bmp";

std::string textures(path+"/purple_texture.bmp");
const char * texture = textures.c_str();

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
        case  GLFW_KEY_UP:
            z_pos_human += 0.4;
        case GLFW_KEY_DOWN:
            z_armwinkel = 0;
            if (i == animationend) {
                speed *= -1;
                i = -animationend;
                speed2 *=-1;
            }
            if (armwinkel == 0) {
                speed2 *= -1;
            }
            z_pos_human -= 0.2;
            armwinkel += speed2;
            beta -= speed;
            alpha += speed;
            i++;
            break;
        case  GLFW_KEY_LEFT:
            x_pos_human -= 0.4;
        case GLFW_KEY_RIGHT:
            if (i == animationend) {
                speed *= -1;
                i = -animationend;
            }
            x_pos_human += 0.2;
            armwinkel += speed/2;
            z_armwinkel += speed / 2;
            beta -= speed;
            alpha += speed;
            i++;
            break;
        case GLFW_KEY_C:
            if (inair == false) {
                inair = true;
            }
            break;
        case GLFW_KEY_SPACE:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveUp(true);
            } else {
                player.moveUp(false);
            }
            break;
        case GLFW_KEY_LEFT_SHIFT:
            if (action==GLFW_PRESS | action==GLFW_REPEAT) {
                player.moveDown(true);
            } else {
                player.moveDown(false);
            }
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

void sendMVP()
{
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; 
	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform, konstant fuer alle Eckpunkte
	glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	//Uebung 6	###################
	glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
	//#########
}


//Uebung "9" ###################
void drawCS() {

    double lineWidth = 0.007f;
//    double lineLength = 0.8f;
    double lineLength = 2.f;

	glm::mat4 Save = Model;
	Model = glm::scale(Model, glm::vec3(lineLength, lineWidth, lineWidth));
	sendMVP();
	drawCube();

	Model = Save;
	Model = glm::scale(Model, glm::vec3(lineWidth, lineLength, lineWidth));
	sendMVP();
	drawCube();

	Model = Save;
	Model = glm::scale(Model, glm::vec3(lineWidth, lineWidth, lineLength));
	sendMVP();
	drawCube();
	Model = Save;
}

void drawPlatform(glm::vec3 at) {
    glm::mat4 Save = Model;
//    Model = glm::scale(Model, glm::vec3(2, 0.3, 2));
    Model = glm::translate(Model, at);
    drawCS();
    sendMVP();
    drawCube();
    Model=Save;
}


//PUPPET ###### ###### ###### ###### ###### ###### ######
void drawPart(double h, double s) {
    glm::mat4 Save = Model;
    Model = glm::translate(Model, glm::vec3(0, h / 2, 0));
    Model = glm::scale(Model, glm::vec3(s, h / 2, s));
    sendMVP();
    drawSphere(10, 10);
    Model = Save;
}
void drawPart(double h, double s, double b) {
    glm::mat4 Save = Model;
    Model = glm::translate(Model, glm::vec3(b, h / 2, 0));
    Model = glm::scale(Model, glm::vec3(s, h / 2, s));
    sendMVP();
    drawSphere(10, 10);
    Model = Save;
}


int main(void)
{
    //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    player.setCamera(&camera);
    player.setCollisionDetector(&collisionDetector);
    Platform p1(glm::vec3(0, 0, -3));
    collisionDetector.addPlatform(&p1);
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
	window = glfwCreateWindow(1024, // Breite
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

	glm::vec3 lightPos = glm::vec3(-1, 2, -1);
	glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace"), lightPos.x, lightPos.y, lightPos.z);

	//second light
	if (secondLight) {
		glm::vec3 lightPos2 = glm::vec3(5, -4, -4);	//x positiv -> links, z negativ -> vorne
		glUniform3f(glGetUniformLocation(programID, "LightPosition_worldspace2"), lightPos2.x, lightPos2.y, lightPos2.z);
		//std::cout << "put second light";
	}
	//##########

	// Load the texture
	//GLuint Texture = loadBMP_custom("mandrill.bmp");
	GLuint Texture = loadBMP_custom(texture);

	// Bind our texture in Texture Unit 0 //multiple textures also possible //put in loop, if textures change
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(glGetUniformLocation(programID, "myTextureSampler"), 0);

    
    
    //PUPPET ############################
    //kreis
    glm::mat4 Save = Model;
    glm::mat4 Save2 = Model;
    glm::mat4 Save3 = Model;
    glm::mat4 Save4 = Model;
    // Eventloop
    
    double fall = 0.001;
    double jumpwinkel = 0.0f;
    
    
	// Eventloop
	while (!glfwWindowShouldClose(window))
	{
//        if (FrameTimer::newFrame()) {
        
			// Clear the screen | Ueberlappungsprojektion++
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

			// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units (Es wird alles von 0.1 bis 100 Einheiten angezeigt)
			Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		
            View = glm::lookAt(camera.position, camera.lookAtAbsolute, glm::vec3(0,1,0));

        // Model matrix : an identity matrix (model will be at the origin)

			//PUPPET
        Model = glm::mat4(1.0f);
        
        sendMVP();
        Model = Save;
        Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
        Model = glm::translate(Model, glm::vec3(x_pos_human, y_pos_human, z_pos_human));
        
        
        //jumping
        if (inair == true) {
            setstraight();
            y_pos_human += 3;
            inair = false;
            jumpwinkel = 180;
        }
        
        if (y_pos_human <= 0) {
            fall = 0;
            jumpwinkel = 0;
        }
        if (y_pos_human >= 0) {
            fall = 0.001;
        }
        y_pos_human -= fall;
        
        //drawing human
        double length = 2.0;
        double ancle = 0.5;
        double thickness = 0.3;
        double gap = 1;
        
        //head
        drawPart(2, 1, 0.5);
        Model = glm::translate(Model, glm::vec3(0.0, -3, 0.0));
        
        //core
        drawPart(3, 1, 0.5);
        Model = glm::translate(Model, glm::vec3(0.0, -1.5, 0.0));
        
        //gap = 1.75;
        
        //becken
        drawPart(1.5, 1.5 / 2, 0.5);
        Model = glm::translate(Model, glm::vec3(0.0, 0.5, 0.0));
        
        //left leg
        gap = -0.1;
        Model = Util::custom_rotate(Model, alpha, glm::vec3(1.0, 0.0, 0.0));
        Model = Util::custom_rotate(Model, z_armwinkel, glm::vec3(0.0, 0.0, 1.0));
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        Model = Util::custom_rotate(Model, -armwinkel, glm::vec3(1.0, 0.0, 0.0));
        
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        Model = glm::translate(Model, glm::vec3(0.0, (-ancle - length) * 2, 0));
        Model = Util::custom_rotate(Model, -alpha, glm::vec3(1.0, 0.0, 0.0));
        Model = Util::custom_rotate(Model, -z_armwinkel, glm::vec3(0.0, 0.0, 1.0));
        
        
        Model = Save3;
        Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
        Model = glm::translate(Model, glm::vec3(x_pos_human, y_pos_human, z_pos_human));
        Model = glm::translate(Model, glm::vec3(0, -4, 0));
        
        //right leg
        gap = 1;
        
        Model = Util::custom_rotate(Model, -alpha, glm::vec3(1.0, 0.0, 0.0));
        Model = Util::custom_rotate(Model, -z_armwinkel, glm::vec3(0.0, 0.0, 1.0));
        
        //Gelenk
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        
        //Oberschenkel
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        
        Model = Util::custom_rotate(Model, -armwinkel, glm::vec3(1.0, 0.0, 0.0));
        
        //Schienbein
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        
        //Fuss
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        Model = Util::custom_rotate(Model, alpha, glm::vec3(1.0, 0.0, 0.0));
        Model = Util::custom_rotate(Model, z_armwinkel, glm::vec3(0.0, 0.0, 1.0));
        sendMVP();
        
        Model = Save2;
        Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
        Model = glm::translate(Model, glm::vec3(x_pos_human, y_pos_human, z_pos_human));
        
        //left arm
        gap = -0.6;
        Model = Util::custom_rotate(Model, jumpwinkel, glm::vec3(1.0, 0.0, 0.0));
        
        Model = Util::custom_rotate(Model, beta, glm::vec3(1.0, 0.0, 0.0));
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        Model = Util::custom_rotate(Model, armwinkel, glm::vec3(1.0, 0.0, 0.0));
        
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        Model = glm::translate(Model, glm::vec3(0.0, (-ancle - length) * 2, 0));
        Model = Util::custom_rotate(Model, -beta, glm::vec3(1.0, 0.0, 0.0));
        
        
        Model = Save4;
        Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
        Model = glm::translate(Model, glm::vec3(x_pos_human, y_pos_human, z_pos_human));
        
        //right arm
        gap = 1.6;
        
        Model = Util::custom_rotate(Model, jumpwinkel, glm::vec3(1.0, 0.0, 0.0));
        
        Model = Util::custom_rotate(Model, -beta, glm::vec3(1.0, 0.0, 0.0));
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        Model = Util::custom_rotate(Model, armwinkel, glm::vec3(1.0, 0.0, 0.0));
        
        drawPart(length, thickness, gap);
        Model = glm::translate(Model, glm::vec3(0.0, length, 0.0));
        
        drawPart(ancle, ancle / 2, gap);
        Model = glm::translate(Model, glm::vec3(0.0, ancle, 0.0));
        Model = Util::custom_rotate(Model, beta, glm::vec3(1.0, 0.0, 0.0));
        sendMVP();


	    //Model = glm::mat4(1.0f);
        //glm::mat4 SaveO = Model;

        //if (camera.getThirdPersonMode()){
        //    //"Player"
        //    SaveO = Model;
        //    Model = glm::translate(Model, player.position);
        //    Model = glm::scale(Model, glm::vec3(1.f/5.f, 1.f/5.f, 1.f/5.f));
        //    sendMVP();
        //    drawSphere(10, 10);

        ////"Teapot"
        //Model = SaveO;

        //}
        //Model = glm::translate(Model, glm::vec3(1.5, 0.0, 0.0));
        //Model = glm::scale(Model, glm::vec3(1.0 / 1000.0, 1.0 / 1000.0, 1.0 / 1000.0));
        ////Transformationsmatrizen werden in Grafikkarte geschickt
        //sendMVP();
        //teapot.display();
        //Model = SaveO;

        //drawPlatform(p1.position);

        //Model = glm::translate(Model, p1.from);
        //drawCS();
        //Model = SaveO;
        //Model = glm::translate(Model, p1.to);
        //drawCS();
        //Model = SaveO;
        



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
