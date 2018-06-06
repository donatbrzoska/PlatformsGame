#pragma once
class Obj3D
{
	//Memberdaten
	GLuint VertexArrayID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	GLuint vertexbuffer;
	GLuint normalbuffer;
	GLuint uvbuffer;

public:
	Obj3D(const char* fn); //Konstruktor
	void display();
	~Obj3D();	//Destruktor
};