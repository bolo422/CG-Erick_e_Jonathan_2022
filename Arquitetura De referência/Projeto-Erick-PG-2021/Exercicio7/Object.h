#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

using namespace std;

class Object
{
public:

	Object();
	Object(string _tag, glm::vec3 _position, glm::vec3 _scale, GLuint _texID, Shader* _shader);
	~Object();

	//equivalente ao setupGeometry
	void initialize(); 
	//atualizar a matriz de transformação do objeto
	void update();
	
	void draw();
	//Setar posição
	inline void setPosition(glm::vec3 _position) { position = _position; }
	inline void setPosition(float x, float y, float z) { position = glm::vec3(x,y,z); }
	inline void setPositionX(float x) { position = glm::vec3(x,position.y,position.z); }
	inline void setPositionY(float y) { position = glm::vec3(position.y, y, position.z); }
	//Incremento de vetor de posição
	inline void addPositionX(float x) { position = glm::vec3(position.x + x, position.y, position.z); }
	inline void addPositionY(float y) { position = glm::vec3(position.x, position.y + y, position.z); }
	//Decremento de valor de posição
	inline void removePositionX(float x) { position = glm::vec3(position.x - x, position.y, position.z); }
	inline void removePositionY(float y) { position = glm::vec3(position.x, position.y - y, position.z); }


	
	//Get posição
	inline glm::vec3 getPosition() { return position; }

	//Setar dimensões
	inline void setDimensions(glm::vec3 _scale) { scale = _scale; }
	inline glm::vec3 getDimesions(){ return scale; }

	//Setar angulo
	inline void setAngle(float _angle) { angle = _angle; }
	//Setar textura
	inline void setTexture(GLuint _texID) { texID = _texID; }
	//Setar shader
	inline void setShader(Shader* _shader) { shader = _shader; }

	inline void setTag(string _tag) { tag = _tag; }
	inline string getTag() { return tag; }
	
protected:
	string tag;

	//Setar rotação
	void setRotation(float _angle, glm::vec3 _axis, bool _reset = false);
	//Setar translação
	void setTranslation(glm::vec3 _displacements, bool _reset = false);
	//Setar escala
	void setScale(glm::vec3 _scaleFactors, bool _reset = false);

	//buffer de geometria
	GLuint VAO; 
	//matriz de transformação
	glm::mat4 transform; 
	//id de textura
	GLuint texID; 
	//ponteiro para o shader
	Shader* shader; 

	glm::vec3 position, scale;
	float angle;

	bool first;


	
};

