#include <RenderLight.h>
#include "GLFW/glfw3.h"
#include <tinyobjloader/tiny_obj_loader.h>

RenderLight::RenderLight(const std::string &modelPath) {
    vertices = loadObjModel(modelPath);
    setupMesh();
}

void RenderLight::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void RenderLight::renderLight(Shader &shader) {
    shader.use();

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 8));
    glBindVertexArray(0);
}

void RenderLight::renderSun(glm::mat4 &projection, glm::mat4 &view, Shader &lightShader) {
    lightShader.use();

    lightShader.setMat4("projection", projection);
    lightShader.setMat4("view", view);

    glm::mat4 model = glm::mat4(1.0f);

    float orbitRadius = 4.0f;
    float orbitSpeed = glfwGetTime() * 1.4f;

    float lightZ = orbitRadius * cos(orbitSpeed);
    float lightY = orbitRadius * sin(orbitSpeed);
    lightPos = glm::vec3(0.0f, lightY, lightZ);

    model = glm::translate(model, lightPos);

    lightShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 8));
    glBindVertexArray(0);
}

std::vector<float> RenderLight::loadObjModel(const std::string& path) {
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warn, err;

  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

  if (!warn.empty()) {
    std::cout << "WARN: " << warn << std::endl;
  }

  if (!err.empty()) {
    std::cerr << "ERR: " << err << std::endl;
  }

  if (!ret) {
    std::cerr << "Failed to load/parse .obj file!" << std::endl;
    return {};
  }

  std::vector<float> vertices;
  for (const auto& shape : shapes) {
    for (const auto& index : shape.mesh.indices) {
      // Vertex positions
      tinyobj::real_t vx = attrib.vertices[3 * index.vertex_index + 0];
      tinyobj::real_t vy = attrib.vertices[3 * index.vertex_index + 1];
      tinyobj::real_t vz = attrib.vertices[3 * index.vertex_index + 2];
      vertices.push_back(vx);
      vertices.push_back(vy);
      vertices.push_back(vz);

      // Normals
      if (index.normal_index >= 0) {
        tinyobj::real_t nx = attrib.normals[3 * index.normal_index + 0];
        tinyobj::real_t ny = attrib.normals[3 * index.normal_index + 1];
        tinyobj::real_t nz = attrib.normals[3 * index.normal_index + 2];
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }

      // Texture coordinates
      if (index.texcoord_index >= 0) {
        tinyobj::real_t tx = attrib.texcoords[2 * index.texcoord_index + 0];
        tinyobj::real_t ty = attrib.texcoords[2 * index.texcoord_index + 1];
        vertices.push_back(tx);
        vertices.push_back(ty);
      } else {
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }
    }
  }

  return vertices;
}
