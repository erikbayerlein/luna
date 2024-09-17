#include "stb_image.h"
#include <RenderObject.h>
#include <iostream>
#include <tinyobjloader/tiny_obj_loader.h>

RenderObject::RenderObject(const std::string &modelPath, const std::string &texturePath, glm::vec3 specular, float shininess)
    : specular(specular), shininess(shininess) {
    vertices = loadObjModel(modelPath);
    texture = loadTexture(texturePath.c_str());
    setupMesh();
}

void RenderObject::setupMesh() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture coordinate attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void RenderObject::render(Shader &shader) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    shader.setVec3("material.specular", specular);
    shader.setFloat("material.shininess", shininess);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 8));
}

unsigned int RenderObject::loadTexture(const char *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(data);

    return textureID;
}

std::vector<float> RenderObject::loadObjModel(const std::string& path) {
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
