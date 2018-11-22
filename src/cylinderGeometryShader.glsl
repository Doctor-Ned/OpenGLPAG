#version 430 core
#define M_PI 3.1415926535897932384626433832795

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

void createTopBottomTriangle(vec4 center, float angle1, float angle2, mat4 mvp) {
	vec4 closer = center;
	closer.x += cos(angle1) * radius;
	closer.z += sin(angle1) * radius;
	vec4 farther = center;
	farther.x += cos(angle2) * radius;
	farther.z += sin(angle2) * radius;

	exNormal = vec3(0.0f, -1.0f, 0.0f);
	
	gl_Position = mvp * center;
	exTexCoord.x = 0.5f;
	exTexCoord.y = 1.0f;
	EmitVertex();

	gl_Position = mvp * closer;
	exTexCoord.x = 0.0f;
	exTexCoord.y = 0.0f;
	EmitVertex();

	gl_Position = mvp * farther;
	exTexCoord.x = 1.0f;
	exTexCoord.y = 0.0f;
	EmitVertex();

	EndPrimitive();

	center.y+=height;
	closer.y+=height;
	farther.y+=height;
	exNormal = vec3(0.0f, 1.0f, 0.0f);

	gl_Position = mvp * center;
	exTexCoord.x = 0.5f;
	exTexCoord.y = 1.0f;
	EmitVertex();

	gl_Position = mvp * closer;
	exTexCoord.x = 0.0f;
	exTexCoord.y = 0.0f;
	EmitVertex();

	gl_Position = mvp * farther;
	exTexCoord.x = 1.0f;
	exTexCoord.y = 0.0f;
	EmitVertex();

	EndPrimitive();
}

void createSideTriangles(vec4 center, float angle1, float angle2, mat4 mvp) {
	vec4 closer = center;
	closer.x += cos(angle1) * radius;
	closer.z += sin(angle1) * radius;
	vec4 farther = center;
	farther.x += cos(angle2) * radius;
	farther.z += sin(angle2) * radius;
	vec4 upCloser = closer;
	upCloser.y += height;
	vec4 upFarther = farther;
	upFarther.y += height;

	vec2 flatLine = vec2(farther.x-closer.x, farther.z-closer.z);
	exNormal = normalize(vec3(flatLine.y, 0.0f, -flatLine.x));

	gl_Position = mvp * closer;
	exTexCoord.x = 0.0f;
	exTexCoord.y = 0.0f;
	EmitVertex();

	gl_Position = mvp * farther;
	exTexCoord.x = 1.0f;
	exTexCoord.y = 1.0f;
	EmitVertex();

	gl_Position = mvp * upCloser;
	exTexCoord.x = 0.0f;
	exTexCoord.y = 1.0f;
	EmitVertex();

	EndPrimitive();

	gl_Position = mvp * upCloser;
	exTexCoord.x = 0.0f;
	exTexCoord.y = 1.0f;
	EmitVertex();

	gl_Position = mvp * farther;
	exTexCoord.x = 1.0f;
	exTexCoord.y = 1.0f;
	EmitVertex();

	gl_Position = mvp * upFarther;
	exTexCoord.x = 1.0f;
	exTexCoord.y = 0.0f;
	EmitVertex();

	EndPrimitive();
}

void createTriangles(vec4 center, float radStep, mat4 mvp) {
	bool last;
	float angle = 0.0f;
	for(int i=0;i<sideAmount;i++) {
		last = i == sideAmount - 1;
		createTopBottomTriangle(center, angle, last ? 0.0f : angle + radStep, mvp);
		createSideTriangles(center, angle, last ? 0.0f : angle + radStep, mvp);
	}
}

void main() {
	mat4 mvp = projection * view * model;
	float radStep = 2 * M_PI / sideAmount;
	vec4 center = gl_in[0].gl_Position;
	center.w = 1.0f;
	createTriangles(center, radStep, mvp);
}