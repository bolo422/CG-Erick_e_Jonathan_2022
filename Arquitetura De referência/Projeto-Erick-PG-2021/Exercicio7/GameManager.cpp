#include "GameManager.h"

int space = 0;
int menuSpace = 0;
int paused = 0;

void GameManager::initialize()
{

	// Inicializa√ß√£o da GLFW
	glfwInit();

	// Cria√ß√£o da janela GLFW
	window = glfwCreateWindow(WIDTH, HEIGHT, "Bunny Girl", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun√ß√£o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun√ß√µes da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa√ß√µes de vers√£o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;


	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Definindo as dimens√µes da viewport com as mesmas dimens√µes da janela da aplica√ß√£o
	//int width, height;

	shader = new Shader("./shaders/transforms.vs", "./shaders/transforms.fs");
	spriteShader = new Shader("./shaders/transforms.vs", "./shaders/animatedsprites.fs");



	shader->Use();


	projection = glm::mat4(1);
	double xmin = 0.0, xmax = 1024.0, ymin = 0.0, ymax = 768.0;

	projection = glm::ortho(xmin, xmax, ymin, ymax, -1.0, 1.0);

	projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


	glm::mat4 model = glm::mat4(1);


	model = glm::translate(model, glm::vec3(400, 300, 0.0));
	model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(960.0f, 1280.0f, 1.0));

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");

	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);


	
	//testes com o spriteShader

	/*modelLoc = glGetUniformLocation(spriteShader->Program, "model");
	glUniform1i(glGetUniformLocation(spriteShader->Program, "ourTexture1"), 0);*/



	createObjects();



	speedTimer.setInitialTime(500);
	scoreTimer.setInitialTime(80);

}

void GameManager::restart()
{
	speed = 0;
	holding = false;
	jumpForce = 0;
	endpulo = 0;
	score = 0;

	for (int i = 0; i < 10; i++)
	{
		numbers[i]->setTexture(loadTexture("./textures/Numbers/n.png"));
	}


	createObjects();

	speedTimer.setInitialTime(1500);
	scoreTimer.setInitialTime(80);
}

void GameManager::assignTextures()
{
}

