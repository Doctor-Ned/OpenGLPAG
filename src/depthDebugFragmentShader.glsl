#version 430 core

out vec4 outColor;

in vec2 exTexCoords;

uniform sampler2D map;
uniform float near_plane;
uniform float far_plane;

float linearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
}

void main() { 
	float depthValue = texture(map, exTexCoords).r;
    outColor = vec4(vec3(depthValue), 1.0);
}