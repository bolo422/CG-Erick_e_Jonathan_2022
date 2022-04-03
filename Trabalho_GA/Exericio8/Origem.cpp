/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico - Jogos Digitais - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 02/03/2022
 *
 */

#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();

void processInput(GLFWwindow *window);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;


bool rotateX=false, rotateY=false, rotateZ=false;

int viewID = 1;

glm::vec3 cameraPos, cameraFront, cameraUp;

bool firstMouse = true;
float lastX = WIDTH / 2.0;
float lastY = HEIGHT / 2.0;
float yaw = -90.0, pitch = 0.0;
float fov = 45.0f;

// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader = Shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	GLuint VAO = setupGeometry();


	glUseProgram(shader.ID);


	//Inicializa��o dos par�metros da c�mera
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // ponto
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // vetor
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");


	//View Matrix: posi��o e oritena��o da c�mera
	glm::mat4 view = glm::mat4(1); //matriz identidade;
	view = glm::lookAt(	cameraPos, //eye = posi��o da camera
						glm::vec3(	0.0f, 0.0f, 0.0f	), //center = ponto a ser observado (objetivo)
						cameraUp); //up
	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

	
	//Poderia j� colocar a posi��o que o view iniciaria


	//Projection Matrix: tipo de proje��o: ortogr�fica ou perspectiva
	glm::mat4 projection = glm::mat4(1); //matriz identidade;
	GLint projectionLoc = glGetUniformLocation(shader.ID, "projection");
	
	projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -3.0f, 3.0f);
	
	glUniformMatrix4fv(projectionLoc, 1, FALSE, glm::value_ptr(projection));


	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);


	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		processInput(window);

		// Limpa o buffer de cor
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(15);

		float angle = (GLfloat)glfwGetTime();

		model = glm::mat4(1); 
		if (rotateX)
		{
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
			
		}
		else if (rotateY)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		}
		else if (rotateZ)
		{
			model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));

		}

		// EXERC�CIO 2 - MUDAR VIEW
		glm::mat4 view = glm::mat4(1);
		switch (viewID)
		{
		case 1:
			view = glm::lookAt(glm::vec3(0.0f, 1.0f, 3.0f), 
				glm::vec3(0.0f, 0.0f, 0.0f), 
				glm::vec3(0.0f, 1.0f, 0.0f)); 
			break;
		case 2:
			view = glm::lookAt(glm::vec3(0.0f, 1.0f, -3.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 3:
			view = glm::lookAt(glm::vec3(3.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 4:
			view = glm::lookAt(glm::vec3(-3.0f, 1.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		case 5:
			view = glm::lookAt(glm::vec3(0.0f, 3.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		default:
			break;
		}

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLoc, 1, FALSE, glm::value_ptr(projection));
		

		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 42);

		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		
		glDrawArrays(GL_POINTS, 0, 42);
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		viewID = 1;
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		viewID = 2;
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		viewID = 3;
	}

	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		viewID = 4;
	}

	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		viewID = 5;
	}



}

// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {

		//Base da pir�mide: 2 tri�ngulos
		//x    y    z    r    g    b

		// EXERC�CIO 1 - TRANSFORMAR TRI�NGULO EM CUBO E CRIAR CH�O

		//CUBO
			-0.5, -0.5, -0.5,		0.6, 0.6, 0.9, //0 -> LADO 1
			-0.5, -0.5,  0.5,		0.6, 0.6, 0.9,
			0.5, -0.5, -0.5,		0.6, 0.6, 0.9,

			-0.5, -0.5, 0.5,		0.6, 0.6, 0.9, //1 - LADO 1
			0.5, -0.5,  0.5,		0.6, 0.6, 0.9,
			0.5, -0.5, -0.5,		0.6, 0.6, 0.9,

/* ------------------------------------------------------------------------- */

			-0.5, -0.5, 0.5,		0.5, 0.1, 0.6, //4 -> LADO 2
			0.5, 0.5,  0.5,			0.5, 0.1, 0.6, //
			0.5, -0.5, 0.5,			0.5, 0.1, 0.6,

			-0.5, 0.5, 0.5,			0.5, 0.1, 0.6, // 8 -> LADO 2
			0.5,  0.5,  0.5,		0.5, 0.1, 0.6, //
			-0.5, -0.5, 0.5,		0.5, 0.1, 0.6,

/* ------------------------------------------------------------------------- */

			-0.5, 0.5, -0.5,		0.4, 0.6, 0.1, //10 -> LADO 3
			-0.5, 0.5,  0.5,		0.4, 0.6, 0.1, //
			 0.5, 0.5, -0.5,		0.4, 0.6, 0.1,

			-0.5, 0.5, 0.5,			0.4, 0.6, 0.1, //11 -> LADO 3
			 0.5, 0.5,  0.5,		0.4, 0.6, 0.1,//
			 0.5, 0.5, -0.5,		0.4, 0.6, 0.1,

/* ------------------------------------------------------------------------- */

			-0.5, -0.5, -0.5,		0.7, 0.2, 0.1, //2 -> LADO 4
			-0.5,  0.5, -0.5,		0.7, 0.2, 0.1, //
			0.5, -0.5, -0.5,		0.7, 0.2, 0.1,

			0.5, -0.5, -0.5,		0.7, 0.2, 0.1, // 6 -> LADO 4
			0.5,  0.5,  -0.5,		0.7, 0.2, 0.1, //
			-0.5, 0.5, -0.5,		0.7, 0.2, 0.1,

/* ------------------------------------------------------------------------- */

			0.5, -0.5, 0.5,			0.2, 0.6, 0.7, // 5 -> LADO 5
			0.5,  0.5,  -0.5,		0.2, 0.6, 0.7, //
			0.5, -0.5, -0.5,		0.2, 0.6, 0.7,

			0.5, -0.5, 0.5,			0.2, 0.6, 0.7, // 7 -> LADO 5
			0.5,  0.5,  0.5,		0.2, 0.6, 0.7, //
			0.5, 0.5, -0.5,			0.2, 0.6, 0.7,

/* ------------------------------------------------------------------------- */

			-0.5, -0.5, -0.5,		0.4, 0.1, 0.6, //3 -> LADO 6
			-0.5,  0.5,  -0.5,		0.4, 0.1, 0.6, //
			-0.5, -0.5, 0.5,		0.4, 0.1, 0.6,

			-0.5, 0.5, 0.5,			0.4, 0.1, 0.6, // 9 -> LADO 6
			-0.5,  -0.5,  0.5,		0.4, 0.1, 0.6, //
			-0.5, 0.5, -0.5,		0.4, 0.1, 0.6,



	//			CH�O		
/* ------------------------------------------------------------------------- */

			-1, -0.51, 1,		1.0, 0, 0.2, //
			1,   -0.51, -1,		1.0, 0, 0.2, //
			-1, -0.51, -1,		1.0, 0, 0.2,

			1, -0.51, 1,		1.0, 0, 0.2, //
			-1,  -0.51, 1,		1.0, 0, 0.2, //
			1, -0.51, -1,		1.0, 0, 0.2,
			


	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void processInput(GLFWwindow *window)
{
	glfwPollEvents(); //verificar as callbacks de input

	float cameraSpeed = 0.05;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //movimento para frente
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //movimento para tr�s
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //movimento para esquerda
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //movimento para direita
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= (yoffset*0.1f);
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;

	//cout << "scrolling!\n";
}

