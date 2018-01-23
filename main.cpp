
#define GLEW_STATIC

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

// Including all my 
#include "stb_image.h"
#include "shader.h"
#include "texture.h"
#include "camera.h"
#include "cube.h"
#include "vars.h"
#include "atlasLoader.h"
#include "flora.h"
#include "tree.h"


double lastTime;
int nbFrames = 0;

int width = 1280;
int height = 720;

// camera
Camera camera(glm::vec3(0.5f, 20.0f, 0.0f));
double lastX = width / 2.0f;
double lastY = height / 2.0f;
bool firstMouse = true;
bool fullScreen = false;

glm::mat4 model;
glm::vec3 transform = glm::vec3(0.0f, -10.0f, 0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

GLFWwindow* window;
unsigned int VBO, VAO, EBO;
unsigned int cubesVBO, cubesVAO, cubesEBO;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput();

std::vector<Cube> cubes;
std::vector<Tree> trees;

unsigned int dirtTextureID;
unsigned int grassTextureID;

int index = 0;
int pressed = 0;
int renderDistance = 8;


// atlas loader
atlasLoader aLoader = atlasLoader();

int ranged_rand(int min, int max) {
	return min + (int)((double)(max - min) * (rand() / (RAND_MAX + 1.0)));
}

void generateCollinetta(int startPointX, int startPointY) {
	int larghezza = ranged_rand(1, 20);
	int altezza = ranged_rand(1, 8);
	int profondita = ranged_rand(1, 20);
	glm::vec3 transform;

	for (int i = 0; i < altezza; i++) {
		transform.y = -5 + i;
		for (int i2 = i; i2 < larghezza - i; i2++) {
			transform.x = startPointX + i2;
			for (int i3 = i; i3 < profondita - i; i3++) {
				transform.z = startPointY + i3;
				Cube currentCube = Cube(transform);

				currentCube.type = 0;
				currentCube.atlasID = aLoader.textureIDs[2];

				cubes.push_back(currentCube);
			}
		}
		
	}

}

void loadChunks() {
	int chunkCubes = 16;
	int ncubes = 0;
	int chunks = 0;
	int lastYcoord;
	glm::vec3 transform;
	for (int c2 = -renderDistance / 2; c2 < renderDistance / 2; c2++) {
		for (int c = -renderDistance / 2; c < renderDistance / 2; c++) {
			for (int y = -8; y < -5; y++) {
				lastYcoord = y;
				transform.y = y;
				for (int i = -8; i < 8; i++) {
					transform.z = i + (c * chunkCubes);
					for (int i2 = -8; i2 < 8; i2++) {
						transform.x = i2 + (c2 * chunkCubes);
						Cube currentCube = Cube(transform);

						currentCube.type = 0;
						currentCube.atlasID = aLoader.textureIDs[2];

						cubes.push_back(currentCube);
						ncubes++;
						// std::cout << ncubes << std::endl;
					}
				}
			}
			chunks++;
		}
	}

	for (int i = 0; i < 50; i++) {
		generateCollinetta(ranged_rand(-40, 40), ranged_rand(-40, 40));
	}
	
	

	/*lastYcoord++;
	std::cout << lastYcoord << std::endl;
	for (int r = 0; r < 100; r++) {
		transform.y = lastYcoord;
		transform.z = ranged_rand(-40, 40);
		transform.x = ranged_rand(-40, 40);

		Cube currentCube = Cube(transform);

		currentCube.type = 0;
		currentCube.atlasID = aLoader.textureIDs[ranged_rand(0, 2)];

		cubes.push_back(currentCube);
		ncubes++;
	}

	std::cout << "Last y: " << lastYcoord << std::endl;
	std::cout << "Loaded: " << ncubes << " cubes (" << chunks << ") chunks" << std::endl;
	*/
}

static const float vertices[] = {
	0.0, 0.0, 1.0, 0.0, 0.25, // front
	0.0, 1.0, 1.0, 0.0, 0.0,
	1.0, 1.0, 1.0, 0.25, 0.0,
	1.0, 0.0, 1.0, 0.25, 0.25,

	0.0, 0.0, 0.0, 0.0, 1.0, //   back
	0.0, 1.0, 0.0, 0.0, 0.75,
	1.0, 1.0, 0.0, 0.25, 0.75,
	1.0, 0.0, 0.0, 0.25, 1.0,

	0.0, 0.0, 1.0, 0.0, 0.50, // top
	0.0, 0.0, 0.0, 0.0, 0.75,
	1.0, 0.0, 0.0, 0.25, 0.75,
	1.0, 0.0, 1.0, 0.25, 0.50,

	0.0, 1.0, 1.0, 0.0, 0.25,  // bottom
	0.0, 1.0, 0.0, 0.0, 0.50,
	1.0, 1.0, 0.0, 0.25, 0.50,
	1.0, 1.0, 1.0, 0.25, 0.25,

	0.0, 1.0, 1.0, 0.25, 0.0, // left
	0.0, 0.0, 1.0, 0.25, 0.25,
	0.0, 0.0, 0.0, 0.50, 0.25,
	0.0, 1.0, 0.0, 0.50, 0.0,

	1.0, 1.0, 1.0, 0.50, 0.0, // right
	1.0, 0.0, 1.0, 0.50, 0.25,
	1.0, 0.0, 0.0, 0.75, 0.25,
	1.0, 1.0, 0.0, 0.75, 0.0
};

unsigned int indices[] = {

	0, 1, 2, // front
	2, 3, 0,

	4, 5, 6, // back
	6, 7, 4,

	8, 9, 10, // bottom
	8, 10, 11,

	12, 13, 14,
	12, 14, 15,

	16, 17, 19,
	17, 18, 19,

	20, 21, 22,
	20, 22, 23
};

int init() {
	// Initialise GLFW
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 0);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	window = glfwCreateWindow(width, height, "Scrausocraft", NULL, NULL);
	
	if (window == NULL) {
		std::cout << "Failed to open GLFW window. If you have an Intel GPU, it's not 3.3 compatible..." << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		glfwTerminate();
		return -1;
	}

	return 0;
}

