#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


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









  // ------------------------------------------ WALL ------------------------------------------ //
  
  std::vector<float> wall = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/wall.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int WallVBO, WallVAO;
  glGenVertexArrays(1, &WallVAO);
  glGenBuffers(1, &WallVBO);

  glBindBuffer(GL_ARRAY_BUFFER, WallVBO);
  glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &wall[0], GL_STATIC_DRAW);

  glBindVertexArray(WallVAO);
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // second, configure the light's WallVAO (WallVBO stays the same; the vertices are the same for the light object which is also a 3D cube)
  unsigned int lightCubeWallVAO;
  glGenVertexArrays(1, &lightCubeWallVAO);
  glBindVertexArray(lightCubeWallVAO);

  // we only need to bind to the WallVBO (to link it with glVertexAttribPointer), no need to fill it;
  // the WallVBO's data already contains all we need (it's already bound, but we do it again for educational purposes)
  glBindBuffer(GL_ARRAY_BUFFER, WallVBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // load textures (we now use a utility function to keep the code more organized)
  // -----------------------------------------------------------------------------
  unsigned int diffuseMapWall = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/wall.jpg");

  // ------------------------------------------------------------------------------------------ //








  // ------------------------------------------ SHELVES ------------------------------------------ //

  std::vector<float> shelves = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/shelves.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int shelvesVBO, shelvesVAO;
  glGenVertexArrays(1, &shelvesVAO);
  glGenBuffers(1, &shelvesVBO);

  glBindBuffer(GL_ARRAY_BUFFER, shelvesVBO);
  glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &shelves[0], GL_STATIC_DRAW);

  glBindVertexArray(shelvesVAO);
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
  unsigned int diffuseMapShelves = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/shelves.jpg");

  // ------------------------------------------------------------------------------------------ //










  // ------------------------------------------ FLOOR bugando ------------------------------------------ //
  //
  // std::vector<float> floor = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/floor.obj");
  //
  // // first, configure the cube's VAO (and VBO)
  // unsigned int floorVBO, floorVAO;
  // glGenVertexArrays(1, &floorVAO);
  // glGenBuffers(1, &floorVBO);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
  // glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &floor[0], GL_STATIC_DRAW);
  //
  // glBindVertexArray(floorVAO);
  // // Position attribute
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);
  //
  // // Normal attribute
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);
  //
  // // Texture coordinate attribute
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);
  //
  // // load textures (we now use a utility function to keep the code more organized)
  // // -----------------------------------------------------------------------------
  // unsigned int diffuseMapfloor = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/floor.jpeg");

  // ------------------------------------------------------------------------------------------ //







  // ------------------------------------------ BED ------------------------------------------ //

  std::vector<float> bed = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/bed.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int bedVBO, bedVAO;
  glGenVertexArrays(1, &bedVAO);
  glGenBuffers(1, &bedVBO);

  glBindBuffer(GL_ARRAY_BUFFER, bedVBO);
  glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &bed[0], GL_STATIC_DRAW);

  glBindVertexArray(bedVAO);
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
  unsigned int diffuseMapbed = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/shelves.jpg");

  // ------------------------------------------------------------------------------------------ //







  // ------------------------------------------ MATTRESS ------------------------------------------ //

  std::vector<float> mattress = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/mattress.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int mattressVBO, mattressVAO;
  glGenVertexArrays(1, &mattressVAO);
  glGenBuffers(1, &mattressVBO);

  glBindBuffer(GL_ARRAY_BUFFER, mattressVBO);
  glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &mattress[0], GL_STATIC_DRAW);

  glBindVertexArray(mattressVAO);
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
  unsigned int diffuseMapmattress = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/mattress.jpg");







  // ------------------------------------------ pillow BUGANDO ------------------------------------------ //

  // std::vector<float> pillow = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/pillow.obj");
  //
  // // first, configure the cube's VAO (and VBO)
  // unsigned int pillowVBO, pillowVAO;
  // glGenVertexArrays(1, &pillowVAO);
  // glGenBuffers(1, &pillowVBO);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, pillowVBO);
  // glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &pillow[0], GL_STATIC_DRAW);
  //
  // glBindVertexArray(pillowVAO);
  // // Position attribute
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);
  //
  // // Normal attribute
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);
  //
  // // Texture coordinate attribute
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);
  //
  // // load textures (we now use a utility function to keep the code more organized)
  // // -----------------------------------------------------------------------------
  // unsigned int diffuseMappillow = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/pillow.jpg");









  // ------------------------------------------ box ------------------------------------------ //

  std::vector<float> box = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/box.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int boxVBO, boxVAO;
  glGenVertexArrays(1, &boxVAO);
  glGenBuffers(1, &boxVBO);

  glBindBuffer(GL_ARRAY_BUFFER, boxVBO);
  glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &box[0], GL_STATIC_DRAW);

  glBindVertexArray(boxVAO);
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
  unsigned int diffuseMapbox = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/box.jpeg");







  // ------------------------------------------ baseLampshade ------------------------------------------ //

  std::vector<float> baseLampshade = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/base_lampshade.obj");

  // first, configure the cube's VAO (and VBO)
  unsigned int baseLampshadeVBO, baseLampshadeVAO;
  glGenVertexArrays(1, &baseLampshadeVAO);
  glGenBuffers(1, &baseLampshadeVBO);

  glBindBuffer(GL_ARRAY_BUFFER, baseLampshadeVBO);
  glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &baseLampshade[0], GL_STATIC_DRAW);

  glBindVertexArray(baseLampshadeVAO);
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
  unsigned int diffuseMapbaseLampshade = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/darkWood.jpeg");




  // ------------------------------------------ lampshade BUGANDO ------------------------------------------ //

  // std::vector<float> lampshade = loadObjModel("/Users/erikbayerlein/Documents/cg3d/src/resources/models/lampshade.obj");
  //
  // // first, configure the cube's VAO (and VBO)
  // unsigned int lampshadeVBO, lampshadeVAO;
  // glGenVertexArrays(1, &lampshadeVAO);
  // glGenBuffers(1, &lampshadeVBO);
  //
  // glBindBuffer(GL_ARRAY_BUFFER, lampshadeVBO);
  // glBufferData(GL_ARRAY_BUFFER, wall.size() * sizeof(float), &lampshade[0], GL_STATIC_DRAW);
  //
  // glBindVertexArray(lampshadeVAO);
  // // Position attribute
  // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  // glEnableVertexAttribArray(0);
  //
  // // Normal attribute
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  // glEnableVertexAttribArray(1);
  //
  // // Texture coordinate attribute
  // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  // glEnableVertexAttribArray(2);
  //
  // // load textures (we now use a utility function to keep the code more organized)
  // // -----------------------------------------------------------------------------
  // unsigned int diffuseMaplampshade = loadTexture("/Users/erikbayerlein/Documents/cg3d/src/resources/textures/lampshade.jpg");




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
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.use();
    lightingShader.setVec3("light.position", lightPos);
    lightingShader.setVec3("viewPos", camera.Position);

    // light properties
    lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
    lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.setFloat("material.shininess", 24.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    lightingShader.setMat4("projection", projection);
    lightingShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    lightingShader.setMat4("model", model);


  // ------------------------------------------ WALL ------------------------------------------ //
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapWall);

    size_t vertexCount = wall.size()/8;

    // render wall
    glBindVertexArray(WallVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));


  // ------------------------------------------ SHELVES ------------------------------------------ //
    // Bind the diffuse map for the second object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapShelves);

    // Model transformation for the second object
    // glm::mat4 model2 = glm::mat4(1.0f);
    // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // lightingShader.setMat4("model", model2);

    size_t shelvesVertexCount = shelves.size() / 8;

    // Render the second object
    glBindVertexArray(shelvesVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(shelvesVertexCount));
    



  // ------------------------------------------ FLOOR ------------------------------------------ //
    // Bind the diffuse map for the second object
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, diffuseMapfloor);
    //
    // // Model transformation for the second object
    // // glm::mat4 model2 = glm::mat4(1.0f);
    // // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // // lightingShader.setMat4("model", model2);
    //
    // size_t floorVertexCount = floor.size() / 8;
    //
    // // Render the second object
    // glBindVertexArray(floorVAO);
    // glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(floorVertexCount));



  // ------------------------------------------ BED ------------------------------------------ //
    // Bind the diffuse map for the second object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapbed);

    // Model transformation for the second object
    // glm::mat4 model2 = glm::mat4(1.0f);
    // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // lightingShader.setMat4("model", model2);

    size_t bedVertexCount = bed.size() / 8;

    // Render the second object
    glBindVertexArray(bedVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(bedVertexCount));



  // ------------------------------------------ MATTRESS ------------------------------------------ //
    // Bind the diffuse map for the second object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapmattress);

    // Model transformation for the second object
    // glm::mat4 model2 = glm::mat4(1.0f);
    // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // lightingShader.setMat4("model", model2);

    size_t mattressVertexCount = mattress.size() / 8;

    // Render the second object
    glBindVertexArray(mattressVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(mattressVertexCount));






  // ------------------------------------------ PILLOW ------------------------------------------ //
    // // Bind the diffuse map for the second object
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, diffuseMappillow);
    //
    // // Model transformation for the second object
    // // glm::mat4 model2 = glm::mat4(1.0f);
    // // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // // lightingShader.setMat4("model", model2);
    //
    // size_t pillowVertexCount = pillow.size() / 8;
    //
    // // Render the second object
    // glBindVertexArray(pillowVAO);
    // glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(pillowVertexCount));



  // ------------------------------------------ box ------------------------------------------ //
    // Bind the diffuse map for the second object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapbox);

    // Model transformation for the second object
    // glm::mat4 model2 = glm::mat4(1.0f);
    // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // lightingShader.setMat4("model", model2);

    size_t boxVertexCount = box.size() / 8;

    // Render the second object
    glBindVertexArray(boxVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(boxVertexCount));




  // ------------------------------------------ baseLampshade ------------------------------------------ //
    // Bind the diffuse map for the second object
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMapbaseLampshade);

    // Model transformation for the second object
    // glm::mat4 model2 = glm::mat4(1.0f);
    // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // lightingShader.setMat4("model", model2);

    size_t baseLampshadeVertexCount = baseLampshade.size() / 8;

    // Render the second object
    glBindVertexArray(baseLampshadeVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(baseLampshadeVertexCount));






  // ------------------------------------------ lampshade ------------------------------------------ //
    // // Bind the diffuse map for the second object
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, diffuseMaplampshade);
    //
    // // Model transformation for the second object
    // // glm::mat4 model2 = glm::mat4(1.0f);
    // // model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // Move the second object
    // // lightingShader.setMat4("model", model2);
    //
    // size_t lampshadeVertexCount = lampshade.size() / 8;
    //
    // // Render the second object
    // glBindVertexArray(lampshadeVAO);
    // glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(lampshadeVertexCount));








  // ------------------------------------------ LAMP ------------------------------------------ //
    lightCubeShader.use();
    lightCubeShader.setMat4("projection", projection);
    lightCubeShader.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    lightCubeShader.setMat4("model", model);

    glBindVertexArray(lightCubeWallVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertexCount));


    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &WallVAO);
    // glDeleteVertexArrays(1, &lightCubeWallVAO);
    // glDeleteBuffers(1, &WallVBO);

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
