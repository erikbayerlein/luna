#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <shaders/shader.h>

class RenderObject {
  public:
    unsigned int VAO, VBO;
    std::vector<float> vertices;
    unsigned int texture;
    glm::vec3 specular;
    float shininess;
    
    RenderObject(const std::string &modelPath, const std::string &texturePath, glm::vec3 specular, float shininess);
    void render(Shader &shader);
    void renderSun(const glm::mat4 &projection, const glm::mat4 &view, GLuint sunVAO, const std::vector<float> &sun, Shader &lightCubeShader);
    
  private:
    void setupMesh();
    unsigned int loadTexture(const char *path);
    std::vector<float> loadObjModel(const std::string& path);
};