void closeGame() {
	// Cleanup VBO and shader
	glDeleteBuffers(5, &VBO);
	// shader.deleteProgram();
	glDeleteVertexArrays(1, &VAO);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

int getYMax(glm::vec2 request) {
	int maxY = -5;
	for (int i = 0; i < cubes.size(); i++) {
		Cube currentCube = cubes[i];
		if (currentCube.getPosition().x == request.x && currentCube.getPosition().z == request.y) {
			if (maxY < currentCube.getPosition().y) maxY = currentCube.getPosition().y;
		}
	}
	return maxY;
}

void createTree() {
	
	int tree_height = ranged_rand(4, 8);
	int leaves_height = ranged_rand(2, tree_height - 1);
	int leaves_width = ranged_rand(1, tree_height - 2);

	// scelgo un punto in cui generare l'albero
	int growPointX = ranged_rand(-renderDistance * 7, renderDistance * 7);
	int growPointZ = ranged_rand(-renderDistance * 7, renderDistance * 7);

	int yPoint = getYMax(glm::vec2(growPointX, growPointZ));

	Tree tree = Tree(glm::vec3(growPointX, yPoint, growPointZ), tree_height, leaves_width, leaves_height);
	tree.generate();
	trees.push_back(tree);

}

int mainLoop() {
	Shader shader = Shader("VertexShader.vs", "FragmentShader.fs");
	shader.use();

	

	unsigned int lastTextureID = -1;

	for (int i = 0; i < 60; i++) {
		createTree();
	}
	

	glm::vec3 Front;

	do {

		double currentFrame = glfwGetTime();
		deltaTime = (float)currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.setMat4("projection", glm::perspective(glm::radians(camera.Zoom), (float)width / (float)height, 0.1f, 10000.0f));
		shader.setMat4("view", camera.GetViewMatrix());

		glBindBuffer(GL_ARRAY_BUFFER, cubesVAO);

		Front = camera.getFront();

		for (int i = 0; i < cubes.size(); i++) {
			Cube currentCube = cubes[i];

			shader.setMat4("model", glm::translate(model, cubes[i].getPosition()));

			if (lastTextureID != currentCube.atlasID) {
				glBindTexture(GL_TEXTURE_2D, currentCube.atlasID);
				lastTextureID = currentCube.atlasID;
			}

			glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		}

		for (int i = 0; i < trees.size(); i++) {
			Tree currentTree = trees[i];
			
			for (int l = 0; l < currentTree.logPoints.size(); l++) {
				if (lastTextureID != aLoader.textureIDs[5]) {
					glBindTexture(GL_TEXTURE_2D, aLoader.textureIDs[5]);
					lastTextureID = aLoader.textureIDs[5];
				}

				shader.setMat4("model", glm::translate(model, currentTree.logPoints[l]));
				glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
			}

			for (int l = 0; l < currentTree.leavesPoints.size(); l++) {
				if (lastTextureID != aLoader.textureIDs[6]) {
					glBindTexture(GL_TEXTURE_2D, aLoader.textureIDs[6]);
					lastTextureID = aLoader.textureIDs[6];
				}
				shader.setMat4("model", glm::translate(model, currentTree.leavesPoints[l]));
				glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
			}

		}

		// Measure speed
		nbFrames++;
		if (glfwGetTime() - lastTime >= 1.0) {
			char integer_string[32];

			sprintf(integer_string, "%d", nbFrames);

			char boh[25] = "Scrauso craft - FPS: ";
			strcat(boh, integer_string);

			glfwSetWindowTitle(window, boh);
			nbFrames = 0;
			lastTime += 1.0;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		// camera.applyGravity(deltaTime, cubes);
	} while (!glfwWindowShouldClose(window));

	closeGame();

	return -1;
}

int main(void) {
	if (init() != 0) return -1;

	/* initialize random seed: */
	srand(time(NULL));

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glGenVertexArrays(1, &cubesVAO);
	glGenBuffers(1, &cubesVBO);
	glGenBuffers(1, &cubesEBO);


	// Fill the buffer data
	glBindVertexArray(cubesVAO);

	glBindBuffer(GL_ARRAY_BUFFER, cubesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubesEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	lastTime = glfwGetTime();

	// Light blue background
	glClearColor(0.5294f, 0.8078f, 0.98f, 1.0f);

	aLoader.loadAll();
	loadChunks();

	return mainLoop();
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput() {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return;
	}

	bool shiftPressed = false;

	if (glfwGetKey(window, GLFW_KEY_P)) {
		std::cout << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		if (pressed != 1) {
			glm::vec3 Front = camera.getFront();
			// std::cout << Front.x << " " << Front.y << " " << Front.z << std::endl;
			pressed = 1;
		}
	} else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE) {
		if (pressed == 1) {
			index++;
			std::cout << index << std::endl;
			pressed = 0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) shiftPressed = true;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime, shiftPressed=shiftPressed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime, shiftPressed = shiftPressed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime, shiftPressed = shiftPressed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime, shiftPressed = shiftPressed);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int nwidth, int nheight) {
	width = nwidth;
	height = nheight;
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse){
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

