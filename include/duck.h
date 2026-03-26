#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "linmath.h"

/*extern GLuint basicRectVAO;
extern GLuint texRectVAO;

extern GLuint basicRectVBO;
extern GLuint texRectVBO;
extern GLuint rectEBO;*/

int duck_init(void);
void duck_main_loop(void (*main_loop)(void));
void duck_draw_basic_rect(mat4x4 transform, vec4 colour);
void duck_draw_tex_rect(mat4x4 transform, GLuint textureUnit, vec2 textureCoords);
void duck_deinit(void);