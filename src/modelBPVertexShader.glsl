#version 430 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

uniform mat4 model;
uniform float scale;
uniform vec3 viewPosition;

layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};
layout (std140) uniform Light {
	vec4 lightPosition;
	vec4 lightColor;
};

out VS_OUT {
	vec3 Pos;
	vec2 TexCoords;
	vec3 Normal;
	vec3 ViewPosition;
} vs_out;

void main() {
	vec3 pos = inPosition * scale;
    vs_out.TexCoords = inTexCoord;
    vs_out.Pos = vec3(model * vec4(pos, 1.0f));
	vs_out.Normal = normalize(vec3(model * vec4(inNormal, 0.0f)));
	vs_out.ViewPosition = vec3(model * vec4(viewPosition, 1.0f));
    gl_Position = projection * view * vec4(vs_out.Pos, 1.0f);
}