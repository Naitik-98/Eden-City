# 🏙️ Eden City

A futuristic voxel-based 3D exploration game built with **C++17**, **Modern OpenGL 3.3**, and **freeglut**.

---

## About

Eden City is a floating city made from voxel blocks. Players explore interconnected floating islands, discover Crystal Towers, and unlock paths to new areas.

The focus is on **exploration and clean graphics** — not survival.

**No** combat. **No** crafting. **No** inventory. **No** hunger.  
Just a futuristic city waiting to be explored.

---

## Tech Stack

| Component | Technology |
|---|---|
| Language | C++17 |
| Graphics | OpenGL 3.3 (Core Profile) |
| Windowing | freeglut |
| GL Loader | GLAD |
| Math | GLM |
| IDE | Visual Studio 2022 |
| Platform | Windows x64 |

---

## Building

### Prerequisites
- Visual Studio 2022 with C++ Desktop Development workload
- Libraries in `lib/` (see Setup below)

### Setup
1. Download **GLAD** (OpenGL 3.3 Core) from [glad.dav1d.de](https://glad.dav1d.de) → place in `lib/glad/`
2. Download **GLM** from [GitHub](https://github.com/g-truc/glm/releases) → place in `lib/glm/`
3. Download **freeglut** MSVC package from [transmissionzero.co.uk](https://www.transmissionzero.co.uk/software/freeglut-devel/) → place in `lib/freeglut/`

### Build & Run
1. Open `EdenCity.sln` in Visual Studio 2022
2. Set configuration to **Debug | x64**
3. Press **F5** to build and run

---

## Controls

| Key | Action |
|---|---|
| **W/A/S/D** | Move |
| **Mouse** | Look around |
| **Space** | Jump |
| **Left Shift** | Sprint |
| **F3** | Debug overlay (FPS, position) |
| **F4** | Wireframe mode |
| **ESC** | Exit |

---

## Project Structure

```
Eden-City/
├── include/     Header files (.h)
├── src/         Source files (.cpp)
├── assets/      Shaders, textures, etc.
├── lib/         Third-party libraries
└── build/       Build output (generated)
```

---

## Architecture

The engine follows a two-layer architecture:
- **Logic Layer** (World, Chunk, Player, Camera) — pure data and math, no OpenGL
- **Rendering Layer** (Renderer, ShaderProgram, MeshBuilder) — all GPU interaction

See the implementation plan for detailed class diagrams.

---

