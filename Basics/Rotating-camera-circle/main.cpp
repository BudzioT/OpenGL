#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>

#include "Shader.h"

void fixFrameBufferResize(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


const int winWidth = 800;
const int winHeight = 600;


int main()
{
	/********************************** Initialization ***********************************/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Colorful triangle", NULL, NULL);
	if (window == NULL) {
		std::cout << "ERROR::GLFW::Initialization_failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "ERROR::GLAD::Initialization_failed" << std::endl;
		glfwTerminate();
		return -1;
	}
	glViewport(0, 0, winWidth, winHeight);
	glfwSetFramebufferSizeCallback(window, fixFrameBufferResize);

	Shader shader("shaders/shader.vs", "shaders/shader.fs");

	/********************************** Buffers ***********************************/
	/* Vertices of the cube */
	float vertices[] = {
		/*		Positions	Texture coords*/
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f
	};

	/* Positions of the cubes in the world space */
	glm::vec3 cubePositions[] = {
		glm::vec3( 0.0f,   0.0f,   0.0f),
		glm::vec3( 2.0f,   5.0f, -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f, -12.3f),
		glm::vec3( 2.4f,  -0.4f,  -3.5f),
		glm::vec3(-1.7f,   3.0f,  -7.5f),
		glm::vec3( 1.3f,  -2.0f,  -2.5f),
		glm::vec3( 1.5f,   2.0f,  -2.5f),
		glm::vec3( 1.5f,   0.2f,  -1.5f),
		glm::vec3(-1.3f,   1.0f,  -1.5f)
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/* Position attrib */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	/* Color attrib */
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/* Texture position attrib */

	/********************************** Texture ***********************************/

	/* First texture */
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

	stbi_set_flip_vertically_on_load(true);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load the texture" << std::endl;
	stbi_image_free(data);

	/* Second texture */
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load the texture" << std::endl;
	stbi_image_free(data);

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	/******************************** 3D matrixes ***********************************/
	/* View matrix */
	glm::mat4 view;

	/* Projection matrix */
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

	/* Gets the location of the uniforms */
	int modelLoc = glGetUniformLocation(shader.ID, "model");
	int viewLoc = glGetUniformLocation(shader.ID, "view");
	int projectionLoc = glGetUniformLocation(shader.ID, "projection");

	glEnable(GL_DEPTH_TEST);

	/******************************** Program loop ***********************************/
	const float radius = 10.0f;
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.use();

		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			if (i % 3 == 0)
				angle = 25.0f * glfwGetTime();
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		/* Rotate the x and z axes based on the time */
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;

		/* Change the view matrix, lookAt takes position, target, up vector as arguments */
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	glfwTerminate();
}


void fixFrameBufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}