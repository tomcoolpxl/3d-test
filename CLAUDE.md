# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

Minimal cross-platform C++20 3D application using OpenGL 4.5, GLFW 3.4, GLM 1.0.1, and glad2 v2.0.4. Renders a cube and sphere with an FPS-style free-fly camera. Intended as a clean rendering/platform foundation for future physics work.

## Build Commands

```bash
# Configure (from repo root)
cmake -B build -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build build

# Run
./build/3d-test          # Linux/WSL2
build\Release\3d-test.exe  # Windows
```

Dependencies (GLFW, GLM, glad2) are fetched automatically via CMake `FetchContent`.

**One-time Linux/WSL2 prerequisites:**
```bash
# X11/Wayland headers for GLFW
sudo apt install libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libwayland-dev libxkbcommon-dev

# Python jinja2 for glad2 code generation
pip install jinja2 --break-system-packages
```

## Source Layout

```
shaders/
  object.vert       # Vertex shader: position + normal transform
  object.frag       # Fragment shader: Lambert directional lighting
src/
  main.cpp          # Entry point, frame loop
  core/
    Camera.h/.cpp   # FPS camera: yaw/pitch, view matrix, movement
  rendering/
    Shader.h/.cpp   # File-based shader load/compile/link, uniform setters
    Mesh.h/.cpp     # VAO/VBO/EBO; buildCube() and buildSphere() factories
  platform/
    Window.h/.cpp   # RAII GLFW window + glad2 init
    Input.h/.cpp    # Key + cursor callbacks via glfwSetWindowUserPointer
```

## Architecture Rules

**Layer separation is strict:**
- `core/` must have zero OpenGL or GLFW dependencies
- `rendering/` owns all VAO/VBO/EBO/shader state
- `platform/` owns all GLFW interaction and `std::chrono` timing
- No global variables except a single application state container
- No inheritance hierarchy, no virtual functions
- No per-frame heap allocations

**Cross-platform constraints:**
- Never include `windows.h` or use Win32 APIs
- Use `std::chrono` for timing (not platform timers)
- Use GLFW exclusively for input and windowing
- All file paths must be relative

## Key Decisions

**Coordinate system:** Right-handed, Y-up, -Z forward (OpenGL convention)

**Scene:** Cube (unit, centered at origin), Sphere (radius 0.5, at position 3,1,0)

**Camera:** FPS free-fly; WASD for movement, QE for vertical, mouse for yaw/pitch; 60° FOV, near 0.1, far 100.0; pitch clamped ±89°

**Mouse input:** `GLFW_CURSOR_DISABLED` + `GLFW_RAW_MOUSE_MOTION` (checked via `glfwRawMouseMotionSupported()`). Raw motion is required on WSL2/Wayland — cursor disabled alone does not deliver events reliably.

**Rendering uniforms:**
- Per-frame: `mat4 view`, `mat4 projection`
- Per-object: `mat4 model`, `vec3 objectColor`
- Lighting: `vec3 lightDir`, `vec3 lightColor`, `float ambientStrength`

**Lighting:** Lambert directional shading, light direction `(0.4, -1.0, 0.3)`, ambient 0.15, no shadows

**Meshes:** Cube — 24 vertices (4/face × 6), indexed (36 indices). Sphere — UV sphere 16×16, radius 0.5, indexed. Both interleaved as `[pos.xyz, normal.xyz]` with stride 6 floats.

**Mesh RAII:** `Mesh` is a plain struct with explicit `destroy()` — no destructor, avoids move-semantics friction on static factory returns.

**Shader path:** Shaders live in `shaders/` at repo root. CMake POST_BUILD copies them to `$<TARGET_FILE_DIR>/shaders/`. `main.cpp` resolves the directory from `argv[0]` (no platform APIs).

**GLAD:** glad2 v2.0.4 via `FetchContent` with `SOURCE_SUBDIR cmake`. Library target: `glad_gl45 REPRODUCIBLE LOADER API gl:core=4.5`. Include: `<glad/gl.h>`. Init: `gladLoadGL(glfwGetProcAddress)`. Requires `GLFW_INCLUDE_NONE` compile definition to prevent GLFW from pulling in its own GL headers.

## CMake Notes

- Project must declare `LANGUAGES C CXX` — glad2 generates C code
- `set(CMAKE_POLICY_VERSION_MINIMUM 3.5)` required before FetchContent on CMake 4.x (glad2's `cmake_minimum_required` predates 3.5)
- Link `dl` on non-Apple Unix

## Compiler Settings

| Platform | Toolset | Flags |
|----------|---------|-------|
| Windows | Clang-cl (VS2022) | `/W4 /O2` |
| Linux | GCC or Clang | `-Wall -Wextra -O3` |

No compiler-specific extensions. No fast-math.
