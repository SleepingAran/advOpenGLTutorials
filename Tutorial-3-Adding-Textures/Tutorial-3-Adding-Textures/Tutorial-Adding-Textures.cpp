#include <iostream>
using namespace std;

//Include GLEW
#define GLEW_STATIC
#include <opengl\include\GL\glew.h>

//Include GLFW
#include <opengl\include\GLFW\glfw3.h>

//Include SOIL
#include<SOIL.h>

//Function Prototype
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLchar* vertexShaderSource = "#version 330 core\n"
"layout (location =0 ) in vec3 position;\n"
"layout (location =1 ) in vec3 color;\n"
"layout (location =2 ) in vec2 texCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position= vec4(position.x, position.y, position.z, 1.0f);\n"
"ourColor = color;\n"
"TexCoord = vec2(texCoord.x, 1.0-texCoord.y);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 330 core\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"out vec4 color;\n"
"uniform sampler2D ourTexture;\n"
"uniform sampler2D ourTexture1;\n"
"void main()\n"
"{\n"
//"color = texture(ourTexture, TexCoord);\n"
"color = mix(texture(ourTexture, TexCoord), texture(ourTexture1,TexCoord),0.2) * vec4(ourColor,1.0f);\n"
"}\n\0";

void main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//create a GLFWWindow object
	GLFWwindow* window = glfwCreateWindow(800, 600, "Tutorial Hello Triangle", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	//set callback function
	glfwSetKeyCallback(window, key_callback);

	//Set TRUE for glew retreving function and pointers
	glewExperimental = GL_TRUE;

	//Initate GLEW
	glewInit();

	//create viewport
	glViewport(0, 0, 800, 600);

	//Enable alpha support for PNG files
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//Vertex Shadow
	//Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Check for compilation error
	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	}
	//Vertex Shadow end

	//Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Check for compilation error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	}
	//Fragment shader end

	//Link shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//Chech for linking error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Set each vertices of the triangle
	GLfloat vertices[] =
	{
		//Position			// Colour			//Texture Coord
		0.5f, 0.5f, 0.0f,	1.0f, 1.0f, 0.0f,	1.0f, 1.0f, //Top Right
		0.5f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f,	1.0f, 0.0f, //Bottom Right
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f, //Bottom Left
		-0.5f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,	0.0f, 1.0f, //Top Left
	};

	// Indexing - share indices
	GLuint indices[] =
	{
		0,1,3,
		1,2,3,
	};

	//Setup pointer array and buffer
	//Element Buffer Object for indexing
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//Bind the Vertex Array object first, then 
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Bind Element Array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//bind and set vertex buffer and attribute pointer
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Texture Coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glDisableVertexAttribArray(2);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Generate Texture
	GLuint texture1;
	int width1, height1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	//Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load texture
	unsigned char* image = SOIL_load_image("testing.jpg", &width1, &height1, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	//generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); //unbind texture

	
	///
	//Generate Texture
	GLuint texture2;
	int width2, height2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	//Texture Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//Texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//load texture
	unsigned char* image1 = SOIL_load_image("testing.png", &width2, &height2, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1);

	//generating mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 1); //unbind texture

	while (!glfwWindowShouldClose(window))
	{
		//Check if any event activated
		glfwPollEvents();

		//Render
		//Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//draw 2 triangle
		glUseProgram(shaderProgram);

		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture"), 0);

		glActiveTexture(GL_TEXTURE);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(shaderProgram, "ourTexture1"), 1);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		//swap the screen buffers
		glfwSwapBuffers(window);
	}
	
	//Clean up after use
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}