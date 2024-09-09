#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <iostream>

enum Type {
  COLOR,
  LIGHT
};


class Shader {
public:
    unsigned int ID;

    // constructor generates the shader on the fly
    // ------------------------------------------------------------------------
    Shader(Type shaderType) {
      if(shaderType == COLOR) {
        // 1.0 declare shaders
        vShaderCode = "#version 330 core\n"
          "layout (location = 0) in vec3 aPos;\n"
          "layout (location = 1) in vec3 aNormal;\n"

          "out vec3 Normal;\n"
          "out vec3 FragPos;\n"

          "uniform mat4 model;\n"
          "uniform mat4 view;\n"
          "uniform mat4 projection;\n"

          "void main()\n"
          "{\n"
          "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
          "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
          "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
          "}\0";

        fShaderCode = "#version 330 core\n"
          "struct Material {\n"
          "   vec3 ambient;\n"
          "   vec3 diffuse;\n"
          "   vec3 specular;\n"
          "   float shininess;\n"
          "};\n"

          "struct Light {\n"
          "   vec3 ambient;\n"
          "   vec3 diffuse;\n"
          "   vec3 specular;\n"
          "   vec3 position;\n"
          "};\n"

          "out vec4 FragColor;\n"

          "in vec3 Normal;\n"
          "in vec3 FragPos;\n"

          "uniform Light light;\n"
          "uniform Material material;\n"
          "uniform vec3 objectColor;\n"
          "uniform vec3 lightColor;\n"
          "uniform vec3 lightPos;\n"
          "uniform vec3 viewPos;\n"

          "void main()\n"
          "{\n"
          // ambient
          "   vec3 ambient = light.ambient * material.ambient;\n"
          // diffuse
          "   vec3 norm = normalize(Normal);\n"
          "   vec3 lightDir = normalize(lightPos - FragPos);\n"
          "   float diff = max(dot(norm, lightDir), 0.0);\n"
          "   vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
          // specular
          "   vec3 viewDir = normalize(viewPos - FragPos);\n"
          "   vec3 reflectDir = reflect(-lightDir, norm);\n"
          "   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);\n"
          "   vec3 specular = light.specular * (spec * material.specular);\n"

          "   vec3 result = ambient + diffuse + specular;\n"
          "   FragColor = vec4(result, 1.0);\n"
          "}\0";
      } else {
        // 1.0 declare shaders
        vShaderCode = "#version 330 core\n"
          "layout (location = 0) in vec3 aPos;\n"
          "uniform mat4 model;\n"
          "uniform mat4 view;\n"
          "uniform mat4 projection;\n"
          "void main()\n"
          "{\n"
          "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
          "}\0";

        fShaderCode = "#version 330 core\n"
          "out vec4 FragColor;\n"
          "void main()\n"
          "{\n"
          "   FragColor = vec4(1.0);\n"
          "}\0";
      }

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
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, float x, float y, float z) const
    { 
      glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z); 
    }
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, glm::vec3 &value) const
    { 
      glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 
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
