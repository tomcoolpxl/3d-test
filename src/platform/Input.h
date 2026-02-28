#pragma once

#include <GLFW/glfw3.h>

struct InputState {
    bool   keys[GLFW_KEY_LAST + 1]{};
    float  mouseDeltaX{0.0f};
    float  mouseDeltaY{0.0f};
    double lastCursorX{0.0};
    double lastCursorY{0.0};
    bool   firstCursor{true};
};

void inputAttach(GLFWwindow* window, InputState& state);
void inputBeginFrame(InputState& state);
bool inputKey(const InputState& state, int key);
