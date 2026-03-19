#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <unistd.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "utils.h"
#include "stb_image.h"
#include "linmath.h"

int main(void) {
	// Boilerplate start
	const double TARGET_FPS = 30.0;

	setbuf(stdout, NULL);

	glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD");
		return -1;
	}
	
	glViewport(0, 0, 800, 600);
	
	stbi_set_flip_vertically_on_load(true);
	// Boilerplate end
	
	const char *vertSource = ReadFileToString("shaders/default.vert");
	const char *fragSource = ReadFileToString("shaders/default.frag");
	
	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertShader, 1, &vertSource, NULL);
	glCompileShader(vertShader);
	
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, &fragSource, NULL);
	glCompileShader(fragShader);
	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertShader);
	glAttachShader(shaderProgram, fragShader);
	glLinkProgram(shaderProgram);
	
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f
	};
	
	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	
	GLuint VBO;
	glGenBuffers(1, &VBO);
	
	GLuint EBO;
	glGenBuffers(1, &EBO);
	
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);  
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);  
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	unsigned char *data = stbi_load("resources/image.jpg", &width, &height, &nrChannels, 0);
	if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        printf("cooked...");
		printf("%s", stbi_failure_reason());
    }
	stbi_image_free(data);
	
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

	mat4x4 trans;
	mat4x4_identity(trans);
	mat4x4_translate(trans, 0.5f, 0.5f, 0.0f);
	
	clock_t start, end;

	while(!glfwWindowShouldClose(window)) {
		start = clock();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
	
		glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
		
		glUseProgram(shaderProgram);
		mat4x4_rotate_Z(trans, trans, 0.1f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, trans[0]);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		end = clock();

		double diff = (double)(end - start) / CLOCKS_PER_SEC * 1000.0;
		if (diff < (1000.0 / TARGET_FPS)) {
			printf("%fms\n", (1000.0 / TARGET_FPS - diff));
			// usleep is in microseconds, hence * 1000.0
			usleep((1000.0 / TARGET_FPS - diff) * 1000.0);
		}
	}
	
	free((char*)vertSource);
	free((char*)fragSource);
	
	glfwTerminate();
	return 0;
}