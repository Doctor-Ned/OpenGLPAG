#version 430 core

layout (location = 0) in vec3 inPosition;
uniform float scale;
void main() {
    gl_Position = vec4(inPosition*scale,1.0f);
}
