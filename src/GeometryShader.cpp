#include "Shader.h"
#include <fstream>
#include "GeometryShader.h"

GeometryShader::GeometryShader(char * vertexPath, char * fragmentPath, char * geometryPath) : Shader(vertexPath, fragmentPath, false) {
	this->geometryPath = geometryPath;
	createShaders();
	linkShaderProgram();
	deleteShaders();
}

void GeometryShader::setValues(float radius, float height, int sideAmount) {
	glUniform1f(getUniformLocation("radius"), radius);
	glUniform1f(getUniformLocation("height"), height);
	glUniform1i(getUniformLocation("sideAmount"), sideAmount);
}

void GeometryShader::createShaders() {
	Shader::createShaders();
	shaders.push_back(createAndCompileShader(GL_GEOMETRY_SHADER, geometryPath));
}
