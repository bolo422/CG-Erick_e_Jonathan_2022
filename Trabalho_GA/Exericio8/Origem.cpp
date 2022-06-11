#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "ModelTransform.h"

#include <iostream>

enum transformationType
{
    position = 0,
    rotation,
    scale
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Transform
//ModelTransform* modelTransform;
int axisSelected = 0;
transformationType controlSelected = scale;
bool tabHold = false;
bool selected = false;
bool capsHold = false;

string clsAssist;

Model* selectedModel;


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(false);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    
    Shader defaultShader("../shaders/hello.vs", "../shaders/hello.fs");
    Shader selectedShader("../shaders/selected.vs", "../shaders/selected.fs");

    Shader ourShader = defaultShader;

    // load models
    // -----------
    Model ourModel("../Modelos/3D_Models/Pokemon/Pikachu.obj");
    ourModel.transform.setPosition(glm::vec3(-2.0f, 0.0f, 0.0f));
    Model ourModel2("../Modelos/3D_Models/Pokemon/PikachuF.obj");
    ourModel2.transform.setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    //Model ourModel3("../Modelos/3D_Models/Dispenser/source/cat toy.obj");
    //ourModel3.setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    vector < Model > models;
    models.push_back(ourModel);
    models.push_back(ourModel2);
    //.push_back(ourModel3);

    selectedModel =& models[1];
    //*modelTransform = selectedModel->transform;

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.setVec3("light.position", camera.Position);
        ourShader.setVec3("light.direction", camera.Front);
        ourShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setVec3("light.ambient", 3.0f, 3.0f, 3.0f); // 0.1f, 0.1f, 0.1f
        ourShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("light.constant", 1.0f);
        ourShader.setFloat("light.linear", 0.09f);
        ourShader.setFloat("light.quadratic", 0.032f);
        ourShader.setFloat("material.shininess", 50.0f); // 32.0f

        if (selected)
            ourShader = selectedShader;
        else
            ourShader = defaultShader;

        // don't forget to enable shader before setting uniforms
        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);

#pragma region model_update
       /* glm::mat4 teste = glm::mat4(models[1])*/

        
        

        //selectedModel->setPosition(modelTransform->getPosition());
        //selectedModel->setScale(modelTransform->getVecScale());


        //models[1] = glm::translate(models[1], modelTransform.getPosition()); // translate it down so it's at the center of the scene
        //models[1].model = glm::scale(models[1].model, modelTransform.getVecScale());	// it's a bit too big for our scene, so scale it down
        //models[1].model = glm::rotate(models[1].model, modelTransform.getAngle(0), glm::vec3(1.0f, 0.0f, 0.0f));
        //models[1].model = glm::rotate(models[1].model, modelTransform.getAngle(1), glm::vec3(0.0f, 1.0f, 0.0f));
        //models[1].model = glm::rotate(models[1].model, modelTransform.getAngle(2), glm::vec3(0.0f, 0.0f, 1.0f));

#pragma endregion

       /* ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);*/
        for (int i = 0; i < models.size(); i++)
        {
            models[i].update(ourShader);
            models[i].Draw(ourShader);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void print(string str)
{
    if (clsAssist != str)
    {
        system("CLS");
        cout << str;
        clsAssist = str;
    }
}

void transformObject(float value)
{
    if (selectedModel == NULL) return;

    if (!selected)
    {
        print("No model selected!\n");
        return;
    }

    switch (controlSelected)
    {
    case position:
        selectedModel->transform.addPosition(axisSelected, value * 0.5f);
        //modelTransform->addPosition(axisSelected, value * 0.5);

        break;
    case rotation:
        selectedModel->transform.addAngle(axisSelected, value * 0.5);
        //modelTransform->addAngle(axisSelected, value*0.5);
        break;
    case scale:
        selectedModel->transform.addScale(value * 0.2);
        //modelTransform->addScale(value*0.2);
        break;
    }
}

void resetModel() 
{
    selectedModel->transform = ModelTransform();
}



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
    {
        if (tabHold)
        {
            if (controlSelected == position)
            {
                controlSelected = rotation;
                print("Rotation selected\n");
            }
            else if (controlSelected == rotation)
            {
                controlSelected = scale;
                print("Scale selected\n");
            }
            else if (controlSelected == scale)
            {
                controlSelected = position;
                print("Position selected\n");
            }
            tabHold = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
        tabHold = true;
    }

    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_RELEASE)
    {
        if (capsHold)
        {
            selected = !selected;
            capsHold = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    {
        capsHold = true;
    }

    
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        axisSelected = 0;
        print("X axis selected\n");
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        axisSelected = 1;
        print("Y axis selected\n");
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        axisSelected = 2;
        print("Z axis selected\n");
    }

    
    if (glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS)
        transformObject(0.1); //ang++
    if (glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS)
        transformObject(-0.1); //ang--
      
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        resetModel();
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
