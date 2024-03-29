#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader/shaderHandler.h"
#include "camera/camera.h"
#include "renderer/renderer.h"
#include "world/chunk.h"
#include "world/world.h"
#include "../utils/vertexData.h"

#include <iostream>

bool wireframe = false;

static int oldState = GLFW_RELEASE;

// camera
Camera camera(glm::vec3(0.0f, 129.0f, 0.0f));
float lastX = 1920 / 2.0f;
float lastY = 1080 / 2.0f;
bool firstMouse = true;

World* worldMap;


void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);


// timing
float deltaTime = 0.0f;	//tempo tra il frame corrente e quello passato
float lastFrame = 0.0f;

int main()
{
    // Inizializzo glfw
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // creo la finestra con glfw
    // --------------------
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "OpenGL - Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Impossibile creare la finestra";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwMaximizeWindow(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Carico la robaccia di glad
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Impossibile inizializzare glad";
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    //debug shader
    Shader debugQuad("./shader/shaders/quadVert.glsl", "./shader/shaders/quadFrag.glsl");

    Shader shaderManager("./shader/shaders/basicVert.glsl", "./shader/shaders/basicFrag.glsl");
    shaderManager.loadTexture("./res/skinAtlas.png");

    Shader skyboxShader("./shader/shaders/skyboxVert.glsl", "./shader/shaders/skyboxFrag.glsl");
    unsigned int cubemapTexture = skyboxShader.loadSkyboxTextures(faces);

    Shader depthMapShader("./shader/shaders/depthVert.glsl", "./shader/shaders/depthFrag.glsl");


    Renderer skyboxRenderer(&camera, &skyboxShader);
    skyboxRenderer.setSkyboxBuffer();
    
    worldMap = new World(&camera, &shaderManager, &depthMapShader, &debugQuad);
    worldMap->generateWorldMesh();

    
    debugQuad.use();
    debugQuad.setInt("depthMap", 0);

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);

    shaderManager.use();
    shaderManager.setInt("texture", 0);
    shaderManager.setInt("shadowMap", 1);


    glm::vec3 lightDir(0.2f, -1.0f, 0.2f);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {

        // logica per frame time (in base al frame time decido poi quanto muovere la camera)
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        worldMap->renderWorld(lightDir);
        skyboxRenderer.renderSkybox(cubemapTexture);

        // Swappa i buffer e mette in coda gli eventi
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // termina e disalloca tutte le risorse di glfw
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    if (!wireframe && glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        wireframe = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (wireframe && glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        wireframe = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }



    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(UP, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }

    //Mouse click
    int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    if (newState == GLFW_PRESS && oldState == GLFW_RELEASE) {
        worldMap->LaunchCollisionRay();
    }
    oldState = newState;  
}

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
    float yoffset = lastY - ypos; // rovesciato, le coordinate vanno da bottom a top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}