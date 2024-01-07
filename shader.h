#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
  public:

    unsigned int ID;

    Shader(const char * vertexPath, const char * fragmentPath) {

      std::string vertexSource;
      std::string fragmentSource;
      std::ifstream vertexShaderFile;
      std::ifstream fragmentShaderFile;

      vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      try {
        // open files
        vertexShaderFile.open(vertexPath);
        fragmentShaderFile.open(fragmentPath);
        // read file buffers into streams
        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        // close files
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // write streams to strings
        vertexSource = vertexShaderStream.str();
        fragmentSource = fragmentShaderStream.str();
      } catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_READ_FAILED: " << e.what() << std::endl;
      }

      const char * vShaderSource = vertexSource.c_str();
      const char * fShaderSource = fragmentSource.c_str();
      unsigned int vertex, fragment;

      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderSource, NULL);
      glCompileShader(vertex);
      checkCompileErrors(vertex, "VERTEX");

      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderSource, NULL);
      glCompileShader(fragment);
      checkCompileErrors(fragment, "FRAGMENT");

      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      glLinkProgram(ID);
      checkCompileErrors(ID, "PROGRAM");

      glDeleteShader(vertex);
      glDeleteShader(fragment);

    } 

    void use() {
      glUseProgram(ID);
    }

    void setBool(const std::string &name, bool value) const {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void setUniform1i(const std::string &name, int value) const {
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setUniform1f(const std::string &name, float value) const {
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void setUniform3f(const std::string &name, float v1, float v2, float v3) const {
      glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
    }

    void setRefUniform3f(const std::string &name, float * v1, float * v2, float * v3) const {
      glUniform3f(glGetUniformLocation(ID, name.c_str()), *v1, *v2, *v3);
    }


  private:

    void checkCompileErrors(unsigned int shader, std::string type) {
      int success;
      char infoLog[1024];

      if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success) {
          glGetShaderInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED: \n" << infoLog << std::endl;
        }

      } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);

        if (!success) {
          glGetProgramInfoLog(shader, 1024, NULL, infoLog);
          std::cout << "ERROR::SHADER::LINKING_FAILED: \n" << infoLog << std::endl;
        }

      }
    }
};


#endif
