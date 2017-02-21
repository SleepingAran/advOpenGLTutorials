#include<iostream>

//include GLEW
#define GLEW_STATIC
#include<GL\glew.h>

//include GLFW
#include<GLFW\glfw3.h>
using namespace std;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
int main()
{
	//initialize environment start
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//initialize environment end

	//initialize GLFW window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr) 
	{
		//if the window is not created, exit with error
		cout << "Failed to create GLFW Window" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //call the "window" created

	if (glewInit() != GLEW_OK) 
	{
		//if failed to initialize, exit with error
		cout << "Failed to initialize GLEW" << endl;
		return -1;
	}

	//Access the clalback function
	glfwSetKeyCallback(window, key_callback);

	glViewport(0, 0, 800, 600);

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