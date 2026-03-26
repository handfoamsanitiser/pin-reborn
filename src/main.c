#include <stdio.h>
#include <stdlib.h>

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "duck.h"
#include "linmath.h"
#include "stb_image.h"
#include "utils.h"

void main_loop(void);

int main(void) {
	int status = duck_init();
	if (status == -1) {
		return -1;
	}

	duck_main_loop(&main_loop);
	
	/*GLuint texture;
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
	glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);*/

	duck_deinit();
	
	glfwTerminate();
	return 0;
}

void main_loop(void) {
	// processing
	mat4x4 trans;
    mat4x4_identity(trans);

    //mat4x4_translate(trans, 0.5f, 0.5f, 0.0f);
    mat4x4_scale_aniso(trans, trans, 0.2f, 0.2f, 0.2f);

    vec4 colour = {0.5f, 0.5f, 0.0f, 1.0f};

    // rendering
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

	duck_draw_basic_rect(trans, colour);
}