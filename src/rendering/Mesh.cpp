#include "Mesh.h"

#include <vector>
#include <cmath>

// ---------- helpers ----------

static Mesh uploadMesh(const std::vector<float>& verts, const std::vector<unsigned>& indices)
{
    Mesh m;
    m.indexCount = static_cast<GLsizei>(indices.size());

    glGenVertexArrays(1, &m.vao);
    glGenBuffers(1, &m.vbo);
    glGenBuffers(1, &m.ebo);

    glBindVertexArray(m.vao);

    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(verts.size() * sizeof(float)),
                 verts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned)),
                 indices.data(), GL_STATIC_DRAW);

    // position (loc 0) — stride = 6 floats (pos + normal)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    // normal (loc 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    return m;
}

// ---------- public API ----------

void Mesh::draw() const
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::destroy()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    vao = vbo = ebo = 0;
    indexCount = 0;
}

// ---------- cube ----------

Mesh Mesh::buildCube()
{
    // 24 vertices: 4 per face, 6 faces.  Layout: px py pz nx ny nz
    // clang-format off
    std::vector<float> v = {
        // +X face
         0.5f,  0.5f,  0.5f,   1,0,0,
         0.5f, -0.5f,  0.5f,   1,0,0,
         0.5f, -0.5f, -0.5f,   1,0,0,
         0.5f,  0.5f, -0.5f,   1,0,0,
        // -X face
        -0.5f,  0.5f, -0.5f,  -1,0,0,
        -0.5f, -0.5f, -0.5f,  -1,0,0,
        -0.5f, -0.5f,  0.5f,  -1,0,0,
        -0.5f,  0.5f,  0.5f,  -1,0,0,
        // +Y face
        -0.5f,  0.5f, -0.5f,   0,1,0,
        -0.5f,  0.5f,  0.5f,   0,1,0,
         0.5f,  0.5f,  0.5f,   0,1,0,
         0.5f,  0.5f, -0.5f,   0,1,0,
        // -Y face
        -0.5f, -0.5f,  0.5f,   0,-1,0,
        -0.5f, -0.5f, -0.5f,   0,-1,0,
         0.5f, -0.5f, -0.5f,   0,-1,0,
         0.5f, -0.5f,  0.5f,   0,-1,0,
        // +Z face
        -0.5f,  0.5f,  0.5f,   0,0,1,
        -0.5f, -0.5f,  0.5f,   0,0,1,
         0.5f, -0.5f,  0.5f,   0,0,1,
         0.5f,  0.5f,  0.5f,   0,0,1,
        // -Z face
         0.5f,  0.5f, -0.5f,   0,0,-1,
         0.5f, -0.5f, -0.5f,   0,0,-1,
        -0.5f, -0.5f, -0.5f,   0,0,-1,
        -0.5f,  0.5f, -0.5f,   0,0,-1,
    };
    // clang-format on

    std::vector<unsigned> idx;
    idx.reserve(36);
    for (unsigned f = 0; f < 6; ++f) {
        unsigned b = f * 4;
        idx.insert(idx.end(), {b,b+1,b+2, b,b+2,b+3});
    }

    return uploadMesh(v, idx);
}

// ---------- sphere ----------

Mesh Mesh::buildSphere(int rings, int sectors)
{
    constexpr float PI = 3.14159265358979f;
    const float R = 0.5f;

    std::vector<float>    verts;
    std::vector<unsigned> indices;

    for (int r = 0; r <= rings; ++r) {
        float phi = PI * r / rings; // 0..PI
        for (int s = 0; s <= sectors; ++s) {
            float theta = 2.0f * PI * s / sectors; // 0..2PI
            float x = std::cos(theta) * std::sin(phi);
            float y = std::cos(phi);
            float z = std::sin(theta) * std::sin(phi);
            // position
            verts.push_back(x * R);
            verts.push_back(y * R);
            verts.push_back(z * R);
            // normal (unit sphere → normal == position / R)
            verts.push_back(x);
            verts.push_back(y);
            verts.push_back(z);
        }
    }

    int stride = sectors + 1;
    for (int r = 0; r < rings; ++r) {
        for (int s = 0; s < sectors; ++s) {
            unsigned tl = static_cast<unsigned>(r       * stride + s);
            unsigned tr = static_cast<unsigned>(r       * stride + s + 1);
            unsigned bl = static_cast<unsigned>((r + 1) * stride + s);
            unsigned br = static_cast<unsigned>((r + 1) * stride + s + 1);
            indices.insert(indices.end(), {tl, bl, tr,  tr, bl, br});
        }
    }

    return uploadMesh(verts, indices);
}
