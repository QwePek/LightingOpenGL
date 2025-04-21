# LightingOpenGL

A C++ and OpenGL project exploring various real-time lighting techniques and models. Inspired by and partially based on tutorials from [LearnOpenGL.com](https://learnopengl.com/). This project uses Premake to generate project files. Unfortunately repository size exceeds 1GB, because of the use of `.HDR` files for PBR rendering. 

## Features

* Renders a 3D scene using OpenGL (Core Profile 3.3+).
* Implementation of various lighting models, switchable in real-time:
    * **Phong**
    * **Blinn-Phong**
    * **Blinn-Phong with Normal Mapping**
    * **Physically Based Rendering (PBR)** with Image-Based Lighting (IBL) basics (cubemap loading, irradiance map generation shown).
* Depth buffer visualization
* Loading and rendering of 3D models (using Assimp for `.obj` format, e.g., backpack model).
* Generation of simple primitives (cubes, spheres) with random materials.
* Loading textures (including normal maps and HDR maps for the skybox).
* Skybox implementation using a Cubemap (generated from an HDR map).
* User Interface (GUI) using ImGui for inspecting and modifying parameters (e.g., camera, objects, lights).
* Fly-through camera controls using mouse and keyboard.
* Support for dynamic directional and point lights.

## Dependencies

This project uses the following libraries (included in the `Dependencies` or `res/libraries` folder):

* **GLFW**: Window and input handling.
* **GLEW**: OpenGL extension loading.
* **GLM**: Mathematics library for graphics.
* **ImGui**: User interface library.
* **Assimp**: 3D model loading.
* **stb_image**: Image/texture loading.

## Building the Project

1.  **Prerequisites:**
    * A C++ compiler supporting C++20 (e.g., Visual Studio 2022).

2.  **Steps:**
    * Clone the repository: `git clone <REPOSITORY_URL>`
    * Launch `GenerateProjects.bat` file or navigate to projects root directory in terminal and launch `Dependencies\premake5\bin\premake5.exe vs2022` or get premake from [repository](https://github.com/premake/premake-core)
    * Open the generated `.sln` file (e.g., `LightingOpenGL.sln`) in Visual Studio.
    * Build the solution (Compile).

## Controls

* **WASD**: Move camera (forward, back, left, right).
* **Left Shift**: Move faster.
* **T**: Enable mouse look (capture cursor).
* **R**: Disable mouse look (release cursor).
* **Mouse**: Look around (when cursor is captured).
* **Mouse Wheel / Scroll**: Zoom / Adjust Field of View (FOV).
* **X (Hold)**: Toggle Wireframe mode.
* **1**: Switch to Phong shader.
* **2**: Switch to Blinn-Phong shader.
* **3**: Switch to Blinn-Phong shader with Normal Mapping.
* **4**: Switch to PBR shader.
* **0**: Switch to Depth Buffer view.
* **ESC**: Exit application.

## Screenshots
![image](https://github.com/user-attachments/assets/cbb018a1-3972-47f4-8a17-2be4caddc066)

![image](https://github.com/user-attachments/assets/be0b1616-97d8-4f7f-a450-f489f3f0f040)
