#version 430 core

// to be used with modelFragmentShader.glsl

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;
layout (location = 5) in vec3 inOffset;

uniform mat4 model;
uniform float scale;
uniform vec3 viewPosition;

layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};

out VS_OUT {
	vec3 pos;
	vec2 texCoords;
	vec3 normal;
	vec3 viewPosition;
} vs_out;

void main() {
	vec3 pos = inPosition * scale;
	pos += inOffset;
    vs_out.texCoords = inTexCoord;
    vs_out.pos = vec3(model * vec4(pos, 1.0f));
	vs_out.normal = normalize(vec3(model * vec4(inNormal, 0.0f)));
	vs_out.viewPosition = normalize(vec3(model * vec4(viewPosition, 1.0f)));
    gl_Position = projection * view * vec4(vs_out.pos, 1.0f);
}