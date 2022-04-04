#include "Sprite.h"

void Sprite::initialize(GLuint _texID, int _nAnimations, int _nFrames)
{
	texID = _texID;
	nAnimations = _nAnimations;
	nFrames = _nFrames;

	iFrame = 0;
	iAnimation = 0;

	dx = 1.0 / (float)nFrames;
	dy = 1.0 / (float)nAnimations;

	float vertices[] = {
		// positions		// colors			// texture coords
		0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	dx, dy, // top right
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	dx, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	0.0f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	0.0f, dy // top left
	};

	unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3 // second triangle
	};

	GLuint VBO, EBO;

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


}

void Sprite::update()
{
	shader->Use();

	//atualizar a matriz de transformação do objeto
	transform = glm::mat4(1);
	setTranslation(position);
	setRotation(angle, glm::vec3(0.0, 0.0, 1.0));
	setScale(scale);

	

	shader->setMat4("model", glm::value_ptr(transform));
	GLint offsetLoc = glGetUniformLocation(shader->Program, "offset");
	glUniform2f(offsetLoc, iFrame * dx, iAnimation * dy);
}


