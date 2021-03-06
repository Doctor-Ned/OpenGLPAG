#version 330 core

out vec4 outColor;

uniform samplerCube environmentMap;

in VS_OUT {
	vec3 pos;
	vec3 normal;
	vec3 viewPosition;
	vec3 refraction;
} fs_in;

void main() {
	outColor = vec4(texture(environmentMap, fs_in.refraction).rgb, 1.0);
}