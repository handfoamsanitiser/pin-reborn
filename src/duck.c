#include "duck.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "linmath.h"
#include "stb_image.h"
#include "utils.h"

GLuint basicRectVAO;
GLuint texRectVAO;

GLuint basicRectVBO;
GLfloat basicRectVertices[] = {
	-1.0f,  1.0f,  0.0f,
	 1.0f,  1.0f,  0.0f,
	 1.0f, -1.0f,  0.0f,
	-1.0f, -1.0f,  0.0f
};

GLuint texRectVBO;
GLfloat texRectVertices[] = {
	-1.0f,  1.0f,  0.0f,	0.0f, 1.0f,
	 1.0f,  1.0f,  0.0f,	1.0f, 1.0f,
	 1.0f, -1.0f,  0.0f,	1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f,	0.0f, 0.0f
};

GLuint rectEBO;
GLuint rectIndeces[] = {
	0, 1, 2,
	0, 2, 3
};

GLuint colourShaderProgram;
GLuint texShaderProgram;

GLFWwindow *window;

const float TARGET_FPS = 60.0;

int duck_init(void) {

	setbuf(stdout, NULL);

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	printf("Screen width: %i", mode->width);
	printf("Screen height: %i", mode->height);
	printf("Screen refresh rate: %i", mode->refreshRate);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD");
		return -1;
	}
	
	glViewport(0, 0, 800, 600);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	stbi_set_flip_vertically_on_load(true);


	glGenVertexArrays(1, &basicRectVAO);
	glGenVertexArrays(1, &texRectVAO);

	glGenBuffers(1, &basicRectVBO);
	glGenBuffers(1, &texRectVBO);

	glGenBuffers(1, &rectEBO);


	glBindVertexArray(basicRectVAO);

	glBindBuffer(GL_ARRAY_BUFFER, basicRectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(basicRectVertices), basicRectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rectIndeces), rectIndeces, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	glBindVertexArray(texRectVAO);

	glBindBuffer(GL_ARRAY_BUFFER, texRectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texRectVertices), texRectVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectEBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);  


	const char *regVertSource = ReadFileToString("shaders/no_texture.vert");
	const char *texVertSource = ReadFileToString("shaders/texture.vert");
	const char *colourFragSource = ReadFileToString("shaders/no_texture.frag");
	const char *texFragSource = ReadFileToString("shaders/texture.frag");
	
	GLuint regVertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(regVertShader, 1, &regVertSource, NULL);
	glCompileShader(regVertShader);

	GLuint texVertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(texVertShader, 1, &texVertSource, NULL);
	glCompileShader(texVertShader);
	
	GLuint colourFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(colourFragShader, 1, &colourFragSource, NULL);
	glCompileShader(colourFragShader);

	GLuint texFragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(texFragShader, 1, &texFragSource, NULL);
	glCompileShader(texFragShader);
	
	colourShaderProgram = glCreateProgram();
	glAttachShader(colourShaderProgram, regVertShader);
	glAttachShader(colourShaderProgram, colourFragShader);
	glLinkProgram(colourShaderProgram);

	texShaderProgram = glCreateProgram();
	glAttachShader(texShaderProgram, texVertShader);
	glAttachShader(texShaderProgram, texFragShader);
	glLinkProgram(texShaderProgram);
	
	glDeleteShader(regVertShader);
	glDeleteShader(texVertShader);
	glDeleteShader(colourFragShader);
	glDeleteShader(texFragShader);

	free((char*)regVertSource);
	free((char*)texVertSource);
	free((char*)colourFragSource);
	free((char*)texFragSource);
}

void duck_main_loop(void (*main_loop)(void)) {
	clock_t start, end;

	while(!glfwWindowShouldClose(window)) {
		start = clock();

		main_loop();
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		end = clock();

		float diff = (float)(end - start) / CLOCKS_PER_SEC * 1000.0f;
		if (diff < (1000.0f / TARGET_FPS)) {
			float time_to_sleep = 1000.0f / TARGET_FPS - diff;
			//printf("%fms\n", time_to_sleep);
			sleep_c(time_to_sleep);
		}
	}
}

void duck_draw_basic_rect(mat4x4 transform, vec4 colour) {
	GLuint transLoc = glGetUniformLocation(colourShaderProgram, "transform");
	GLuint colourLoc = glGetUniformLocation(colourShaderProgram, "colour");

	glUseProgram(colourShaderProgram);
	glUniformMatrix4fv(transLoc, 1, GL_FALSE, transform[0]);
	glUniform4fv(colourLoc, 1, colour);

	glBindVertexArray(basicRectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void duck_deinit(void) {
	glDeleteBuffers(1, &basicRectVBO);
	glDeleteBuffers(1, &texRectVBO);
	glDeleteBuffers(1, &rectEBO);

	glDeleteVertexArrays(1, &basicRectVAO);
	glDeleteVertexArrays(1, &texRectVAO);

	glDeleteProgram(colourShaderProgram);
	glDeleteProgram(texShaderProgram);
}