#version 430 core

in vec3 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;

uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform vec4 color;
uniform sampler2D imgTexture;
uniform int disableTexture;

out vec4 outColor;

void main() {
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;
		
	vec3 norm = normalize(exNormal);
	vec3 lightDirection = normalize(lightPosition - exPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColor;
	outColor=vec4(ambient+diffuse,1.0f) * color;
	if(disableTexture == 0) outColor = outColor * texture(imgTexture, exTexCoord);
}