# =============================================================================
# CMakeLists.txt — Eden City
# =============================================================================
# Build system: CMake 3.16+
# Compiler: GCC/G++ (MinGW-W64) or any C++17-capable compiler
# Tested: cmake --build build --config Debug
# =============================================================================

cmake_minimum_required(VERSION 3.16)
project(EdenCity VERSION 0.1 LANGUAGES C CXX)

# -----------------------------------------------------------------------------
# C++ Standard
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Renderer.cpp
    src/ShaderProgram.cpp
    src/Chunk.cpp
    src/World.cpp
    src/WorldGenerator.cpp
    src/MeshBuilder.cpp
    src/Camera.cpp
    src/InputManager.cpp
    src/Player.cpp
    src/Debug.cpp
    lib/glad/src/glad.c          # OpenGL function loader
)

# glad.c is a generated third-party file — suppress its pedantic warnings
set_source_files_properties(lib/glad/src/glad.c PROPERTIES
    COMPILE_FLAGS "-Wno-pedantic"
)

# (Week 1 additions will be listed here as they are created)
# src/Camera.cpp
# src/InputManager.cpp
# src/Player.cpp
# src/Debug.cpp

# -----------------------------------------------------------------------------
# Executable
# -----------------------------------------------------------------------------
add_executable(EdenCity ${SOURCES})

# -----------------------------------------------------------------------------
# Include directories
# -----------------------------------------------------------------------------
target_include_directories(EdenCity PRIVATE
    ${CMAKE_SOURCE_DIR}/include          # our headers
    ${CMAKE_SOURCE_DIR}/lib/glad/include # glad.h + KHR/khrplatform.h
    ${CMAKE_SOURCE_DIR}/lib             # glm/glm.hpp  (lib/glm/glm.hpp)
    ${CMAKE_SOURCE_DIR}/lib/freeglut/include  # GL/glut.h, GL/freeglut.h
)

# -----------------------------------------------------------------------------
# Link libraries
# -----------------------------------------------------------------------------
target_link_directories(EdenCity PRIVATE
    ${CMAKE_SOURCE_DIR}/lib/freeglut/lib/x64
)

if(WIN32)
    target_link_libraries(EdenCity PRIVATE
        opengl32          # Windows system OpenGL
        ${CMAKE_SOURCE_DIR}/lib/freeglut/lib/x64/libfreeglut.dll.a # MinGW: absolute path
        gdi32             # required by freeglut on Windows
        winmm             # Windows multimedia (freeglut dependency)
    )
else()
    # Linux / macOS fallback
    find_package(OpenGL REQUIRED)
    find_package(GLUT REQUIRED)
    target_link_libraries(EdenCity PRIVATE OpenGL::GL GLUT::GLUT)
endif()

# -----------------------------------------------------------------------------
# Compiler warnings — Warning Level 4 equivalent
# -----------------------------------------------------------------------------
if(MSVC)
    target_compile_options(EdenCity PRIVATE /W4)
else()
    target_compile_options(EdenCity PRIVATE
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter   # acceptable during active development
    )
    # Statically link MinGW standard libraries to avoid missing DLL errors
    target_link_options(EdenCity PRIVATE -static-libgcc -static-libstdc++ -static)
endif()

# -----------------------------------------------------------------------------
# Post-build: copy freeglut DLL next to the exe (Windows only)
# -----------------------------------------------------------------------------
if(WIN32)
    add_custom_command(TARGET EdenCity POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/lib/freeglut/bin/x64/libfreeglut.dll"
            "$<TARGET_FILE_DIR:EdenCity>/libfreeglut.dll"
        COMMENT "Copying libfreeglut.dll to output directory"
    )
endif()

# -----------------------------------------------------------------------------
# Output directory — keep build artifacts in build/
# -----------------------------------------------------------------------------
set_target_properties(EdenCity PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Shader files are referenced at runtime relative to exe, so we copy them too
add_custom_command(TARGET EdenCity POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:EdenCity>/assets/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/assets/shaders/basic.vert"
        "$<TARGET_FILE_DIR:EdenCity>/assets/shaders/basic.vert"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/assets/shaders/basic.frag"
        "$<TARGET_FILE_DIR:EdenCity>/assets/shaders/basic.frag"
    COMMENT "Copying shaders to output directory"
)
