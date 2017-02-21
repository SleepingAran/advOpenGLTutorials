#include<iostream>
#define GLEW_STATIC
#include <opengl\include\GL\glew.h>
#include <opengl\include\GLFW\glfw3.h>
#include <SOIL.h>

using namespace std;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main()
{
	//Initialization starts
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//initialize GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial 3", nullptr, nullptr);
	if (window == nullptr)
	{
		//if the window is not created, exit with error
		cout << "Failed to create GLFW Window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//Initialization ends

	//set callback function
	glfwSetKeyCallback(window, key_callback);

	//create viewport
	glViewport(0, 0, 800, 600);

	//SOIL Starts
	int width, height;
	unsigned char* image = SOIL_load_image("testing.png", &width, &height, 0, SOIL_LOAD_RGB);

	//SOIL Ends

	//Draw triangle start
	//Set each vertices of the triangle
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	//Setup pointer array and buffer
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind the Vertex Array object first, then bind and set vertex buffer and attribute pointer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//Draw triangle ends

	//prevent app from self quit
	while (!glfwWindowShouldClose(window))
	{
		//Check and call events
		glfwPollEvents();

		//Rendering commands
		glClearColor(0.0f, 0.0f, .6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}