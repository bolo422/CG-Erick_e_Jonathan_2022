#pragma once

//#include <iostream>
//#include <string>
//#include <assert.h>
//// GLAD
//#include <glad/glad.h>
//
//// GLFW
//#include <GLFW/glfw3.h>
//
////GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include "Model.h"
#include "Camera.h"



using namespace std;


class GameManager
{
public:
	GameManager() {}
	~GameManager() {}
	
	void Start();
	void Update();
	void Finish();

	// Protótipo da função de callback de teclado
	//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {}
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


	void processInput(GLFWwindow* window);

	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


protected:

	const GLuint WIDTH = 800, HEIGHT = 600;
	bool rotateX, rotateY, rotateZ;// , firstMouse;
	int viewID = 1;
	/*glm::vec3 cameraPos, cameraFront, cameraUp;
	float lastX, lastY, yaw, fov, pitch;*/
	GLFWwindow* window;
	//const GLubyte* renderer, version;
	int width, height;
	Shader* shader = 0;
	Model* Model1 = 0;
	GLuint VAO = 0;
	glm::mat4 model;
	GLint modelLoc;
	glm::mat4 view;
	GLint viewLoc;
	glm::mat4 projection;
	GLint projectionLoc;



};

