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
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	int width, height;
	unsigned char* image = SOIL_load_image("testing.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	//SOIL Ends

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