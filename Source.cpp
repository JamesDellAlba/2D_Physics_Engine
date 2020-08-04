#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//shader source code in GLSL
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"out vec4 vertexColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}\0";

//fragment shader source code in GLSL
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"	FragColor = ourColor;\n"
"}\n\0";

int main() {
	
	//initialize and configure glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	int horizontalSize = 1080;
	int verticalSize = 720;

	//create window object
	GLFWwindow* window = glfwCreateWindow(horizontalSize, verticalSize, "Test Window", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to create a window";
		return -1;
	}
	//create context and set frame buffer size
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//check if glad is initialized
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//set viewport
	glViewport(0, 0, horizontalSize, verticalSize);

	//set verticies of the triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	//create a shader object (referenced by an ID)
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//compile the shader source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	//check for compile time errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) //print compile time errors
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//compile fragment shader source code
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	//check for compile time errors
	int fragmentShaderSuccess;
	char infoLogFragment[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderSuccess);
	if (!fragmentShaderSuccess) //print compile time errors
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLogFragment);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFragment << std::endl;
	}

	//create shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	//link shaders to this shader program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	//delete shader objects (now unused)
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//generate VAO (vertex array object) and VBO (vertex buffer object)
	unsigned int VAO;
	unsigned int VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	glBindVertexArray(VAO);
	// 2. copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3. then set our vertex attributes pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//render loop
	int i = 0;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		
		cout << "rendering frame " << i << endl;
		i++; //count each frame

		// --- Drawing code (in render loop) ---
		glUseProgram(shaderProgram);
		//shift colors
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//swap buffers and poll I/O events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//delete resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate(); //terminate and clear glfw resources
	return 0;
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions
	glViewport(0, 0, width, height);
}