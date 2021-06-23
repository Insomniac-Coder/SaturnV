# Saturn Engine
Saturn is in development interactive application and rendering engine primarily for Windows but can work on Linux and Mac with a few tweaks, most of the features in this repository are in development

## Getting Started
Visual Studio 2017 or 2019 is recommended, Saturn is officially untested on other development environments whilst we focus on a Windows build.

Start by cloning the repository with git clone https://github.com/Insomniac-Coder/SaturnV.

## The Plan
The plan is to develop an Engine which is capable of loading 3D models and has basic features like Batch rendering, lighting, shadows and skybox implemented, this implementation has batch rendering implemented to a certain degree and is capable of rendering hundreds of objects and millions of vertices in just a couple of draw calls!

### Main features to come:
- Fully automatic batch renderer which takes care of the process by taking materials, shaders and vertex buffer size limit into account
- On the fly shader creation, it's pretty common feature in modern game engines, in which shaders get modified/created depending upon the requirements of the objects in the scene
- Procedural skybox
- Shadow (soft/hard)
- Input management system

## What the project has as of now?
Currently the project is capable of doing the intial setup that is requitrf fot rendering on to the screen, as we know that Vulkan is really VERBOSE! So the project currently just creates anw window, lists all the GPUs on the system and pick the suitable one for rendering and post that it proceeds to create a render pass , pgraphics pipeline, swapchain, frame buffers and command buffers. on starting the project you'll just be able to see a rectangle being rendered on the screen with Indexed draw call. Features like Batch rendering have already been implemented on my Opengl version of this engine which you can check out by going to https://github.com/Insomniac-Coder/Saturn-Engine
