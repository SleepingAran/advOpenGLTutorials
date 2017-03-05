#include <iostream>

//GLEW
#define GLEW_STATIC
#include <opengl\include\GL\glew.h>

//GLFW
#include <opengl\include\GLFW\glfw3.h>

//GLM for transformation
//add this
#include <opengl\glm\glm.hpp>
#include <opengl\glm\gtc\matrix_transform.hpp>
#include <opengl\glm\gtc\type_ptr.hpp>

//Include Shader header file
#include "Shader.h"
const GLint WIDTH = 800, HEIGHT = 600;

void main()
{
	//Initialization Start
	//Init GLFW
	glfwInit();

	//Setup GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Transformation", nullptr, nullptr);

	//For higher resolution screen
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//If failed to create window
	if (window == nullptr)
	{
		std::cout << "Failed to create Windows";
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glViewport(0, 0, screenWidth, screenHeight);
	//Initialization End
	///////////////////////////////////////////////

	Shader ourShader("core.txt","corefrag.txt");
	GLfloat vertices[] =
	{
		//Position			//Color
		-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f, //bottom left
		0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,//bottom right
		0.0f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,//middle top
	};

	//Setup pointer array and buffer
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind the Vertex Array object first, then bind and set vertex buffer and attribute pointer
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0); //Color attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		//clear screen
		glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Call shader
		ourShader.Use();

		//transformation
		//add this
		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3((GLfloat)abs(sin(glfwGetTime())*0.5f), (GLfloat)abs(cos(glfwGetTime())*0.5f), 0.0f)); //translate
		//transform = glm::rotate(transform, (GLfloat)glfwGetTime()*2.0f, glm::vec3(1.0f,0.0f,0.0f)); //rotate
		//transform = glm::scale(transform, glm::vec3((GLfloat)sin(glfwGetTime())*1.0f, (GLfloat)sin(glfwGetTime())*1.0f, (GLfloat)sin(glfwGetTime())*1.0f)); //scale
		GLint transformLocation = glGetUniformLocation(ourShader.Program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));
		
		//drawing
		
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

}