void GameManager::createObjects()
{

	// CriaÁ„o dos objetos do jogo


	//Background
	Object *obj = new Object; // inicializaÁ„o da vari·vel tempor·ria que armazena o objeto antes de coloc·-lo no vetor de objetos

	texID = loadTexture("./textures/Background/background.png"); // carrega a textura que ser· colocada na criaÁ„o do objeto
	obj = new Object("bg", glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader); // criaÁ„o do objeto
	objects.push_back(obj); // coloca o objeto na prÛxim posi„o do vetor de objetos
	obj = new Object("bg", glm::vec3(WIDTH * 1.5, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader);
	objects.push_back(obj);


	//MiddleGround
	texID = loadTexture("./textures/Background/middleground.png");
	obj = new Object("mg", glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader);
	objects.push_back(obj);
	obj = new Object("mg", glm::vec3(WIDTH * 1.5, HEIGHT / 2, 0), glm::vec3(1024, 768, 1.0), texID, shader);
	objects.push_back(obj);

	//Ground
	texID = loadTexture("./textures/Background/ground.png");
	obj = new Object("ground", glm::vec3(WIDTH / 2, 15, 0), glm::vec3(1024, 30, 1.0), texID, shader);
	objects.push_back(obj);
	obj = new  Object("ground", glm::vec3(WIDTH * 1.5, 15, 0), glm::vec3(1024, 30, 1.0), texID, shader);
	objects.push_back(obj);

	//Enemy
	texID = loadTexture("./textures/Enemy/bee-1.png");
	enemy = Object("enemy", glm::vec3(1500, 80, 0), glm::vec3(74, 64, 1.0), texID, shader);

	//Player
	texID = loadTexture("./textures/Player/skip.png");
	player.initialize(texID,1,8);
	player.setTag("player");
	player.setPosition(glm::vec3(200, 62, 0));
	player.setDimensions(glm::vec3(74, 64, 1.0));
	player.setAngle(glm::radians(0.0f));
	//player.setTexture(texID);
	player.setShader(shader);
	// ERRO -> N√O DESENHA A SPRITE / SPRITE INVISÕVEL
	//player.setShader(spriteShader);

	//MenuArt
	texID = loadTexture("./textures/menuart.png");
	obj = new Object("menuArt", glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(997, 501, 1.0), texID, shader);
	menuArt.push_back(obj);

	texID = loadTexture("./textures/menuart2.png");
	obj = new Object("menuArt", glm::vec3(WIDTH / 2, HEIGHT / 2-270, 0), glm::vec3(997/2, 501/2, 1.0), texID, shader);
	menuArt.push_back(obj);

	texID = loadTexture("./textures/menuart3.png");
	obj = new Object("menuArt", glm::vec3(WIDTH / 2, HEIGHT / 2, 0), glm::vec3(997, 501, 1.0), texID, shader);
	menuArt.push_back(obj);

	texID = loadTexture("./textures/menuart4.png");
	obj = new Object("menuArt", glm::vec3(WIDTH / 2, HEIGHT / 2 - 270, 0), glm::vec3(997 / 2, 501 / 2, 1.0), texID, shader);
	menuArt.push_back(obj);

	//Numbers
	if (numbers.size() < 10) { //Evita que n˙meros sejam criados novamente apÛs gameover
		for (int i = 0; i < 10; i++)
		{
			texID = loadTexture("./textures/Numbers/n.png");
			obj = new Object("numbers", glm::vec3(WIDTH / 2 - WIDTH / 4 + 30 * i, HEIGHT / 2 + 200, 0), glm::vec3(32, 33, 1.0), texID, shader);
			numbers.push_back(obj);
		}
	}



}

void GameManager::generateEnemy()
{
	srand(time(NULL));
	switch (rand() % 3 + 1)
	{
	case 1:
		texID = loadTexture("./textures/Enemy/bee-1.png");
		enemy = Object("enemy", glm::vec3(1100, 90, 0), glm::vec3(64, 64, 1.0), texID, shader);
		break;
	case 2:
		texID = loadTexture("./textures/Enemy/bee-2.png");
		enemy = Object("enemy", glm::vec3(1100, 120, 0), glm::vec3(64, 64, 1.0), texID, shader);
		break;
	case 3:
		texID = loadTexture("./textures/Enemy/slug-1.png");
		enemy = Object("enemy", glm::vec3(1100, 51, 0), glm::vec3(64, 42, 1.0), texID, shader);
		break;

	default:
		break;
	}
}

void GameManager::drawEnvironment()
{

		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->update();
			objects[i]->draw();
		}

		for (int i = 0; i < 10; i++)
		{
			numbers[i]->update();
			numbers[i]->draw();

		}

		if(!pause){
		//Movimento MG
		{
			objects[2]->addPositionX(-2 - speed);
			objects[3]->addPositionX(-2 - speed);

			if (objects[2]->getPosition().x <= -512) {
				objects[2]->setPositionX(WIDTH * 1.5);
			}

			if (objects[3]->getPosition().x <= -512) {
				objects[3]->setPositionX(WIDTH * 1.5);
			}

			if (objects[2]->getPosition().x - objects[3]->getPosition().x != -1024 && objects[2]->getPosition().x < objects[3]->getPosition().x)
			{
				objects[2]->setPositionX(512);
				objects[3]->setPositionX(1536);
			}
			else if (objects[3]->getPosition().x - objects[2]->getPosition().x != -1024 && objects[3]->getPosition().x < objects[2]->getPosition().x)
			{
				objects[2]->setPositionX(512);
				objects[3]->setPositionX(1536);
			}
		}

		//Movimento BG
		{
			objects[0]->addPositionX(-0.5 - speed);
			objects[1]->addPositionX(-0.5 - speed);

			if (objects[0]->getPosition().x <= -512) {
				objects[0]->setPositionX(WIDTH * 1.5);
			}

			if (objects[1]->getPosition().x <= -512) {
				objects[1]->setPositionX(WIDTH * 1.5);
			}

			if (objects[0]->getPosition().x - objects[1]->getPosition().x != -1024 && objects[0]->getPosition().x < objects[1]->getPosition().x)
			{
				objects[0]->setPositionX(512);
				objects[1]->setPositionX(1536);
			}
			else if (objects[1]->getPosition().x - objects[0]->getPosition().x != -1024 && objects[1]->getPosition().x < objects[0]->getPosition().x)
			{
				objects[0]->setPositionX(512);
				objects[1]->setPositionX(1536);
			}
		}

		//Movimento Ground
		{
			objects[4]->addPositionX(-3 - speed);
			objects[5]->addPositionX(-3 - speed);

			if (objects[4]->getPosition().x <= -512) {
				objects[4]->setPositionX(WIDTH * 1.5);
			}

			if (objects[5]->getPosition().x <= -512) {
				objects[5]->setPositionX(WIDTH * 1.5);
			}

			if (objects[4]->getPosition().x - objects[5]->getPosition().x != -1024 && objects[4]->getPosition().x < objects[5]->getPosition().x)
			{
				objects[4]->setPositionX(512);
				objects[5]->setPositionX(1536);
			}
			else if (objects[5]->getPosition().x - objects[4]->getPosition().x != -1024 && objects[5]->getPosition().x < objects[4]->getPosition().x)
			{
				objects[4]->setPositionX(512);
				objects[5]->setPositionX(1536);
			}
		}
	}
}

