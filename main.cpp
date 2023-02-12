#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Impossibile creare la finestra";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Carico la robaccia di glad
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Impossibile inizializzare glad";
        return -1;
    }

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        //Process Input

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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