#include "MVP.hpp"

static GLuint programID;
static glm::mat4 Model;
static glm::mat4 View;
static glm::mat4 Projection;

void MVP::initialize(GLuint programID){
    ::programID=programID;
    Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    Model = glm::mat4(1.f);

}

void MVP::setModel(glm::mat4 Model) {
    ::Model=Model;
    
    glm::mat4 MVP = Projection * View * Model;
//    std::cout << &MVP << std::endl;
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, konstant fuer alle Eckpunkte
    glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    
    glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &::Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}

void MVP::setView(glm::mat4 View){
    ::View=View;
    
    glm::mat4 MVP = Projection * View * Model;
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, konstant fuer alle Eckpunkte
    glUniformMatrix4fv(glGetUniformLocation(programID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    
    glUniformMatrix4fv(glGetUniformLocation(programID, "M"), 1, GL_FALSE, &Model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "V"), 1, GL_FALSE, &::View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programID, "P"), 1, GL_FALSE, &Projection[0][0]);
}
