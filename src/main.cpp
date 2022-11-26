#include <iostream>
#include <cmath>
#include <numbers>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadians = std::numbers::pi_v<float> / 180.0f;

GLuint VAO, VBO, shader, uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

// Shader code
static const char *vshader =
"                                                    \n\
#version 330                                         \n\
layout (location = 0) in vec3 pos;                   \n\
uniform mat4 model;                                 \n\
void main() {                                        \n\
	gl_Position = model * vec4(0.4 * pos.x, 0.4 * pos.y, pos.z, 1.0);    \n\
}                                                    \n\
";

static const char *fshader =
"                                                    \n\
#version 330                                         \n\
out vec4 color;                                      \n\
void main() {                                        \n\
	color = vec4(1.0, 0.0, 0.0, 1.0);                \n\
}                                                    \n\
";

void CreateTriangle() {
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
}

int AddShader(GLuint program, const char* code, GLenum type) {
	GLuint shader = glCreateShader(type);

	const GLchar* c[1];
	c[0] = code;

	GLint codeLen[1];
	codeLen[0] = strlen(code);

	GLint result = 0;
	GLchar glErr[1024] = { 0 };

	glShaderSource(shader, 1, c, codeLen);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(glErr), NULL, glErr);
		std::cerr << "Error compiling the " << type << " shader: " << glErr << "\n";
		return 1;
	}

	glAttachShader(program, shader);

	return 0;
}

int CompileShaders() {
	shader = glCreateProgram();

	if (!shader) {
		std::cerr << "Error creating shader program \n";
		return 1;
	}
	
	if (AddShader(shader, vshader, GL_VERTEX_SHADER) == 1) {
		return 1;
	}
	if (AddShader(shader, fshader, GL_FRAGMENT_SHADER) == 1) {
		return 1;
	}

	GLint result = 0;
	GLchar glErr[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(glErr), NULL, glErr);
		std::cerr << "Error linking program: " << glErr << "\n";
		return 1;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shader, sizeof(glErr), NULL, glErr);
		std::cerr << "Error validating program: " << glErr << "\n";
		return 1;
	}

	uniformModel = glGetUniformLocation(shader, "model");

	return 0;
}

int main() {
	// Init GLFW
	if (!glfwInit()) {
		std::cerr << "GLFW init failed!\n";
		return 1;
	}

	// Setup window properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Core profile = no backwards compat.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Allow forwards compat.
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);
	if (!mainWindow) {
		std::cerr << "GLFW window creation failed!\n";
		glfwTerminate();
		return 1;
	}

	// Get buffer size info
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context for GLFW to use
	glfwMakeContextCurrent(mainWindow);

	// Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW init failed!\n";
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Setup viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	CreateTriangle();
	if (CompileShaders() == 1) {
		std::cerr << "Compile error.\n";
		return 1;
	}

	// loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) {
		// Get and handle user input events
		glfwPollEvents();

		if (direction) {
			triOffset += triIncrement;
		}
		else {
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset) {
			direction = !direction;
		}

		curAngle += 0.01f;
		if (curAngle >= 360.0f) {
			curAngle -= 360.0f;
		}

		// clear window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f);
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f)); 

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}
