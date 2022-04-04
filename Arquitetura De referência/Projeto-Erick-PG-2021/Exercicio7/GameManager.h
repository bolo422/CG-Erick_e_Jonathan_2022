#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>
#include <chrono>
#include <ctime> 
#include <cstdlib>
#include <map>


// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H  


// stb
#include "stb_image.h"

#include <vector>

//#include "Shader.h"
#include "Player.h"
#include "Timer.h"



using namespace std;
enum Scene{Menu, Gameplay, Gameover};


class GameManager
{
public:
	GameManager() {}
	~GameManager(){}

	void initialize();
	void restart();
	void assignTextures();
	void createObjects();
	void generateEnemy();
	void drawEnvironment();
	void write(string number);

	void run();
	void finish();


	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

	int loadTexture(string path);

	bool CheckCollision(Object& one, Object& two); // AABB - AABB collision




protected:
	Scene scene = Menu;
	



	Timer speedTimer, scoreTimer;
	float speed = 0;

	GLFWwindow* window = 0;
	int width = 0, height = 0;
	Shader* shader = 0;
	Shader* spriteShader = 0;

	glm::mat4 projection;
	GLint projLoc = 0;

	//Objects and Textures
	GLuint texID = 0;
	vector <Object*> objects;
	vector <Object*> menuArt;
	vector <Object*> numbers;
	Object enemy;
	Player player;

	int score = 0;

	bool holding = false;
	int jumpForce = 0;
	int endpulo = 0;
	bool pause = false;
	bool gameOver = false;

	const GLuint WIDTH = 1024, HEIGHT = 768;
	const float pi = 3.14159;
};

