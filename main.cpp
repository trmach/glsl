#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <stdbool.h>
#include <iostream>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

#define VERTEX_SHADER_PATH "vertShader.glsl"
#define FRAGMENT_SHADER_PATH "fragShader.glsl"

void framebufferSizeCallback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow *window);

float triangleVertices[] = {
  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
  1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
 -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
 -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f
};

unsigned int triangleIndices[] = {
  0, 1, 3,
  1, 2, 3
};

const char *vertexShaderSource =
"#version 460 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main() {\n"
"  gl_Position = vec4(aPos, 1.0);\n"
"  ourColor = aColor;\n"
"}\0";

const char *fragmentShaderSource =
"#version 460 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main() {\n"
"  FragColor = vec4(ourColor, 1.0);\n"
"}\0";

int main() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello world!", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

  // change viewport if window size should change
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  Shader ourShader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

  unsigned int VBO, VAO, EBO; // vertex buf object, vertex array object, element array buf
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &EBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

  // Vertex shader position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Create and compile vertex shader
  
  // Main window loop
  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    // ------ ** put all rendering commands here ** ------ //
  
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();
    ourShader.setUniform3f("iResolution", 1, WINDOW_WIDTH, WINDOW_HEIGHT);
    ourShader.setUniform1f("iTime", glfwGetTime());

 //   glBindVertexArray(VAO);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    // --------------------------------------------------- //
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
