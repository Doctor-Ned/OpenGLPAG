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

in VS_OUT {
	vec3 Pos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentPos;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

out vec4 outColor;

void main() {

	vec3 normal = texture(texture_normal1, fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	vec3 color = disableTexture ? vec3(color) : texture(texture_diffuse1, fs_in.TexCoords).rgb;
	vec3 ambient = 0.1f * color;
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

	vec3 specular = vec3(0.2) * spec;
	outColor = vec4(ambient + diffuse + specular, 1.0);
//	vec4 diffColor = disableTexture ? color : texture(texture_diffuse1, exTexCoord);
//	float ambientStrength = 0.1f;
//	mat3 TBN = mat3(exTangent, exBitangent, exNormal);
//	vec3 lightDirection = normalize(vec3(lightPosition) - exPosition);
//	vec3 normal = normalize(TBN * normalize(texture(texture_normal1, exTexCoord).rgb) * 2.0 - 1.0);
//	//vec3 normal = exNormal;
//	float mult = max(dot(normal, lightDirection), 0.0f);
//	vec4 ambient = ambientStrength * lightColor * diffColor;
//	vec4 diffuse = mult * diffColor;
//	vec4 specular = texture(texture_specular1, exTexCoord);
//	outColor = ambient + diffuse + specular;
//	outColor.w = 1.0f;
}