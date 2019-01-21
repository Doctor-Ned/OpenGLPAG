#version 430 core

in vec2 exTexCoords;
out vec4 outColor;
uniform sampler2D text;

void main() {
	outColor=texture(text, exTexCoords);
}