#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glm/fwd.hpp"
#include "shaders/shader.h"
#include "camera/Camera.h"
#include "tinyobjloader/tiny_obj_loader.h"

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
unsigned int loadTexture(char const *path);
std::vector<float> loadObjModel(const std::string &path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(4.7f, 2.6f, 4.7f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(-1.0f, 1.2f, 0.8f);


int main() {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Luna", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader zprogram
  // ------------------------------------
  Shader lightingShader(OBJECT);
  Shader lightCubeShader(LIGHTSOURCE);



  // ------------------------------------------ water ------------------------------------------ //

  std::vector<float> water = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/water.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int waterVBO, waterVAO;
  glGenVertexArrays(1, &waterVAO);
  glGenBuffers(1, &waterVBO);

  glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
  glBufferData(GL_ARRAY_BUFFER, water.size() * sizeof(float), &water[0], GL_STATIC_DRAW);

  glBindVertexArray(waterVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);


  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMapwater = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/water.jpeg");






  // ------------------------------------------ dirt ------------------------------------------ //

  std::vector<float> dirt = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/dirt.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int dirtVBO, dirtVAO;
  glGenVertexArrays(1, &dirtVAO);
  glGenBuffers(1, &dirtVBO);

  glBindBuffer(GL_ARRAY_BUFFER, dirtVBO);
  glBufferData(GL_ARRAY_BUFFER, dirt.size() * sizeof(float), &dirt[0], GL_STATIC_DRAW);

  glBindVertexArray(dirtVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMapdirt = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/dirt.jpg");





  // ------------------------------------------ grass ------------------------------------------ //

  std::vector<float> grass = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/grass.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int grassVBO, grassVAO;
  glGenVertexArrays(1, &grassVAO);
  glGenBuffers(1, &grassVBO);

  glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
  glBufferData(GL_ARRAY_BUFFER, grass.size() * sizeof(float), &grass[0], GL_STATIC_DRAW);

  glBindVertexArray(grassVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMapgrass = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/grass.jpg");





  // ------------------------------------------ stone ------------------------------------------ //

  std::vector<float> stone = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/stone.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int stoneVBO, stoneVAO;
  glGenVertexArrays(1, &stoneVAO);
  glGenBuffers(1, &stoneVBO);

  glBindBuffer(GL_ARRAY_BUFFER, stoneVBO);
  glBufferData(GL_ARRAY_BUFFER, stone.size() * sizeof(float), &stone[0], GL_STATIC_DRAW);

  glBindVertexArray(stoneVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMapstone = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/stone.jpg");





  // ------------------------------------------ wood ------------------------------------------ //

  std::vector<float> wood = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/wood.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int woodVBO, woodVAO;
  glGenVertexArrays(1, &woodVAO);
  glGenBuffers(1, &woodVBO);

  glBindBuffer(GL_ARRAY_BUFFER, woodVBO);
  glBufferData(GL_ARRAY_BUFFER, wood.size() * sizeof(float), &wood[0], GL_STATIC_DRAW);

  glBindVertexArray(woodVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMapwood = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/wood.jpeg");





  // ------------------------------------------ bridge ------------------------------------------ //

  std::vector<float> bridge = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/bridge.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int bridgeVBO, bridgeVAO;
  glGenVertexArrays(1, &bridgeVAO);
  glGenBuffers(1, &bridgeVBO);

  glBindBuffer(GL_ARRAY_BUFFER, bridgeVBO);
  glBufferData(GL_ARRAY_BUFFER, bridge.size() * sizeof(float), &bridge[0], GL_STATIC_DRAW);

  glBindVertexArray(bridgeVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);



  // ------------------------------------------ leaves ------------------------------------------ //

  std::vector<float> leaves = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/leaves.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int leavesVBO, leavesVAO;
  glGenVertexArrays(1, &leavesVAO);
  glGenBuffers(1, &leavesVBO);

  glBindBuffer(GL_ARRAY_BUFFER, leavesVBO);
  glBufferData(GL_ARRAY_BUFFER, leaves.size() * sizeof(float), &leaves[0], GL_STATIC_DRAW);

  glBindVertexArray(leavesVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMapleaves = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/leaves.jpg");



  // ------------------------------------------ sun ------------------------------------------ //
  std::vector<float> sun = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/sun.obj");

  unsigned int sunVAO, sunVBO;
  glGenVertexArrays(1, &sunVAO);
  glGenBuffers(1, &sunVBO);

  glBindVertexArray(sunVAO);

  glBindBuffer(GL_ARRAY_BUFFER, sunVBO);
  glBufferData(GL_ARRAY_BUFFER, sun.size() * sizeof(float), &sun[0], GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);





  glEnable(GL_CULL_FACE);


  // shader configuration
  // --------------------
  lightingShader.use(); 
  lightingShader.setInt("material.diffuse", 0);


  // render loop
  // -----------
  while (!glfwWindowShouldClose(window)) {
    // per-frame logic
    // ---------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.902f, 0.945f, 0.847f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.use();
    lightingShader.setVec3("light.position", lightPos);
    lightingShader.setVec3("viewPos", camera.Position);

    // light properties
    lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
    lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);




  // ------------------------------------------ water ------------------------------------------ //
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapwater);

    // material properties
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.setFloat("material.shininess", 128.0f);

    size_t waterVertexCount = water.size()/8;

    // render water
    glBindVertexArray(waterVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(waterVertexCount));




  // ------------------------------------------ dirt ------------------------------------------ //
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapdirt);

    // material properties
    lightingShader.setVec3("material.specular", 0.3f, 0.3f, 0.3f);
    lightingShader.setFloat("material.shininess", 16.0f);

    size_t dirtVertexCount = dirt.size()/8;

    // render dirt
    glBindVertexArray(dirtVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(dirtVertexCount));




  // ------------------------------------------ grass ------------------------------------------ //
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapgrass);

    // material properties
    lightingShader.setVec3("material.specular", 0.4f, 0.4f, 0.4f);
    lightingShader.setFloat("material.shininess", 32.0f);

    size_t grassVertexCount = grass.size()/8;

    // render grass
    glBindVertexArray(grassVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(grassVertexCount));



  // ------------------------------------------ stone ------------------------------------------ //
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapstone);

    // material properties
    lightingShader.setVec3("material.specular", 0.1f, 0.1f, 0.1f);
    lightingShader.setFloat("material.shininess", 8.0f);

    size_t stoneVertexCount = stone.size()/8;

    // render stone
    glBindVertexArray(stoneVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(stoneVertexCount));



  // ------------------------------------------ wood ------------------------------------------ //
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapwood);

    // material properties
    lightingShader.setVec3("material.specular", 0.3f, 0.2f, 0.2f);
    lightingShader.setFloat("material.shininess", 24.0f);

    size_t woodVertexCount = wood.size()/8;

    // render wood
    glBindVertexArray(woodVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(woodVertexCount));



  // ------------------------------------------ bridge ------------------------------------------ //
    size_t bridgeVertexCount = bridge.size()/8;

    // render bridge
    glBindVertexArray(bridgeVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(bridgeVertexCount));



  // ------------------------------------------ leaves ------------------------------------------ //
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapleaves);

    // material properties
    lightingShader.setVec3("material.specular", 0.2f, 0.3f, 0.2f);
    lightingShader.setFloat("material.shininess", 16.0f);

    size_t leavesVertexCount = leaves.size()/8;

    // render leaves
    glBindVertexArray(leavesVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(leavesVertexCount));





  // ------------------------------------------ SUN ------------------------------------------ //
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);

    glm::vec3 orbit = glm::vec3(-2.0f, 0.0f, 0.0f);

    float orbitRadius = 4.0f;
    float orbitSpeed = glfwGetTime() * 1.4f;

    float lightZ = orbitRadius * cos(orbitSpeed);
    float lightY = orbitRadius * sin(orbitSpeed);
    lightPos = glm::vec3(0.0f, lightY, lightZ);

    model = glm::translate(model, lightPos);

    lightCubeShader.setMat4("model", model);

    size_t sunVertexCount = sun.size()/8;
    glBindVertexArray(sunVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(sunVertexCount));


    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = static_cast<float>(2.5 * deltaTime);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    camera.ProcessKeyboard(UP, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    camera.ProcessKeyboard(DOWN, deltaTime);
  
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}


// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  // load image, create texture and generate mipmaps
  int width, height, nrComponents;
  stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
                                          //
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);

  if (data) {
    GLenum format;
    if(nrComponents == 1)
      format = GL_RED;
    else if(nrComponents == 3)
      format = GL_RGB;
    else if(nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else {
    std::cout << "Failed to load texture: " << stbi_failure_reason() << std::endl;
  }

  stbi_image_free(data);
  return textureID;
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

std::vector<float> loadObjModel(const std::string& path) {
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

      // Normals (if present)
      if (index.normal_index >= 0) {
        tinyobj::real_t nx = attrib.normals[3 * index.normal_index + 0];
        tinyobj::real_t ny = attrib.normals[3 * index.normal_index + 1];
        tinyobj::real_t nz = attrib.normals[3 * index.normal_index + 2];
        vertices.push_back(nx);
        vertices.push_back(ny);
        vertices.push_back(nz);
      } else {
        // Fallback if normals are missing
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }

      // Texture coordinates (if present)
      if (index.texcoord_index >= 0) {
        tinyobj::real_t tx = attrib.texcoords[2 * index.texcoord_index + 0];
        tinyobj::real_t ty = attrib.texcoords[2 * index.texcoord_index + 1];
        vertices.push_back(tx);
        vertices.push_back(ty);
      } else {
        // Fallback if texture coordinates are missing
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);
      }
    }
  }

  return vertices;
}
