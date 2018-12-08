#version 430 core

in vec4 exPosition;
in vec3 exNormal;
in vec2 exTexCoord;
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
void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(lightColor);
    vec3 norm = normalize(exNormal);
    vec3 lightDirection = normalize(vec3(lightPosition) - vec3(exPosition));
    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * vec3(lightColor);
    outColor=vec4(ambient+diffuse,1.0f) * color;
    if(!disableTexture) outColor = outColor * texture(texture_diffuse1, exTexCoord);
}
