#version 430 core

in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;
in vec3 exTangent;
in vec3 exBitangent;
layout (std140) uniform Light {
	vec4 lightPosition;
	vec4 lightColor;
};
layout (std140) uniform TextureColor {
	vec4 color;
	bool disableTexture;
};
uniform sampler2D texture_diffuse1;

out vec4 outColor;

//TODO: add different textures, use tangent & bitangent

void main() {
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * vec3(lightColor);
	
	vec3 norm = normalize(exNormal);
	vec3 lightDirection = normalize(vec3(lightPosition) - exPosition);
	float diff = max(dot(norm, lightDirection), 0.0f);
	vec3 diffuse = diff * vec3(lightColor);
	outColor=vec4(ambient+diffuse,1.0f) * color;
	if(!disableTexture) outColor = outColor * texture(texture_diffuse1, exTexCoord);
}