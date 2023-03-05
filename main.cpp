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

#include <iostream>

bool wireframe = false;

static int oldState = GLFW_RELEASE;
float mesh[300000000];

// camera
Camera camera(glm::vec3(0.0f, 65.0f, 3.0f));
float lastX = 1920 / 2.0f;
float lastY = 1080 / 2.0f;
bool firstMouse = true;

//Chunk chunk(0, 0);
World worldMap(mesh);


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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    Shader shaderManager("./shader/shaders/basicVert.glsl", "./shader/shaders/basicFrag.glsl", "./res/skinAtlas.jpg");

    Renderer renderer(&camera, &shaderManager);


    //==== Start TMP - Test Only ==========\\

    //====== End TMP - Test Only ==========\\

    //std::vector<float> Mesh = chunk.generateChunkMesh();
    int faceInMesh = worldMap.getWorldMesh();
    std::cout << faceInMesh;
    renderer.setBuffer(mesh, faceInMesh);
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


        shaderManager.setInt("texture", 0);
        shaderManager.use();

        renderer.renderMesh(faceInMesh);
        //break;


        /*
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)1920 / (float)1080, 0.1f, 100.0f);
        shaderManager.setMat4("projection", projection);

        glm::mat4 view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
        shaderManager.setMat4("view", view);


        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            //model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shaderManager.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        */
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
    //int newState = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
    //if (newState == GLFW_PRESS && oldState == GLFW_RELEASE) {
    //    camera.launchRaycastCollision();
    //}
    //oldState = newState;  
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