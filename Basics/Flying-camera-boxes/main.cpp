/* Standard libraries */
#include <iostream>

/* OpenGL graphics libraries*/
#include "glad/glad.h"
#include "GLFW/glfw3.h"

/* Math libraries */
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

/* Image library */
#include "stb_image.h"

/* Own library */
#include "Shader.h"


void changeOnFrameBufferResize(GLFWwindow* window, int width, int height);
void proccesInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset);

const int winWidth = 800;
const int winHeight = 600;


/*************************************** CAMERA ***************************************/
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

/*************************************** FRAMES ***************************************/
float deltaTime = 0.0f; /* Time between current and last frame */
float lastTime = 0.0f; /* Time of last frame */

/*************************************** MOUSE ***************************************/
float lastX = winWidth / 2;
float lastY = winHeight / 2;
float yaw = -90.0f;
float pitch = 0.0f;
bool firstMove = true;

float fov = 45.0f;


int main()
{
	/*************************************** INITIALIZATION ***************************************/
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Cubes", NULL, NULL);
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
	glfwSetFramebufferSizeCallback(window, changeOnFrameBufferResize);

	/* Disable the cursor (To give it a effect like in a FPS game) */
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	/* Call the mouse_callback function, everytime the cursor moves */
	glfwSetCursorPosCallback(window, mouse_callback);
	/* Call the scroll_callback function, everytime the user scrolls */
	glfwSetScrollCallback(window, scroll_callback);

	/*************************************** POSITIONS ***************************************/
	float cubeVertices[] = {
		/*		Vertices	Texture Positions*/
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

		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f
	};

	glm::vec3 cubePositions[10] = {
		glm::vec3(0.0f,   0.0f,   0.0f),
		glm::vec3(2.0f,   5.0f, -15.0f),
		glm::vec3(-1.5f,  -2.2f,  -2.5f),
		glm::vec3(-3.8f,  -2.0f, -12.3f),
		glm::vec3(2.4f,  -0.4f,  -3.5f),
		glm::vec3(-1.7f,   3.0f,  -7.5f),
		glm::vec3(1.3f,  -2.0f,  -2.5f),
		glm::vec3(1.5f,   2.0f,  -2.5f),
		glm::vec3(1.5f,   0.2f,  -1.5f),
		glm::vec3(-1.3f,   1.0f,  -1.5f)
	};

	/*************************************** SHADERS ***************************************/
	Shader shader("shaders/shader.vs", "shaders/shader.fs");
	shader.use();

	/*************************************** BUFFERS ***************************************/
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*************************************** TEXTURES ***************************************/
	/* texture1 */
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int tex_width, tex_height, tex_nrChannels;
	unsigned char* tex_data = stbi_load("textures/container.jpg", &tex_width, &tex_height, &tex_nrChannels, 0);
	if (tex_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "texture1 couldn't be loaded" << std::endl;
	stbi_image_free(tex_data);

	/* texture2 */
	unsigned int texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	tex_data = stbi_load("textures/awesomeface.png", &tex_width, &tex_height, &tex_nrChannels, 0);
	if (tex_data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_width, tex_height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, tex_data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "texture2 couldn't be loaded" << std::endl;
	stbi_image_free(tex_data);

	shader.use();
	shader.setInt("texture1", 0);
	shader.setInt("texture2", 1);

	/*************************************** MATRICES ***************************************/
	glm::mat4 project = glm::mat4(1.0f);

	glEnable(GL_DEPTH_TEST);

	/*************************************** RENDER LOOP ***************************************/
	while (!glfwWindowShouldClose(window)) {
		proccesInput(window);

		glClearColor(0.2f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		shader.use();
		glBindVertexArray(VAO);

		for (int i = 0; i < 10; i++) {
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);

			float angle = 20.0f * i;
			if (i % 3 == 0)
				model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
			else
				model = glm::rotate(model, angle * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
			shader.setMat4f("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		/* Set the angle to the fov */
		project = glm::perspective(glm::radians(fov), (float)winWidth / winHeight, 0.1f, 100.0f);

		shader.setMat4f("view", view);
		shader.setMat4f("projection", project);

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastTime;
		lastTime = currentFrame;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
}


void changeOnFrameBufferResize(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void proccesInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	/* Camera input */
	/* The camera speed will be constant, even if the device calculates frames more quickly */
	const float camSpeed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += camSpeed * cameraFront; /* Moves forward */
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= camSpeed * cameraFront; /* Moves backward */
	/* Moves left by calculating the right vector, substracting it */
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
	/* Moves right by calculating the right vector, adding it */
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camSpeed;
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos)
{
	/* Avoid the "camera jump" when first moving the mouse */
	if (firstMove) {
		lastX = xPos;
		lastY = yPos;
		firstMove = false;
	}

	/* Create offset, update the last movment */
	float xOffset = xPos - lastX;
	float yOffset = lastY - yPos;
	lastX = xPos;
	lastY = yPos;

	/* Multiply the move offset by sensitivity */
	const float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;
	
	/* Update the yaw(x-axis) and pitch(y-axis) camera angles */
	yaw += xOffset;
	pitch += yOffset;
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	/* Setting the camera direction */
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	fov -= (float)yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}