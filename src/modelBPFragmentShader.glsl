#version 430 core

struct DirLight {
	vec4 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	mat4 model;
};

struct PointLight {
	vec4 position;
	float constant;
	float linear;
	float quadratic;
	float padding;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	mat4 model;
};

struct SpotLight {
	vec4 position;
	vec4 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	mat4 model;
	vec3 padding;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

#define MAX_LIGHTS_OF_TYPE 16

layout (std140) uniform TextureColor {
	vec4 color;
	bool disableTexture;
};
layout (std140) uniform DirLights {
	vec3 dpadding;
	int dirLightAmount;
	DirLight dirLights[MAX_LIGHTS_OF_TYPE];
};
layout (std140) uniform PointLights {
	vec3 ppadding;
	int pointLightAmount;
	PointLight pointLights[MAX_LIGHTS_OF_TYPE];
};
layout (std140) uniform SpotLights {
	vec3 spadding;
	int spotLightAmount;
	SpotLight spotLights[MAX_LIGHTS_OF_TYPE];
};
layout (std140) uniform Light {
	vec4 lightPosition;
	vec4 lightColor;
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
uniform int blinnPhong;
uniform int useSpecularMap;
uniform int useLight;

out vec4 outColor;

vec3 calcDirLight(DirLight light, vec3 diffuse, vec3 specular, vec3 viewDir) {
	vec3 direction = normalize(vec3(light.model * (-light.direction)));
    float diff = max(dot(direction, fs_in.normal), 0.0);

	vec3 reflectDir = reflect(-direction, fs_in.normal);
	vec3 halfwayDir = normalize(direction + viewDir);
    float spec = pow(max(dot(fs_in.normal, halfwayDir), 0.0), shininess);

	return ((vec3(light.ambient) * diffuse) + (vec3(light.diffuse) * diff * diffuse) + (vec3(light.specular) * spec * specular));
}

vec3 calcPointLight(PointLight light, vec3 diffuse, vec3 specular, vec3 viewDir) {
	vec3 position = vec3(light.model * vec4(0.0f, 0.0f, 0.0f, 1.0f));
	vec3 direction = normalize(position - fs_in.pos);
	float diff = max(dot(direction, fs_in.normal), 0.0);
	
	vec3 reflectDir = reflect(-direction, fs_in.normal);
	vec3 halfwayDir = normalize(direction + viewDir);
    float spec = pow(max(dot(fs_in.normal, halfwayDir), 0.0), shininess);

	float dist = length(position - fs_in.pos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (((vec3(light.ambient) * diffuse) + (vec3(light.diffuse) * diff * diffuse) + (vec3(light.specular) * spec * specular)) * attenuation);
}

vec3 calcSpotLight(SpotLight light, vec3 diffuse, vec3 specular, vec3 viewDir) {
	vec3 position = vec3(light.model * vec4(0.0f, 0.0f, 0.0f, 1.0f));
	vec3 direction = normalize(position - fs_in.pos);
	//vec3 spotDirection = normalize(vec3(light.model * -light.direction));
	vec3 spotDirection = normalize(vec3(light.model * vec4(vec3(-light.direction), 0.0f)));
	float diff = max(dot(direction, fs_in.normal), 0.0);

	float cutOff = cos(light.cutOff);
	float outerCutOff = cos(light.outerCutOff);
	
	vec3 reflectDir = reflect(-direction, fs_in.normal);
	vec3 halfwayDir = normalize(direction + viewDir);
    float spec = pow(max(dot(fs_in.normal, halfwayDir), 0.0), shininess);
	
	float dist = length(position - fs_in.pos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
	
	float theta = dot(direction, spotDirection);
	float epsilon = cutOff - outerCutOff;
	float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);

	vec3 amb = vec3(light.ambient) * diffuse;
	vec3 dif = vec3(light.diffuse) * diff * diffuse * intensity;
	vec3 spe = vec3(light.specular) * spec * specular * intensity;

	return (amb + dif + spe) * attenuation;

	//return (((vec3(light.ambient) * diffuse) + (vec3(light.diffuse) * diff * diffuse) + (vec3(light.specular) * spec * specular)) * attenuation * intensity);
}

void main() {
	vec3 diffuse = color.rgb;
	if(!disableTexture) diffuse *= texture(texture_diffuse1, fs_in.texCoords).rgb;
    vec3 ambient = 0.05 * diffuse;
	if(useLight == 0) {
		outColor = vec4(ambient + diffuse, 1.0f);
	} else if(blinnPhong > 0) {
		vec3 specular = useSpecularMap > 0 ? texture(texture_specular1, fs_in.texCoords).rgb : vec3(0.5f);
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
	} else {
		vec3 lightDirection = normalize(vec3(lightPosition) - fs_in.pos);
		float diff = max(dot(fs_in.normal, lightDirection), 0.0f);
		diffuse *= diff * vec3(lightColor);
		outColor = vec4(ambient + diffuse, 1.0f);
	}
}