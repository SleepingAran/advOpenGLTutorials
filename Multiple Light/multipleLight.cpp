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

void mouse_callback(GLFWwindow* window, double Xcurrentpos, double Ycurrentpos);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
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
	glfwSetCursorPosCallback(window, mouse_callback);
	//hide pointer 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//set scrollcallback
	glfwSetScrollCallback(window, scroll_callback);

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

	Shader ourShader("multilight.vs", "multilight.frag");
	Shader lampShader("lamp.vs", "lamp.frag");
	
	GLfloat vertices[] = {
		// Positions           // Normals        
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,  

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f, 
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f, 
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	};

	//Random cube boxes
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f,   -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f,  -12.3f),
		glm::vec3(2.4f,   -0.4f,  -3.5f),
		glm::vec3(-1.7f,  3.0f,   -7.5f),
		glm::vec3(1.3f,   -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,   -2.5f),
		glm::vec3(1.5f,   0.2f,   -1.5f),
		glm::vec3(-1.3f,  1.0f,   -1.5f)
	};
	
	//Random light position
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,0.2f,2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,2.0f,-12.0f),
		glm::vec3(0.0f,0.0f,-3.0f),
	};

	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	
		while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();
		//clear screen
		glClearColor(0.3f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Call shader
		do_movement();
		ourShader.Use();

		
		GLint viewPosLoc = glGetUniformLocation(ourShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);

		glm::vec3 lightColor;
		lightColor.r = 0.5f*sin(glfwGetTime());
		lightColor.g = 1.0f*cos(glfwGetTime());
		lightColor.b = 1.0f;
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // Decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // Low influence

		 // Set material properties
		glUniform3f(glGetUniformLocation(ourShader.Program, "material.ambient"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(ourShader.Program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
		glUniform3f(glGetUniformLocation(ourShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
		glUniform1f(glGetUniformLocation(ourShader.Program, "material.shininess"), 3200.0f);

		//add directional light location
		GLint lightDirLoc = glGetUniformLocation(ourShader.Program, "dirLight.direction");
		glUniform3f(lightDirLoc, lightPos.x, lightPos.y, lightPos.z);
		GLint lightAmbientLoc = glGetUniformLocation(ourShader.Program, "dirLight.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(ourShader.Program, "dirLight.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(ourShader.Program, "dirLight.specular");
		glUniform3f(lightAmbientLoc, ambientColor.r, ambientColor.g, ambientColor.b);
		glUniform3f(lightDiffuseLoc, diffuseColor.r, diffuseColor.g, diffuseColor.b);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);
		
		//Point light number 1
		GLint lightPosLoc = glGetUniformLocation(ourShader.Program, "pointLights[0].position");
		glUniform3f(lightPosLoc, pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		lightAmbientLoc = glGetUniformLocation(ourShader.Program, "pointLights[0].ambient");
		lightDiffuseLoc = glGetUniformLocation(ourShader.Program, "pointLights[0].diffuse");
		lightSpecularLoc = glGetUniformLocation(ourShader.Program, "pointLights[0].specular");
		glUniform3f(lightAmbientLoc, ambientColor.r, ambientColor.g, ambientColor.b);
		glUniform3f(lightDiffuseLoc, diffuseColor.r, diffuseColor.g, diffuseColor.b);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[0].quadratic"), 0.017f);

		//point light 2
		lightPosLoc = glGetUniformLocation(ourShader.Program, "pointLights[1].position");
		glUniform3f(lightPosLoc, pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		lightAmbientLoc = glGetUniformLocation(ourShader.Program, "pointLights[1].ambient");
		lightDiffuseLoc = glGetUniformLocation(ourShader.Program, "pointLights[1].diffuse");
		lightSpecularLoc = glGetUniformLocation(ourShader.Program, "pointLights[1].specular");
		glUniform3f(lightAmbientLoc, ambientColor.r, ambientColor.g, ambientColor.b);
		glUniform3f(lightDiffuseLoc, diffuseColor.r, diffuseColor.g, diffuseColor.b);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[1].quadratic"), 0.017f);

		//Point light 3
		lightPosLoc = glGetUniformLocation(ourShader.Program, "pointLights[2].position");
		glUniform3f(lightPosLoc, pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		lightAmbientLoc = glGetUniformLocation(ourShader.Program, "pointLights[2].ambient");
		lightDiffuseLoc = glGetUniformLocation(ourShader.Program, "pointLights[2].diffuse");
		lightSpecularLoc = glGetUniformLocation(ourShader.Program, "pointLights[2].specular");
		glUniform3f(lightAmbientLoc, ambientColor.r, ambientColor.g, ambientColor.b);
		glUniform3f(lightDiffuseLoc, diffuseColor.r, diffuseColor.g, diffuseColor.b);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[2].quadratic"), 0.017f);

		//Point light 4
		lightPosLoc = glGetUniformLocation(ourShader.Program, "pointLights[3].position");
		glUniform3f(lightPosLoc, pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		lightAmbientLoc = glGetUniformLocation(ourShader.Program, "pointLights[3].ambient");
		lightDiffuseLoc = glGetUniformLocation(ourShader.Program, "pointLights[3].diffuse");
		lightSpecularLoc = glGetUniformLocation(ourShader.Program, "pointLights[3].specular");
		glUniform3f(lightAmbientLoc, ambientColor.r, ambientColor.g, ambientColor.b);
		glUniform3f(lightDiffuseLoc, diffuseColor.r, diffuseColor.g, diffuseColor.b);
		glUniform3f(lightSpecularLoc, 1.0f, 1.0f, 1.0f);

		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].linear"), 0.07f);
		glUniform1f(glGetUniformLocation(ourShader.Program, "pointLights[3].quadratic"), 0.017f);

		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 model;
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		
		glBindVertexArray(containerVAO);
		//Draw multiple box
		for (GLuint i = 0; i < 10; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0 ,36);
		}
		glBindVertexArray(0);

		lampShader.Use();
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(lightVAO);
		//Draw mutliple lamp
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4();
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); //make the lamp smaller
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		glBindVertexArray(0);
		glfwSwapBuffers(window);

	}

	//glDeleteVertexArrays(1, &containerVAO);
	//glDeleteBuffers(1, &VBO);
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

void do_movement()
{
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);


}

void mouse_callback(GLFWwindow* window, double Xcurrentpos, double Ycurrentpos)
{
	GLfloat xoffset = Xcurrentpos - lastX;
	GLfloat yoffset = lastY - Ycurrentpos;
	lastX = Xcurrentpos;
	lastY = Ycurrentpos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffeset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
