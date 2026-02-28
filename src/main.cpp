#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core/Camera.h"
#include "platform/Window.h"
#include "platform/Input.h"
#include "rendering/Shader.h"
#include "rendering/Mesh.h"

#include <chrono>
#include <string>
#include <cstdio>

// Resolve the shader directory from argv[0]
static std::string exeDir(const char* argv0)
{
    std::string path(argv0);
    auto pos = path.find_last_of("/\\");
    if (pos != std::string::npos)
        return path.substr(0, pos);
    return ".";
}

int main(int /*argc*/, char* argv[])
{
    const std::string dir = exeDir(argv[0]);

    Window window(1280, 720, "3d-test");
    InputState input;
    inputAttach(window.handle(), input);

    Camera camera;
    camera.updateVectors();

    Shader shader(dir + "/shaders/object.vert",
                  dir + "/shaders/object.frag");

    Mesh cube   = Mesh::buildCube();
    Mesh sphere = Mesh::buildSphere(16, 16);

    // Lighting constants
    const glm::vec3 lightDir    = glm::normalize(glm::vec3(0.4f, -1.0f, 0.3f));
    const glm::vec3 lightColor  = glm::vec3(1.0f);
    const float     ambient     = 0.15f;
    const float     moveSpeed   = 5.0f;
    const float     mouseSens   = 0.1f;

    using Clock = std::chrono::steady_clock;
    auto prev = Clock::now();

    while (!window.shouldClose()) {
        inputBeginFrame(input);
        window.pollEvents();

        // Delta time
        auto now = Clock::now();
        float dt = std::chrono::duration<float>(now - prev).count();
        prev = now;
        if (dt > 0.05f) dt = 0.05f; // cap at 50ms

        // ESC → close
        if (inputKey(input, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window.handle(), GLFW_TRUE);

        // Camera: mouse look
        camera.processMouseDelta(input.mouseDeltaX, input.mouseDeltaY, mouseSens);

        // Camera: WASD + QE
        camera.processMovement(
            inputKey(input, GLFW_KEY_W),
            inputKey(input, GLFW_KEY_S),
            inputKey(input, GLFW_KEY_A),
            inputKey(input, GLFW_KEY_D),
            inputKey(input, GLFW_KEY_E),
            inputKey(input, GLFW_KEY_Q),
            moveSpeed, dt);

        // Render
        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, window.width(), window.height());

        float aspect = static_cast<float>(window.width()) / static_cast<float>(window.height());
        glm::mat4 proj = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
        glm::mat4 view = camera.viewMatrix();

        shader.use();
        shader.setMat4("view",       view);
        shader.setMat4("projection", proj);
        shader.setVec3("lightDir",   lightDir);
        shader.setVec3("lightColor", lightColor);
        shader.setFloat("ambientStrength", ambient);

        // Cube at origin
        {
            glm::mat4 model = glm::mat4(1.0f);
            shader.setMat4("model",       model);
            shader.setVec3("objectColor", glm::vec3(0.8f, 0.4f, 0.2f));
            cube.draw();
        }

        // Sphere at (3, 1, 0)
        {
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 1.0f, 0.0f));
            shader.setMat4("model",       model);
            shader.setVec3("objectColor", glm::vec3(0.3f, 0.6f, 0.9f));
            sphere.draw();
        }

        window.swapBuffers();
    }

    cube.destroy();
    sphere.destroy();

    return 0;
}
