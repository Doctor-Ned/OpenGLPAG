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
	void setDisableTexture(bool disable);
	void setScale(float scale);
	void setColor(glm::vec4 color);
	void setModel(glm::mat4 model);
	bool getTextureDisabled();
	float getScale();
	glm::vec4 getColor();
	glm::mat4 getModel();
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
	glm::vec4 color;
	glm::mat4 model;
	float scale;
	bool disableTexture;
};

#endif