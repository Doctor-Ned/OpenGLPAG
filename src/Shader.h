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
	void refreshUniforms();
	void setDisableTexture(bool disable);
	void setScale(float scale);
	void setColor(glm::vec4 color);
	void setModel(glm::mat4 model);
	void setViewPosition(glm::vec3 viewPosition);
	void setViewDirection(glm::vec3 viewDirection);
	void setShininess(float shininess);
	void setUseSpecular(bool useSpecular);
	void setUseLight(bool useLight);
	void setView(glm::mat4 view);
	void setProjection(glm::mat4 projection);
	bool getTextureDisabled();
	float getScale();
	glm::vec4 getColor();
	glm::mat4 getModel();
	glm::vec3 getViewPosition();
	glm::vec3 getViewDirection();
	glm::mat4 getView();
	glm::mat4 getProjection();
	void setInt(char* name, int value);
	bool getBlinnPhong();
	bool getUseSpecular();
	bool getUseLight();
	float getShininess();
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
	glm::vec4 *color = NULL;
	glm::mat4 *model = NULL, *view = NULL, *projection = NULL;
	glm::vec3 *viewPosition = NULL, *viewDirection = NULL;
	float *scale = NULL;
	float *shininess = NULL;
	bool *disableTexture = NULL, *blinnPhong = NULL, *useSpecularMap = NULL, *useLight = NULL;
};

#endif