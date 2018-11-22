#version 430 core

in vec3 exPosition;
in vec3 exColor;
layout (std140) uniform Light {
    vec4 lightPosition;
    vec4 lightColor;
};
out vec4 outColor;
void main() {
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * vec3(lightColor);
    outColor=vec4(ambient*exColor,1.0f);
}

