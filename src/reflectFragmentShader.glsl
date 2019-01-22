#version 430 core

out vec4 outColor;

uniform samplerCube environmentMap;

in VS_OUT {
	vec3 pos;
	vec3 normal;
	vec3 viewPosition;
} fs_in;

void main() {
	vec3 I = normalize(fs_in.pos - fs_in.viewPosition);
	vec3 R = reflect(I, normalize(fs_in.normal));
	outColor = vec4(texture(environmentMap, R).rgb, 1.0);
}