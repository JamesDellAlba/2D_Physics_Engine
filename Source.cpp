#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//shader source code in GLSL
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

//fragment shader source code in GLSL
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n";

class Triangle {
public:
	float vertices[9] = {};
	unsigned int shaderProgram;
	unsigned int VAO;

	Triangle(float x1, float y1, float z1,	 //left
		float x2, float y2, float z2,	 //right
		float x3, float y3, float z3) { //top
		vertices[0] = x1;
		vertices[1] = y1;
		vertices[2] = z1;
		vertices[3] = x2;
		vertices[4] = y2;
		vertices[5] = z2;
		vertices[6] = x3;
		vertices[7] = y3;
		vertices[8] = z3;
	}

	void prepareToDraw() {
		unsigned int VBO;
		glGenBuffers(1, &VBO); //generate a buffer with an ID

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
		//unsigned int shaderProgram;
		shaderProgram = glCreateProgram();
		//link shaders to this shader program
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glUseProgram(shaderProgram);
		//delete shader objects (now unused)
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		//generate VAO (vertex array object)
		//unsigned int VAO;
		glGenVertexArrays(1, &VAO);

		// Initialization code (done once (unless your object frequently changes))
		// 1. bind Vertex Array Object
		glBindVertexArray(VAO);
		// 2. copy our vertices array in a buffer for OpenGL to use
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), this->vertices, GL_STATIC_DRAW);
		// 3. then set our vertex attributes pointers
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}
};

int main() {
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, true);

	int horizontalSize = 1920;
	int verticalSize = 1440;

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
	Triangle triangle1(-0.25, 0.0, 0.0,
						0.25, 0.0, 0.0,
						0.0, 0.5, 0.0);
	triangle1.prepareToDraw();
	Triangle triangle2(-0.5, -0.5, 0.0,
		0.0, -0.5, 0.0,
		-0.25, 0.0, 0.0);
	triangle2.prepareToDraw();

	
	


	//render loop
	int i = 0;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
		cout << "rendering frame " << i << endl;
		i++;
		// ..:: Drawing code (in render loop)
		// 4. draw the object
		glUseProgram(triangle1.shaderProgram);
		glBindVertexArray(triangle1.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//draw second triangle
		glUseProgram(triangle2.shaderProgram);
		glBindVertexArray(triangle2.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

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

