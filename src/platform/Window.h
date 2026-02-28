#pragma once

#include <GLFW/glfw3.h>

class Window {
public:
    explicit Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&)            = delete;
    Window& operator=(const Window&) = delete;

    bool       shouldClose() const;
    void       swapBuffers();
    void       pollEvents();
    GLFWwindow* handle() const { return m_window; }
    int        width()   const { return m_width; }
    int        height()  const { return m_height; }

private:
    GLFWwindow* m_window{nullptr};
    int         m_width;
    int         m_height;
};
