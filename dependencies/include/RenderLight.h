#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/shader.h>

class RenderLight {
  public:
    unsigned int VAO, VBO;
    std::vector<float> vertices;
    glm::vec3 lightPos;
    
    RenderLight(const std::string &modelPath);
    void renderLight(Shader &shader);
    void renderSun(glm::mat4 &projection, glm::mat4 &view, Shader &lightCubeShader);
    
  private:
    void setupMesh();
    std::vector<float> loadObjModel(const std::string& path);
};
