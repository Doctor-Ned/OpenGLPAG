#version 430 core

layout (location = 0) in vec3 inPosition;
uniform mat4 model;
layout (std140) uniform ViewProjection {
    mat4 view;
    mat4 projection;
}
;
out vec3 geoPosition[1];
void main() {
    geoPosition[0] = inPosition;
    gl_Position = vec4(inPosition,1.0f);
}