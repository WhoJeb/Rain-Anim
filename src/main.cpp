#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../include/shader.hpp"

#include <iostream>
#include <vector>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const int INSTANCE_COUNT = 1000;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main() {
    // glfw init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rain Animation", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // load GL function pointers
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to init GLEW\n";
        return -1;
    }

    // Build and compile shader
    Shader s("src/shaders/shader.vert",
             "src/shaders/shader.frag");

    // Raindrop rectangle (two triangles)
    float vertices[] = {
        // pos (x,y,z)                      // color (r,g,b)
        -0.001f, -0.1f, 0.0f,   0.0f, 0.35f, 0.5f,
         0.001f, -0.1f, 0.0f,   0.0f, 0.35f, 0.5f,
         0.001f,  0.1f, 0.0f,   0.0f, 0.35f, 0.5f,
        -0.001f,  0.1f, 0.0f,   0.0f, 0.35f, 0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::vector<glm::vec2> offsets(INSTANCE_COUNT);
    for (int i = 0; i < INSTANCE_COUNT; i++) {
        float x = ((rand() % 2000) - 1000) / 500.0f; // random X in [-1, 1]
        float y = ((rand() % 1000) / 100.0f);         // random Y in [0, 5?]
        offsets[i] = glm::vec2(x, y);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // VAO + VBO + EBO
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Instance VBO
    GLuint instanceVBO;
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, INSTANCE_COUNT * sizeof(glm::vec2), offsets.data(), GL_DYNAMIC_DRAW);

    // Instance offset attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void *)0);
    glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);

    // Projection & View (2D orthographic)
    glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    float topY = 1.2f;
    float bottomY = -1.2f;
    float right = 1.2f;
    float left = -1.2f;

    float lastFrameTime = glfwGetTime();

    // render loop
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        // Update falling rain
        for (int i = 0; i < INSTANCE_COUNT; i++) {
          offsets[i].y -= 3.0f * deltaTime;
          // offsets[i].x -= ((rand() % 50) - 25.0f)  * deltaTime;

          if (offsets[i].y < bottomY) {
            offsets[i].y = topY;
          }
        }

        // Upload new instance positions
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, INSTANCE_COUNT * sizeof(glm::vec2), offsets.data());

        s.use();
        s.setMat4("projection", projection);
        s.setMat4("view", view);

        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, INSTANCE_COUNT);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

