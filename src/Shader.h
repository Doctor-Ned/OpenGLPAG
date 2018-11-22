#ifndef SHADER_H
#define SHADER_H

#include "Headers.h"
#include "Ubo.h"

class Shader {
public:
	Shader(char *vertexPath, char *fragmentPath);
	virtual GLuint getID();
	void use();
	void remove();
	void setColor(glm::vec4 color);
	void setModel(glm::mat4 model);
	virtual void bind(Ubo *ubo);
	GLint getUniformLocation(const char *name);
protected:
	Shader(char *vertexPath, char *fragmentPath, bool initialise);
	GLuint createAndCompileShader(int shaderType, const char* file);
	virtual void createShaders();
	void deleteShaders();
	void linkShaderProgram();
	GLuint id;
	std::vector<GLuint> shaders;
	char *fragmentPath;
	char *vertexPath;
};

#endif