#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D textureUnit;

void main() {
    FragColor = texture(textureUnit, TexCoord);
} 