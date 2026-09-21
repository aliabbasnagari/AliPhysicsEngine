cmake_minimum_required(VERSION 3.16)
project(AliPhysicsEngine LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Put all binaries in a single, predictable location regardless of generator
# (single-config like Makefiles/Ninja, or multi-config like Visual Studio).
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
endforeach()

# ---------------------------------------------------------------------------
# GLFW (windowing / input / GL context creation)
# ---------------------------------------------------------------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
# Wayland support requires wayland-scanner/wayland-protocols, which is not
# always available (e.g. WSL). X11 alone is enough since WSLg exposes an
# X11 display.
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(external/glfw)

# ---------------------------------------------------------------------------
# glad (OpenGL function loader)
# ---------------------------------------------------------------------------
add_library(glad STATIC external/glad/src/gl.c)
target_include_directories(glad PUBLIC external/glad/include)

# ---------------------------------------------------------------------------
# Dear ImGui (core + GLFW/OpenGL3 backends)
# ---------------------------------------------------------------------------
add_library(imgui STATIC
    external/imgui/imgui.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
    external/imgui
    external/imgui/backends
)
target_link_libraries(imgui PUBLIC glfw)

# ---------------------------------------------------------------------------
# Engine test app
# ---------------------------------------------------------------------------
add_executable(AliPhysicsEngine 
    src/main.cpp
    src/core/Application.cpp
    src/graphics/Renderer.cpp
    src/physics/Particle.cpp
    src/physics/VerletParticle.cpp
    src/physics/PhysicsWorld.cpp
    )
target_include_directories(AliPhysicsEngine PRIVATE src)
target_link_libraries(AliPhysicsEngine PRIVATE glfw glad imgui)
