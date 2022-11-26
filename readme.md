# OpenGL-Udemy-Linux

Code for opengl Udemy course, setup for linux environment.

### Instructions

Be sure to have the following packages installed on your OS:

- vcpkg

Modify the path to your vcpkg install in the `CMakeLists.txt` for the CMAKE_TOOLCHAIN_FILE directive

- GLEW
- GLFW3
- GLM



1. `mkdir out`
2. `cmake -S . -B /out`
3. `cmake --build /out`
4. `./out/opengl-udemy`
