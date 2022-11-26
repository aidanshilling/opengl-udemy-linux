# OpenGL-Udemy-Linux

Code for opengl Udemy course, setup for linux environment.

### Instructions

Be sure to have the following packages installed on your OS:

- vcpkg

Modify the path to your vcpkg install in the `CMakeLists.txt` file in the CMAKE_TOOLCHAIN_FILE

- GLEW
- GLFW3



1. `mkdir out`
2. `cmake -S . -B /out`
3. `cmake --build /out`
4. `./out/opengl-udemy`
