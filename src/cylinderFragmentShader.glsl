#version 430 core

layout (std140) uniform Light {
    vec4 lightPosition;
    vec4 lightColor;
};
uniform vec4 color;
out vec4 outColor;
void main() {
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * vec3(lightColor);
    outColor=vec4(ambient,1.0f) * color;
}