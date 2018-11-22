#version 430 core

layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};
uniform mat4 model;
uniform float radius;
uniform float height;
uniform int sideAmount;

in vec3 geoPosition;
in vec3 geoNormal;

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {
	exPosition = geoPosition;
	exNormal = geoNormal;
	mat4 mvp = projection * view * model;

    gl_Position = gl_in[0].gl_Position + vec4(-0.1, 0.0, 0.0, 0.0);
    EmitVertex();

    EndPrimitive();
}