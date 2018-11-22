#version 430 core

uniform float scale;
layout (location = 0) in vec3 inPosition;
void main() {
    gl_Position = vec4(inPosition*scale,1.0f);
}
