#include "Object.h"

Object::Object()
{
}

Object::Object(string _tag, glm::vec3 _position, glm::vec3 _scale, GLuint _texID, Shader* _shader)
{
	initialize();
	tag = _tag;
	position = _position;
	scale = _scale;
	texID = _texID;
	shader = _shader;
}

Object::~Object()
{
}

void Object::initialize()
{
	GLuint VBO, EBO;

	float vertices[] = {
		// positions		// colors			// texture coords
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	1.0f, 1.0, // top right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, 1.0 // top left
	};
	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //unbind

	transform = glm::mat4(1);
	position = glm::vec3(0.0, 0.0, 0.0);
	scale = glm::vec3(1.0, 1.0, 1.0);
	angle = 0.0;
	shader = NULL;
	texID = 0;

	first = true;
}

void Object::update()
{
	shader->Use();

	//atualizar a matriz de transformação do objeto
	transform = glm::mat4(1);
	setTranslation(position);
	setRotation(angle, glm::vec3(0.0, 0.0, 1.0));
	setScale(scale);

	shader->setMat4("model", glm::value_ptr(transform));
}

void Object::draw()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texID);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}



void Object::setRotation(float _angle, glm::vec3 _axis, bool _reset)
{
	if (_reset) {
		transform = glm::mat4(1); //inicializa com matriz identidade
	}
	else {
		transform = glm::rotate(transform, _angle, _axis);
	}
}

void Object::setTranslation(glm::vec3 _displacements, bool _reset)
{
	if (_reset) {
		transform = glm::mat4(1); //inicializa com matriz identidade
	}
	else {
		transform = glm::translate(transform, _displacements);
	}
}

void Object::setScale(glm::vec3 _scaleFactors, bool _reset)
{
	if (_reset) {
		transform = glm::mat4(1); //inicializa com matriz identidade
	}
	else {
		transform = glm::scale(transform, _scaleFactors);
	}
}
