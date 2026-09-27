#include <iostream>
#include <sstream>
#include <cmath>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include "../Header_Files/Texture2D.h"
#include "../Header_Files/ShaderProgram.h"
#include "../common/includes/glm/glm.hpp"




const char* APP_TITLE = "Introduction to Modern OpenGL - Hello Shader";
const int gWindowWidth = 800;
const int gWindowHeight = 600;


void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode);
void showFPS(GLFWwindow * window);
bool initOpenGL();
GLFWwindow* gWindow = NULL;
bool gWireframe = false;
const std::string texture1Filename = "textures/IOGL-datas/textures/airplane.PNG";
const std::string texture2Filename = "textures/IOGL-datas/textures/crate.jpg";

int main(){

    if(!initOpenGL()){
            std::cerr << "GLFW initialization failed" << std::endl;
            return -1;
    }

    GLfloat vertices[] = {   
        -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 
         0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.0f, 0.0f, 0.0f
    };

    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };


    GLuint vbo, ibo, vao;

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    ShaderProgram shaderProgram;
    shaderProgram.loadShaders("Source_Files/shaders/basic.vert", "Source_Files/shaders/basic.frag");

    Texture2D texture1;
    texture1.loadTexture(texture1Filename, true);

    Texture2D texture2;
    texture2.loadTexture(texture2Filename, true);

    glClearColor(0.23f, 0.30f, 0.47f, 1.0f);


    while(!glfwWindowShouldClose(gWindow)){
        showFPS(gWindow);
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        texture1.bind(0);
        texture2.bind(1);
        
        shaderProgram.use();
        glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture1"), 0);
        glUniform1i(glGetUniformLocation(shaderProgram.getProgram(), "myTexture2"), 1);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);

        glfwSwapBuffers(gWindow);
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);

    glfwTerminate();


    return 0;
}

bool initOpenGL(){
    if(!glfwInit()){
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);



    gWindow = glfwCreateWindow(gWindowWidth, gWindowHeight, APP_TITLE, NULL, NULL);

    if(gWindow == NULL){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(gWindow);

    glfwSetKeyCallback(gWindow, glfw_onKey);

    glewExperimental = GL_TRUE;

    if(glewInit() != GLEW_OK){
        std::cerr << "GLFW initialization failed" << std::endl;
        return false;
    }

    return true;
}

void glfw_onKey(GLFWwindow* window, int key, int scancode, int action, int mode){
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if(key == GLFW_KEY_W && action == GLFW_PRESS){
        gWireframe = !gWireframe;
        if(gWireframe){
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

void showFPS(GLFWwindow * window){
    static double previousSeconds = 0.0;
    static int frameCount = 0;
    double elapsedSeconds;
    double currentSeconds = glfwGetTime(); //returns the number os seconds since GLFW started, as a double

    elapsedSeconds = currentSeconds - previousSeconds;

    // limit text update 4 times per second
    if(elapsedSeconds > 0.25){
        previousSeconds = currentSeconds;
        double fps = (double) frameCount / elapsedSeconds;
        double msPerFrame = 1000.0 / fps;

        std::ostringstream outs;
        outs.precision(3);
        outs << std::fixed << APP_TITLE << "  "
        << "FPS: " << fps << "  "
        << "Frame time: " << msPerFrame << " (ms)";

        glfwSetWindowTitle(window, outs.str().c_str());

        frameCount = 0;
    }

    frameCount++;
}