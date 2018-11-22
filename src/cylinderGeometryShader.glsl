#version 430 core

layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};
uniform float scale;
uniform mat4 model;
uniform float radius;
uniform float height;
uniform int sideAmount;

out vec3 exPosition;
out vec3 exNormal;
out vec2 exTexCoord;

layout(points) in;
layout(triangle_strip, max_vertices = 3) out;

void main() {
	mat4 mvp = projection * view * model;
	exTexCoord = vec2(0.0f,0.0f);
	exNormal = vec3(0.0f, 0.0f, -1.0f);

    gl_Position = mvp * gl_in[0].gl_Position;
	exPosition = vec3(gl_Position);
    EmitVertex();

	exTexCoord = vec2(1.0f,0.0f);
	
	gl_Position = mvp * (gl_in[0].gl_Position + vec4(0.0f, height, 0.0f, 1.0f));
	exPosition = vec3(gl_Position);
    EmitVertex();

	exTexCoord = vec2(0.0f,1.0f);
	
	gl_Position = mvp * (gl_in[0].gl_Position + vec4(radius, 0.0f, 0.0f, 1.0f));
	exPosition = vec3(gl_Position);
    EmitVertex();

    EndPrimitive();
}