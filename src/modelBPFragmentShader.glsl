#version 430 core

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_LIGHTS_OF_TYPE 16

layout (std140) uniform TextureColor {
	vec4 color;
	bool disableTexture;
};
layout (std140) uniform Lights {
	int dirLightAmount;
	int pointLightAmount;
	int spotLightAmount;
	DirLight dirLights[MAX_LIGHTS_OF_TYPE];
	PointLight pointLights[MAX_LIGHTS_OF_TYPE];
	SpotLight spotLights[MAX_LIGHTS_OF_TYPE];
};

in VS_OUT {
	vec3 pos;
	vec2 texCoords;
	vec3 normal;
	vec3 viewPosition;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;
uniform mat4 model;

out vec4 outColor;

vec3 calcDirLight(DirLight light, vec3 diffuse, vec3 specular, vec3 viewDir) {
	vec3 direction = normalize(vec3(model * vec4(-light.direction, 1.0f)));
    float diff = max(dot(direction, fs_in.normal), 0.0);

	vec3 reflectDir = reflect(-direction, fs_in.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	return ((light.ambient * diffuse) + (light.diffuse * diff * diffuse) + (light.specular * spec * specular));
}

vec3 calcPointLight(PointLight light, vec3 diffuse, vec3 specular, vec3 viewDir) {
	vec3 position = vec3(model * vec4(light.position, 1.0f));
	vec3 direction = normalize(position - fs_in.pos);
	float diff = max(dot(direction, fs_in.normal), 0.0);

	vec3 reflectDir = reflect(-direction, fs_in.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

	float dist = length(position - fs_in.pos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (((light.ambient * diffuse) + (light.diffuse * diff * diffuse) + (light.specular * spec * specular)) * attenuation);
}

vec3 calcSpotLight(SpotLight light, vec3 diffuse, vec3 specular, vec3 viewDir) {
	vec3 position = vec3(model * vec4(light.position, 1.0f));
	vec3 direction = normalize(position - fs_in.pos);
	vec3 spotDirection = normalize(vec3(model * vec4(light.direction, 1.0f)));
	float diff = max(dot(direction, fs_in.normal), 0.0);
	
	vec3 reflectDir = reflect(-direction, fs_in.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	
	float dist = length(position - fs_in.pos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	float theta = dot(direction, spotDirection);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	return (((light.ambient * diffuse) + (light.diffuse * diff * diffuse) + (light.specular * spec * specular)) * attenuation * intensity);
}

void main() {
	vec3 diffuse = disableTexture ? color.rgb : texture(texture_diffuse1, fs_in.texCoords).rgb;
    vec3 ambient = 0.05 * diffuse;
    vec3 specular = texture(texture_specular1, fs_in.texCoords).rgb;
	vec3 viewDir = normalize(fs_in.viewPosition - fs_in.pos);

	vec3 color = vec3(0.0f);

	for(int i=0;i<dirLightAmount;i++) {
		color += calcDirLight(dirLights[i], diffuse, specular, viewDir);
	}
	for(int i=0;i<pointLightAmount;i++) {
		color += calcPointLight(pointLights[i], diffuse, specular, viewDir);
	}
	for(int i=0;i<spotLightAmount;i++) {
		color += calcSpotLight(spotLights[i], diffuse, specular, viewDir);
	}

    outColor = vec4(color, 1.0f);
}