#include <iostream>
using namespace std;
//GLEW
#define GLEW_STATIC
#include <opengl\include\GL\glew.h>

//GLFW
#include <opengl\include\GLFW\glfw3.h>

//SOIL
#include <SOIL.h>
//GLM for transformation
//add this
#include <opengl\glm\glm.hpp>
#include <opengl\glm\gtc\matrix_transform.hpp>
#include <opengl\glm\gtc\type_ptr.hpp>

//Include Shader header file
#include "Shader.h"
#include "camera.h"
const GLint WIDTH = 800, HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
//void mouse_callback(GLFWwindow* window, double Xcurrentpos, double Ycurrentpos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void do_movement();
//limit framerate on all hardware
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
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

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Coordinate System", nullptr, nullptr);
	glfwSetKeyCallback(window, key_callback);

	//set mousecallback
	//glfwSetCursorPosCallback(window, mouse_callback);
	//hide pointer 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//set scrollcallback
	//glfwSetScrollCallback(window, scroll_callback);
	
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
	glEnable(GL_DEPTH_TEST);
	//Initialization End
	///////////////////////////////////////////////

	Shader ourShader("lighting.vs","lighting.frag");
	Shader lampShader("lamp.vs", "lamp.frag");
	/*GLfloat vertices[] =
	{
		//Position			//Texture
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f,  //bottom left
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, //bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 0.0f, //middle top
		-0.5f, 0.5f, 0.0f,  0.0f, 1.0f, //Top left

	};

	GLuint indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};
	*/
	GLfloat vertices[] = {
		-0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f, 
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f,  

		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f,  
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f,  

		-0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f, -0.5f, 
	};
	
	//Setup pointer array and buffer
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind the Vertex Array object first, then bind and set vertex buffer and attribute pointer
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0); //Color attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	///
	
	//Generate Texture 1
	GLuint texture1;
	int width1, height1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	//Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//load texture
	unsigned char* image = SOIL_load_image("bear.png", &width1, &height1, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	//generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture
	
	///
	//Generate Texture 2
	GLuint texture2;
	int width2, height2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//load texture
	unsigned char* image1 = SOIL_load_image("bear.png", &width2, &height2, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);

	//generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 1); //unbind texture
	///
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		//clear screen
		glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Bind texture using texture units
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture1"), 0);
		
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(ourShader.Program, "ourTexture2"), 0);
		//Call shader
		ourShader.Use();
		/*
		do_movement();
		
		//Create transformation
		glm::mat4 model, view, projection;
		model = glm::rotate(model, 1.0f*((GLfloat)(sin(glfwGetTime()))), glm::vec3(0.5f, 1.0f, 0.0f));
		//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
		//projection = glm::perspective(fov, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		
		//look at
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//mouse movement
		glm::vec3 front;
		front.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);

		//Get uniform location
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		//Pass trasnformation to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		*/
		//draw container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}
/*
void do_movement()
{
	GLfloat cameraSpeed = 1.0f * deltaTime;
	if (keys[GLFW_KEY_W])
		cameraPos += cameraSpeed*cameraFront;
	if (keys[ GLFW_KEY_S])
		cameraPos -= cameraSpeed*cameraFront;
	if (keys [GLFW_KEY_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;
	if (keys[GLFW_KEY_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp))*cameraSpeed;

	
}

void mouse_callback(GLFWwindow* window, double Xcurrentpos, double Ycurrentpos)
{
	GLfloat xoffset = Xcurrentpos - lastX;
	GLfloat yoffset = lastY - Ycurrentpos;
	lastX = Xcurrentpos;
	lastY = Ycurrentpos;

	GLfloat sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;
	
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

void scroll_callback(GLFWwindow* window, double xoffeset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset*0.1f;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}
*/