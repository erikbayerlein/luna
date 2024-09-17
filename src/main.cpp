#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaders/shader.h>
#include <camera/Camera.h>
#include <RenderObject.h>
#include <RenderLight.h>

#include "glm/fwd.hpp"
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

glm::vec3 lightPos(-1.0f, 1.2f, 0.8f);


int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

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

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  Shader objectShader(OBJECT);
  Shader lightShader(LIGHTSOURCE);


  RenderObject water(
    "/Users/erikbayerlein/Documents/cg3d/src/resources/models/water.obj",
    "/Users/erikbayerlein/Documents/cg3d/src/resources/textures/water.jpeg",
    glm::vec3(0.5f, 0.5f, 0.5f),
    128.0f
  );

  RenderObject dirt(
    "/Users/erikbayerlein/Documents/cg3d/src/resources/models/dirt.obj",
    "/Users/erikbayerlein/Documents/cg3d/src/resources/textures/dirt.jpg",
    glm::vec3(0.3f, 0.3f, 0.3f),
    16.0f
  );

  RenderObject grass(
    "/Users/erikbayerlein/Documents/cg3d/src/resources/models/grass.obj",
    "/Users/erikbayerlein/Documents/cg3d/src/resources/textures/grass.jpg",
    glm::vec3(0.4f, 0.4f, 0.4f),
    32.0f
  );

  RenderObject wood(
    "/Users/erikbayerlein/Documents/cg3d/src/resources/models/wood.obj",
    "/Users/erikbayerlein/Documents/cg3d/src/resources/textures/wood.jpeg",
    glm::vec3(0.3f, 0.2f, 0.2f),
    24.0f
  );

  RenderObject bridge(
    "/Users/erikbayerlein/Documents/cg3d/src/resources/models/bridge.obj",
    "/Users/erikbayerlein/Documents/cg3d/src/resources/textures/wood.jpeg",
    glm::vec3(0.3f, 0.2f, 0.2f),
    24.0f
  );

  RenderObject stone(
    "/Users/erikbayerlein/Documents/cg3d/src/resources/models/stone.obj",
    "/Users/erikbayerlein/Documents/cg3d/src/resources/textures/stone.jpg",
    glm::vec3(0.1f, 0.1f, 0.1f),
    8.0f
  );

  RenderObject leaves(
    "/Users/erikbayerlein/Documents/cg3d/src/resources/models/leaves.obj",
    "/Users/erikbayerlein/Documents/cg3d/src/resources/textures/leaves.jpg",
    glm::vec3(0.2f, 0.3f, 0.2f),
    16.0f
  );


  RenderLight sun("/Users/erikbayerlein/Documents/cg3d/src/resources/models/sun.obj");

  glEnable(GL_CULL_FACE);

  // shader configuration
  // --------------------
  objectShader.use(); 
  objectShader.setInt("material.diffuse", 0);


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

    // Update light position based on time
    float orbitRadius = 4.0f;
    float orbitSpeed = glfwGetTime() * 1.4f;

    float lightZ = orbitRadius * cos(orbitSpeed);
    float lightY = orbitRadius * sin(orbitSpeed);
    glm::vec3 lightPos = glm::vec3(0.0f, lightY, lightZ);

    // be sure to activate shader when setting uniforms/drawing objects
    objectShader.use();
    objectShader.setVec3("light.position", lightPos);
    objectShader.setVec3("viewPos", camera.Position);

    // light properties
    objectShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f); 
    objectShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    objectShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    objectShader.setMat4("projection", projection);
    objectShader.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    objectShader.setMat4("model", model);

    water.render(objectShader);
    dirt.render(objectShader);
    grass.render(objectShader);
    stone.render(objectShader);
    wood.render(objectShader);
    leaves.render(objectShader);
    bridge.render(objectShader);


    glfwSwapBuffers(window);
    glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

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
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}
