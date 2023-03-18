#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <engine/shader_m.h>
#include <engine/camera.h>
#include <iostream>

using namespace std;

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

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


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

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("camera.vs", "camera.fs");
    Shader lightCubeShader("camera.vs", "camera.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };

    glm::vec3 cubePositions[] = {
      glm::vec3(0.0f,  0.0f,  0.0f),//CUBO NEGRO FONDO
   //CARA FRENTE
     glm::vec3(-0.32f, 0.32f, 0.37f),//PRIMERA FILA
     glm::vec3(0.0f, 0.32f, 0.37f),
     glm::vec3(0.32f, 0.32f, 0.37f),//PRIMERA FILA
     glm::vec3(-0.32f, 0.0f, 0.37f),//SEGUNDA FILA
     glm::vec3(0.0f, 0.0f, 0.37f),
     glm::vec3(0.32f, 0.0f, 0.37f),//SEGUNDA FILA
     glm::vec3(-0.32f, -0.32f, 0.37f),//TERCERA FILA
     glm::vec3(0.0f, -0.32f, 0.37f),
     glm::vec3(0.32f, -0.32f, 0.37f),//TERCERA FILA
      
     //CARA TRACERA
     glm::vec3(-0.32f, 0.32f, -0.37f),//PRIMERA FILA
     glm::vec3(0.0f, 0.32f, -0.37f),
     glm::vec3(0.32f, 0.32f, -0.37f),//PRIMERA FILA
     glm::vec3(-0.32f, 0.0f, -0.37f),//SEGUNDA FILA
     glm::vec3(0.0f, 0.0f, -0.37f),
     glm::vec3(0.32f, 0.0f, -0.37f),//SEGUNDA FILA
     glm::vec3(-0.32f, -0.32f, -0.37f),//TERCERA FILA
     glm::vec3(0.0f, -0.32f, -0.37f),
     glm::vec3(0.32f, -0.32f, -0.37f),//TERCERA FILA

     //CARA IZQUIERDA (MI IZQUIERDA)
     glm::vec3(-0.37f, 0.32f, -0.32f),//PRIMERA FILA
     glm::vec3(-0.37f, 0.32f, 0.0f),
     glm::vec3(-0.37f, 0.32f, 0.32f),//PRIMERA FILA
     glm::vec3(-0.37f, 0.0f, -0.32f),//SEGUNDA FILA
     glm::vec3(-0.37f, 0.0f, 0.0f),
     glm::vec3(-0.37f, 0.0f, 0.32f),//SEGUNDA FILA
     glm::vec3(-0.37f, -0.32f, -0.32f),//TERCERA FILA
     glm::vec3(-0.37f, -0.32f, 0.0f),
     glm::vec3(-0.37f, -0.32f, 0.32f),//TERCERA FILA

      //CARA DERECHA (MI DERECHA)
     glm::vec3(0.37f, 0.32f, -0.32f),//PRIMERA FILA
     glm::vec3(0.37f, 0.32f, 0.0f),
     glm::vec3(0.37f, 0.32f, 0.32f),//PRIMERA FILA
     glm::vec3(0.37f, 0.0f, -0.32f),//SEGUNDA FILA
     glm::vec3(0.37f, 0.0f, 0.0f),
     glm::vec3(0.37f, 0.0f, 0.32f),//SEGUNDA FILA
     glm::vec3(0.37f, -0.32f, -0.32f),//TERCERA FILA
     glm::vec3(0.37f, -0.32f, 0.0f),
     glm::vec3(0.37f, -0.32f, 0.32f),//TERCERA FILA

     //CARA ARRIBA 
     glm::vec3(-0.32f, 0.37f,  -0.32f),//PRIMERA FILA
     glm::vec3(0.0f, 0.37f,  -0.32f),
     glm::vec3(0.32f, 0.37f,  -0.32f),//PRIMERA FILA
     glm::vec3(-0.32f, 0.37f,  0.0f),//SEGUNDA FILA
     glm::vec3(0.0f, 0.37f,  0.0f),
     glm::vec3(0.32f, 0.37f,  0.0f),//SEGUNDA FILA
     glm::vec3(0.0f, 0.37f,  0.32f),//TERCERA FILA
     glm::vec3(-0.32f, 0.37f,  0.32f),
     glm::vec3(0.32f, 0.37f,  0.32f),//TERCERA FILA


     //CARA ABAJO 
     glm::vec3(-0.32f, -0.37f,  -0.32f),//PRIMERA FILA
     glm::vec3(0.0f, -0.37f,  -0.32f),
     glm::vec3(0.32f, -0.37f,  -0.32f),//PRIMERA FILA
     glm::vec3(-0.32f, -0.37f,  0.0f),//SEGUNDA FILA
     glm::vec3(0.0f, -0.37f,  0.0f),
     glm::vec3(0.32f, -0.37f,  0.0f),//SEGUNDA FILA
     glm::vec3(0.0f, -0.37f,  0.32f),//TERCERA FILA
     glm::vec3(-0.32f, -0.37f,  0.32f),
     glm::vec3(0.32f, -0.37f,  0.32f)//TERCERA FILA
                                    /* glm::vec3(0.0f, 0.0f, 0.0f),
       glm::vec3(-0.3f, 0.3f, 0.4f),
       glm::vec3(0.0f, 0.3f, 0.4f),
       glm::vec3(0.3f, 0.3f, 0.4f),
       glm::vec3(0.3f, 0.0f, 0.4f),
       glm::vec3(0.0f, 0.0f, 0.4f),
       glm::vec3(-0.3f, 0.0f, 0.4f),
       glm::vec3(-0.3f, -0.3f, 0.4f),
       glm::vec3(0.0f, -0.3f, 0.4f),
       glm::vec3(0.3f, -0.3f, 0.4f) ,*/
       //glm::vec3(-0.4f, 0.3f, 0.0f),//
       //glm::vec3(-0.4f, 0.3f, 0.3f),
       //glm::vec3(-0.4f, 0.3f, -0.3f),
       // glm::vec3(-0.4f, 0.0f, -0.3f),
       // glm::vec3(-0.4f, 0.0f, 0.0f),
       // glm::vec3(-0.4f, 0.0f, 0.3f),
       // glm::vec3(-0.4f, -0.3f, 0.3f),
       // glm::vec3(-0.4f, -0.3f, 0.0f),
       // glm::vec3(-0.4f, -0.3f, -0.3f),
       // glm::vec3(-0.4f, -0.3f, -0.3f),
       // glm::vec3(-0.3f, 0.3f, -0.4f),//
       //glm::vec3(0.0f, 0.3f, -0.4f),
       //glm::vec3(0.3f, 0.3f, -0.4f),
       //glm::vec3(0.3f, 0.0f, -0.4f),
       //glm::vec3(0.0f, 0.0f, -0.4f),
       //glm::vec3(-0.3f, 0.0f, -0.4f),
       //glm::vec3(-0.3f, -0.3f, -0.4f),
       //glm::vec3(0.0f, -0.3f,-0.4f),
       //glm::vec3(0.3f, -0.3f, -0.4f) ,

       //glm::vec3(0.4f, 0.3f, 0.0f),//
       //glm::vec3(0.4f, 0.3f, 0.3f),
       //glm::vec3(0.4f, 0.3f, -0.3f),
       // glm::vec3(0.4f, 0.0f, -0.3f),
       // glm::vec3(0.4f, 0.0f, 0.0f),
       // glm::vec3(0.4f, 0.0f, 0.3f),
       // glm::vec3(0.4f, -0.3f, 0.3f),
       // glm::vec3(0.4f, -0.3f, 0.0f),
       // glm::vec3(0.4f, -0.3f, -0.3f),//

       // glm::vec3(0.0f, 0.4f, 0.0f),
       // glm::vec3(0.3f, 0.4f, 0.0f),
       // glm::vec3(-0.3f, 0.4f, 0.0f),
       //  glm::vec3(0.0f, 0.4f, 0.0f),
       // glm::vec3(0.0f, 0.4f, 0.3f),
       // glm::vec3(0.0f, 0.4f, -0.3f),


    };


    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // we only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // render the cube
        glBindVertexArray(cubeVAO);

        for (unsigned int i = 0; i < 55; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            //if (i % 3 == 0) { // every 3rd iteration (including the first) we set the angle using GLFW's time function.
            // //angle = glfwGetTime() * 25.0f;
            //    model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
            //}
            


            if(i> 0) {
                //1.0f, 1.0f, 1.0f __BLANCO
                //0.0f, 1.0f, 0.0f _VERDE
                //1.0f, 0.5f, 0.0f
                //1.0f, 0.0f, 0.0f  _ROJO
                //1.0f, 1.0f, 0.0f _AMARILLO
                //0.0f, 0.0f, 1.0 _AZUL
             
                model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
                lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);

               
            }
            if (/*i >=1 && i <= 9*/i == 9 || i == 18 || i == 27 || i == 36 || i == 45 || i == 54 || i == 5 || i == 14 || i == 23) {//ROJO
                lightingShader.setVec3("objectColor", 1.0f, 0.0f, 0.0f);
              
            }
            if (/*i >= 10 && i <= 18*/i == 6|| i == 15 || i == 24|| i == 33 || i == 42 || i == 51 || i == 4 || i == 13 || i == 22) {//AZUL
                lightingShader.setVec3("objectColor", 0.0f, 0.0f, 1.0f);

            }
            if (/*i >= 19 && i <= 27*/i == 7 || i == 16 || i == 25 || i == 34 || i == 43 || i == 52 || i == 32 || i == 41 || i == 50) {//VERDE
                lightingShader.setVec3("objectColor", 0.0f, 1.0f, 0.0f);

            }
            if (/*i >= 28 && i <= 36*/i == 31 || i == 40 || i == 49 || i == 2 || i == 11 || i == 20|| i == 29 || i == 38 || i == 47) {//AMARILLO
                lightingShader.setVec3("objectColor", 1.0f, 1.0f, 0.0f);

            }
            if (/*i >= 37 && i <= 45*/i==1||i==10 || i == 19 || i == 28 || i == 37 || i == 46 || i == 35 || i == 44 || i == 35) {//NARANJA
                lightingShader.setVec3("objectColor", 1.0f, 0.5f, 0.0f);

            }
            //if (i >= 36 && i <= 55) {//BLANCO
            //    lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);

            //}
            ////model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            //
            lightingShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glDrawArrays(GL_TRIANGLES, 0, 36);


         //also draw the lamp object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lightCubeShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
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