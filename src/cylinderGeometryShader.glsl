#version 430 core

layout (std140) uniform ViewProjection {
	mat4 view;
	mat4 projection;
};
uniform float scale;
uniform mat4 model;
uniform float height;

out vec4 exPosition;
out vec3 exNormal;
out vec2 exTexCoord;

layout(triangles) in;
layout(triangle_strip, max_vertices = 12) out;

void createTriangles(vec4 center, vec4 closer, vec4 farther) {
	mat4 vp = projection * view;

	//bottom triangle
	exNormal = vec3(0.0f, -1.0f, 0.0f);
	exNormal = vec3(model * vec4(exNormal, 0.0f));

	exTexCoord = vec2(0.5f, 1.0f);
	exPosition = model * center;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(1.0f, 0.0f);
	exPosition = model * farther;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(0.0f, 0.0f);
	exPosition = model * closer;
	gl_Position = vp * exPosition;
	EmitVertex();

	EndPrimitive();

	//top triangle
	exNormal = vec3(0.0f, 1.0f, 0.0f);
	exNormal = vec3(model * vec4(exNormal, 0.0f));

	exTexCoord = vec2(0.5f, 1.0f);
	exPosition = center;
	exPosition.y += height * scale;
	exPosition = model * exPosition;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(0.0f, 0.0f);
	exPosition = closer;
	exPosition.y += height * scale;
	exPosition = model * exPosition;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(1.0f, 0.0f);
	exPosition = farther;
	exPosition.y += height * scale;
	exPosition = model * exPosition;
	gl_Position = vp * exPosition;
	EmitVertex();

	EndPrimitive();

	//side triangles
	vec4 upCloser = closer, upFarther = farther;
	upCloser.y+=height * scale;
	upFarther.y+=height * scale;

	vec2 flatLine = vec2(farther.x-closer.x, farther.z-closer.z);
	exNormal = normalize(vec3(flatLine.y, 0.0f, -flatLine.x));
	exNormal = vec3(model * vec4(exNormal, 0.0f));

	exTexCoord = vec2(0.0f, 1.0f);
	exPosition = model * upCloser;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(0.0f, 0.0f);
	exPosition = model * closer;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(1.0f, 0.0f);
	exPosition = model * farther;
	gl_Position = vp * exPosition;
	EmitVertex();

	EndPrimitive();

	exTexCoord = vec2(0.0f, 1.0f);
	exPosition = model * upCloser;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(1.0f, 0.0f);
	exPosition = model * farther;
	gl_Position = vp * exPosition;
	EmitVertex();

	exTexCoord = vec2(1.0f, 1.0f);
	exPosition = model * upFarther;
	gl_Position = vp * exPosition;
	EmitVertex();

	EndPrimitive();
}

void main() {
	createTriangles(gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position);
}