void GameManager::write(string number)
{
	int stringSize = number.length();

	for (int i = 0; i < number.length(); i++)
	{
		numbers[i]->setTexture(loadTexture("./textures/Numbers/" + to_string((int)number[i] - 48) + ".png"));
	}
	
}



void GameManager::run()
{
	while (!glfwWindowShouldClose(window)) {
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun√ß√µes de callback correspondentes
		glfwPollEvents();
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		//glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

		if (paused == 1 && !gameOver) {
			pause = !pause;
			paused = 0;
		}

		drawEnvironment();
		enemy.update();
		enemy.draw();

		write(to_string(score));

		//spriteShader->Use();
		//spriteShader->setMat4("projection", glm::value_ptr(ortho));
		player.update();
		player.draw();

		switch (scene)
		{
		case Menu:

			menuArt[0]->update();
			menuArt[0]->draw();
			menuArt[1]->update();
			menuArt[1]->draw();
			//pause = true;
			if (menuSpace == 1) {
				//pause = false;
				menuSpace == 0;
				scene = Gameplay;
			}

			break;

		case Gameplay:

				if (!pause) {
					if (CheckCollision(player, enemy)) {
						cout << "colis„o\n";
						gameOver = true;
						menuSpace = 0;
						scene = Gameover;
					}

					if (enemy.getPosition().x < -530)
						generateEnemy();
					else
						enemy.addPositionX(-6 - speed);

					switch (space)
					{
					case 0: break;
					case 1: jumpForce += 4; break;
					case 2: endpulo = jumpForce; jumpForce = 0; space = 0; player.jump(true); player.setEndJump(false);  break;
					}

					if (endpulo > 0 && !player.getEndJump()) {
						player.jump(endpulo, 1.5);
					}

					scoreTimer.tick();
					if (scoreTimer.over())
						score++;

					speedTimer.tick();
					if (speedTimer.over()) {
						speed++;
						speedTimer.restart(speedTimer.getInitialTime() * 0.95);
						cout << "Incremento de tempo : Speed = " << speed << " Ticks: " << speedTimer.getRemainingTime() << endl;
					}
				}
			break;


		case Gameover:
			pause = true;
			menuArt[2]->update();
			menuArt[2]->draw();
			menuArt[3]->update();
			menuArt[3]->draw();

			if (menuSpace == 1) {
				pause = false;
				gameOver = false;
				menuSpace == 0;
				player.resetJump();
				objects.clear();
				menuArt.clear();

				scene = Gameplay;
				restart();
			}

			break;

		}

		// Troca os buffers da tela
		glfwSwapBuffers(window);

	}
}

void GameManager::finish()
{
	glfwTerminate();
}

void GameManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		paused = 1;
	//   ATEN«√O!!!!!!!!!!
	//   Implementar estes ifs no inÌcio do update e lembrar de voltar o array de keys para 0!!!!



	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		space = 1;
		
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
		space = 2;
		menuSpace = 1;
	}
		
	
}

int GameManager::loadTexture(string path)
{
	GLuint texID;

	//identificador de textura da mem√≥ria
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Ajusta os par√¢metros de wrapping e filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregamento da imagem
	int width, height, nrChannels;
	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

bool GameManager::CheckCollision(Object& one, Object& two)
{
	// collision x-axis?
	bool collisionX = one.getPosition().x + one.getDimesions().x * 0.70  >= two.getPosition().x &&
		two.getPosition().x + two.getDimesions().x >= one.getPosition().x;
	// collision y-axis?
	bool collisionY = one.getPosition().y + one.getDimesions().y * 0.70 >= two.getPosition().y &&
		two.getPosition().y + two.getDimesions().y * 0.70 >= one.getPosition().y;
	// collision only if on both axes
	return collisionX && collisionY;
}




