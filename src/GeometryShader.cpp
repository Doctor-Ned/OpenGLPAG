#include "Shader.h"
#include <fstream>
#include "GeometryShader.h"

GeometryShader::GeometryShader(char * vertexPath, char * fragmentPath, char * geometryPath) : Shader(vertexPath, fragmentPath, false) {
	this->geometryPath = geometryPath;
	createShaders();
	linkShaderProgram();
	deleteShaders();
}

void GeometryShader::createShaders() {
	Shader::createShaders();
	shaders.push_back(createAndCompileShader(GL_GEOMETRY_SHADER, geometryPath));
}
