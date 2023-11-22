#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "Camera.h"

#include <iostream>

/************************************** WINDOW VARIABLES **************************************/
const int winWidth = 800;
const int winHeight = 600;

/************************************** CAMERA **************************************/
Camera camera({ 0.0f, 0.0f, 3.0f });
float lastX = winWidth / 2;
float lastY = winHeight / 2;
bool firstMouseMove = true;

/************************************** FRAMES **************************************/
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/************************************** LIGHT **************************************/
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

/************************************** PROCESS FUNCTIONS **************************************/
void processInput(GLFWwindow* window);

/************************************** CALLBACK FUNCTIONS **************************************/
void frameBufferResize_callback(GLFWwindow* window, int width, int height);
void mouseMovement_callback(GLFWwindow* window, double xPos, double yPos);
void mouseScroll_callback(GLFWwindow* window, double xOffset, double yOffset);


int main()
{
	/************************************** INITIALIZATION **************************************/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Light", NULL, NULL);
	if (window == NULL) {
		std::cout << "ERROR::GLFW::Window_initialization_failed" << std::endl;
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

	glfwSetFramebufferSizeCallback(window, frameBufferResize_callback);
	glfwSetCursorPosCallback(window, mouseMovement_callback);
	glfwSetScrollCallback(window, mouseScroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	/************************************** POSITIONS **************************************/
	float cubeVertices[] = {
		/*		Positions		Normals */
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

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

		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f
	};

	/************************************** BUFFERS **************************************/
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/************************************** SHADERS **************************************/
	Shader objectShader("shaders/object.vs", "shaders/object.fs");
	Shader lightShader("shaders/lightSource.vs", "shaders/lightSource.fs");

	/************************************** RENDER LOOP **************************************/
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* Move the light */
		lightPos.x = sin(glfwGetTime()) * 2.0f;
		lightPos.y = sin(glfwGetTime() / 2.0f) * 1.0f;

		/* Transformations */
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection =
			glm::perspective(glm::radians(camera.Zoom), (float)winWidth / winHeight, 0.1f, 100.0f);

		/* Change the lighting color */
		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);
		glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);

		/* Object uniforms */
		objectShader.use();
		objectShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		objectShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		objectShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		objectShader.setFloat("material.shininess", 32.0f);
		objectShader.setVec3("light.ambient", ambientColor);
		objectShader.setVec3("light.diffuse", diffuseColor);
		objectShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		objectShader.setVec3("light.position", lightPos);
		objectShader.setVec3("viewPos", camera.Position);
		glm::mat4 model = glm::mat4(1.0f);
		objectShader.setMat4("model", model);
		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);
		/* Render the object */
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/* Light uniforms */
		lightShader.use();
		lightShader.setVec3("lightColor", lightColor);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);
		/* Render the light */
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		/* Frames calculation */
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/************************************** CLEANUP **************************************/
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	/* Movement inputs */
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
		deltaTime *= 2;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_movement::RIGHT, deltaTime);

}

void frameBufferResize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseMovement_callback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouseMove) {
		lastX = xPos;
		lastY = yPos;
		firstMouseMove = false;
	}

	float xOffset = (float)(xPos - lastX);
	float yOffset = (float)(lastY - yPos);
	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

void mouseScroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}