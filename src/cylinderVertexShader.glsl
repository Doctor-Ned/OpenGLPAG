#version 430 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
uniform mat4 model;
layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};
out vec3 geoPosition;
out vec3 geoNormal;
void main() {
    geoPosition = inPosition;
    geoNormal = inNormal;
    gl_Position = gl_Vertex;
}