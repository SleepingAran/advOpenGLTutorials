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
}