#include "Input.h"

static void keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
    auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(window));
    if (!state) return;
    if (key < 0 || key > GLFW_KEY_LAST) return;
    if (action == GLFW_PRESS)   state->keys[key] = true;
    if (action == GLFW_RELEASE) state->keys[key] = false;
}

static void cursorCallback(GLFWwindow* window, double xpos, double ypos)
{
    auto* state = static_cast<InputState*>(glfwGetWindowUserPointer(window));
    if (!state) return;

    if (state->firstCursor) {
        state->lastCursorX = xpos;
        state->lastCursorY = ypos;
        state->firstCursor = false;
    }

    state->mouseDeltaX += static_cast<float>(xpos - state->lastCursorX);
    state->mouseDeltaY += static_cast<float>(state->lastCursorY - ypos); // Y inverted
    state->lastCursorX  = xpos;
    state->lastCursorY  = ypos;
}

void inputAttach(GLFWwindow* window, InputState& state)
{
    glfwSetWindowUserPointer(window, &state);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void inputBeginFrame(InputState& state)
{
    state.mouseDeltaX = 0.0f;
    state.mouseDeltaY = 0.0f;
}

bool inputKey(const InputState& state, int key)
{
    if (key < 0 || key > GLFW_KEY_LAST) return false;
    return state.keys[key];
}
