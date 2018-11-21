#version 430 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
uniform mat4 model;
layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};
out vec3 exPosition;
out vec3 exNormal;
out vec2 exTexCoord;
void main() {
    exTexCoord = inTexCoord;
    exPosition = vec3(model * vec4(inPosition, 1.0));
    exNormal = vec3(model * vec4(inNormal, 1.0));
    gl_Position = projection * view * vec4(exPosition, 1.0);
}