#include "GameManager.h"

int w = 0, a = 0, s = 0, d = 0;
glm::vec3 cameraPos, cameraFront, cameraUp;
float lastX, lastY, yaw, fov, pitch;
bool firstMouse;

void GameManager::Start()
{
	// Dimensões da janela (pode ser alterado em tempo de execução)
	rotateX = false, rotateY = false, rotateZ = false;
	viewID = 1;
	firstMouse = true;
	lastX = WIDTH / 2.0;
	lastY = HEIGHT / 2.0;
	yaw = -90.0, pitch = 0.0;
	fov = 45.0f;

	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	//renderer = glGetString(GL_RENDERER); /* get renderer string */
	//version = glGetString(GL_VERSION); /* version as a string */
	//cout << "Renderer: " << renderer << endl;
	//cout << "OpenGL version supported " << version << endl;

	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	shader = new Shader("../shaders/hello.vs", "../shaders/hello.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	VAO = setupGeometry();


	//glUseProgram(shader.ID);
	shader->Use();


	//Inicialização dos parâmetros da câmera
	cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); // ponto
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f); // vetor
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	model = glm::mat4(1); //matriz identidade;
	modelLoc = glGetUniformLocation(shader->ID, "model");


	//View Matrix: posição e oritenação da câmera
	view = glm::mat4(1); //matriz identidade;
	view = glm::lookAt(cameraPos, //eye = posição da camera
		glm::vec3(0.0f, 0.0f, 0.0f), //center = ponto a ser observado (objetivo)
		cameraUp); //up
	viewLoc = glGetUniformLocation(shader->ID, "view");
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));


	//Poderia já colocar a posição que o view iniciaria


	//Projection Matrix: tipo de projeção: ortográfica ou perspectiva
	projection = glm::mat4(1); //matriz identidade;
	projectionLoc = glGetUniformLocation(shader->ID, "projection");

	projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
	//projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -3.0f, 3.0f);

	glUniformMatrix4fv(projectionLoc, 1, FALSE, glm::value_ptr(projection));


	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	glEnable(GL_DEPTH_TEST);





}

void GameManager::Update()
{
	while (!glfwWindowShouldClose(window)) {
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
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

		// EXERCÍCIO 2 - MUDAR VIEW
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
}

void GameManager::Finish()
{
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
}

int GameManager::setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo e as armazenamos de forma
	// sequencial, já visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do vértice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO único ou em VBOs separados
	GLfloat vertices[] = {

		//Base da pirâmide: 2 triângulos
		//x    y    z    r    g    b

		// EXERCÍCIO 1 - TRANSFORMAR TRIÂNGULO EM CUBO E CRIAR CHÃO

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



																		 //			CHÃO		
																	 /* ------------------------------------------------------------------------- */

																				 -1, -0.51, 1,		1.0, 0, 0.2, //
																				 1,   -0.51, -1,		1.0, 0, 0.2, //
																				 -1, -0.51, -1,		1.0, 0, 0.2,

																				 1, -0.51, 1,		1.0, 0, 0.2, //
																				 -1,  -0.51, 1,		1.0, 0, 0.2, //
																				 1, -0.51, -1,		1.0, 0, 0.2,



	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

void GameManager::processInput(GLFWwindow* window)
{

	glfwPollEvents(); //verificar as callbacks de input

	float cameraSpeed = 0.05;
	if (w == 1) //movimento para frente
		cameraPos += cameraSpeed * cameraFront;
	if (s == 1) //movimento para trás
		cameraPos -= cameraSpeed * cameraFront;
	if (a == 1) //movimento para esquerda
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (d == 1) //movimento para direita
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;


}

void GameManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
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

void GameManager::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= (yoffset * 0.1f);
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;

	//cout << "scrolling!\n";
}


void GameManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		w = 1;
	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		w = 0;
	//   ATENÇÃO!!!!!!!!!!
	//   Implementar estes ifs no início do update e lembrar de voltar o array de keys para 0!!!!

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		a = 1;
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		a = 0;

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		s = 1;
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		s = 0;

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		d = 1;
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		d = 0;

}