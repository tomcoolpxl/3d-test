#pragma once

#include <glad/gl.h>

struct Mesh {
    GLuint   vao{0};
    GLuint   vbo{0};
    GLuint   ebo{0};
    GLsizei  indexCount{0};

    void draw()    const;
    void destroy();

    static Mesh buildCube();
    static Mesh buildSphere(int rings = 16, int sectors = 16);
};
