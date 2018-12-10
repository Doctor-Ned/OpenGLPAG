#version 430 core

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
	vec3 Normal;
	vec3 ViewPosition;
} fs_in;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform float shininess;

out vec4 outColor;

void main() {
	vec3 color = disableTexture ? color.rgb : texture(texture_diffuse1, fs_in.TexCoords).rgb;
    vec3 ambient = 0.05 * color;
    
    vec3 lightDir = normalize(vec3(lightPosition) - fs_in.Pos);
    float diff = max(dot(lightDir, fs_in.Normal), 0.0);
    vec3 diffuse = diff * color;
    
	vec3 viewDir = normalize(fs_in.ViewPosition - fs_in.Pos);
    vec3 reflectDir = reflect(-lightDir, fs_in.Normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(fs_in.Normal, halfwayDir), 0.0), shininess);
    vec3 specular = spec * texture(texture_specular1, fs_in.TexCoords).rgb;

    outColor = vec4(ambient + diffuse + specular, 1.0);
}