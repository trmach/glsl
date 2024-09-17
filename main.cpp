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

float MOVEMENT_SPEED = 0.02;
float ROTATION_SPEED = 0.02;

struct Position {
  float x;
  float y;
  float z;
};

struct Rotation {
  float x;
  float y;
  float z;
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height); 
void closeOnEscapeKey(GLFWwindow *window);

void key_handle_W(GLFWwindow * window, Position * p, Rotation * r);
void key_handle_S(GLFWwindow * window, Position * p, Rotation * r);
void key_handle_A(GLFWwindow * window, Position * p, Rotation * r);
void key_handle_D(GLFWwindow * window, Position * p, Rotation * r);

void key_handle_SPACE(GLFWwindow * window, Position * p);
void key_handle_LEFT_SHIFT(GLFWwindow * window, Position * p);

void key_handle_LEFT(GLFWwindow * window, Rotation * r);
void key_handle_RIGHT(GLFWwindow * window, Rotation * r);

void key_handle_UP(GLFWwindow * window, Rotation * r);
void key_handle_DOWN(GLFWwindow * window, Rotation * r);



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

  // create and compile our shader, neatly.
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

  // we manage the camera position and rotation here:

  struct Position pos = {0, 1, -3};
  struct Rotation rot = {0, 0, 0};


  
  // Main window loop
  while(!glfwWindowShouldClose(window)) {
    closeOnEscapeKey(window);
    // ------ ** put all rendering commands here ** ------ //
  
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ourShader.use();
    ourShader.setUniform3f("iResolution", 1, WINDOW_WIDTH, WINDOW_HEIGHT);
    ourShader.setUniform1f("iTime", glfwGetTime());
    ourShader.setRefUniform3f("iRotation", &rot.x, &rot.y, &rot.z);
    ourShader.setRefUniform3f("iPosition", &pos.x, &pos.y, &pos.z);

    key_handle_W(window, &pos, &rot);
    key_handle_S(window, &pos, &rot);
    key_handle_A(window, &pos, &rot);
    key_handle_D(window, &pos, &rot);

    key_handle_SPACE(window, &pos);
    key_handle_LEFT_SHIFT(window, &pos);

    key_handle_LEFT(window, &rot);
    key_handle_RIGHT(window, &rot);

    key_handle_UP(window, &rot);
    key_handle_DOWN(window, &rot); // better than glfwSetKeyCallback frfr
  


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

void closeOnEscapeKey(GLFWwindow *window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}


void key_handle_W(GLFWwindow * window, Position * p, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    p->x += MOVEMENT_SPEED*sin(r->z);
    p->z += MOVEMENT_SPEED*cos(r->z);
  }
}

void key_handle_S(GLFWwindow * window, Position * p, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    p->x -= MOVEMENT_SPEED*sin(r->z);
    p->z -= MOVEMENT_SPEED*cos(r->z);

  }
}

void key_handle_A(GLFWwindow * window, Position * p, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    p->x -= MOVEMENT_SPEED*cos(r->z);
    p->z += MOVEMENT_SPEED*sin(r->z);

  }
}

void key_handle_D(GLFWwindow * window, Position * p, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    p->x += MOVEMENT_SPEED*cos(r->z);
    p->z -= MOVEMENT_SPEED*sin(r->z);

  }
}

void key_handle_SPACE(GLFWwindow * window, Position * p) {
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    p->y += MOVEMENT_SPEED;
  }
}

void key_handle_LEFT_SHIFT(GLFWwindow * window, Position * p) {
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
    p->y -= MOVEMENT_SPEED;
  }
}

void key_handle_LEFT(GLFWwindow * window, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    r->z -= ROTATION_SPEED;
  }
}

void key_handle_RIGHT(GLFWwindow * window, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    r->z += ROTATION_SPEED;
  }
}

void key_handle_UP(GLFWwindow * window, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    r->y -= ROTATION_SPEED;
  }
}

void key_handle_DOWN(GLFWwindow * window, Rotation * r) {
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    r->y += ROTATION_SPEED;
  }
}
