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
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentPos;
} vs_out;
void main() {

	vec3 pos = inPosition * scale;
    vs_out.TexCoords = inTexCoord;
    vs_out.Pos = vec3(model * vec4(pos, 1.0f));
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vec3 T = normalize(normalMatrix * inTangent);
	vec3 N = normalize(normalMatrix * inNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	mat3 TBN = transpose(mat3(T, B, N));
	vs_out.TangentLightPos = TBN * vec3(lightPosition);
	vs_out.TangentViewPos = TBN * viewPosition;
	vs_out.TangentPos = TBN * vs_out.Pos;
    gl_Position = projection * view * vec4(vs_out.Pos, 1.0f);
}