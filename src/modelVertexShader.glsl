#version 430 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

uniform mat4 model;
uniform float scale;
layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};
out vec3 exPosition;
out vec3 exNormal;
out vec3 exTangent;
out vec3 exBitangent;
out vec2 exTexCoord;
void main() {
	vec3 pos = inPosition * scale;
    exTexCoord = inTexCoord;
    exPosition = vec3(model * vec4(pos, 1.0f));
    exNormal = vec3(model * vec4(inNormal, 0.0f));
	exTangent = vec3(model * vec4(inTangent, 0.0f));
	exBitangent = vec3(model * vec4(inBitangent, 0.0f));
    gl_Position = projection * view * vec4(exPosition, 1.0f);
}