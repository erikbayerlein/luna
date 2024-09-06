#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>


class Shader {
public:
    unsigned int ID;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader() {
      // 1.0 declare shaders
      vShaderCode = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
        "layout (location = 1) in vec2 aTexCoord;\n" // the color variable has attribute position 1
        "out vec2 TexCoord;\n" // the color variable has attribute position 1
        "uniform mat4 model;\n" // the color variable has attribute position 1
        "uniform mat4 view;\n" // the color variable has attribute position 1
        "uniform mat4 projection;\n" // the color variable has attribute position 1
        "void main()\n"
        "{\n"
        "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
        "   TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n" // set ourColor to the input color we got from the vertex data
        "}\0";

      fShaderCode = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoord;\n" // a way to pass data from our app on the CPU to the shaders on the GPU
        "uniform sampler2D texture1;\n" // a way to pass data from our app on the CPU to the shaders on the GPU
        "uniform sampler2D texture2;\n" // a way to pass data from our app on the CPU to the shaders on the GPU
        "void main()\n"
        "{\n"
        "   FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.1);\n"
        "}\n\0";

      // 2. compile shaders
      unsigned int vertex, fragment;
      // vertex shader
      vertex = glCreateShader(GL_VERTEX_SHADER);
      glShaderSource(vertex, 1, &vShaderCode, NULL);
      glCompileShader(vertex);
      checkCompileErrors(vertex, "VERTEX");
      // fragment Shader
      fragment = glCreateShader(GL_FRAGMENT_SHADER);
      glShaderSource(fragment, 1, &fShaderCode, NULL);
      glCompileShader(fragment);
      checkCompileErrors(fragment, "FRAGMENT");
      // shader Program
      ID = glCreateProgram();
      glAttachShader(ID, vertex);
      glAttachShader(ID, fragment);
      glLinkProgram(ID);
      checkCompileErrors(ID, "PROGRAM");
      // delete the shaders as they're linked into our program now and no longer necessary
      glDeleteShader(vertex);
      glDeleteShader(fragment);
    }
    // activate the shader
    // ------------------------------------------------------------------------
    void use() 
    { 
      glUseProgram(ID); 
    }
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const
    {         
      glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const
    { 
      glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const
    { 
      glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, glm::mat4 &value) const
    { 
      glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

private:
  const char* vShaderCode;
  const char* fShaderCode;
  // utility function for checking shader compilation/linking errors.
  // ------------------------------------------------------------------------
  void checkCompileErrors(unsigned int shader, std::string type)
  {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (!success)
      {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------- -- " << std::endl;
      }
    }
    else
    {
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (!success)
      {
        glGetProgramInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- ---------------------------------- -- " << std::endl;
      }
    }
  }
};
#endif
