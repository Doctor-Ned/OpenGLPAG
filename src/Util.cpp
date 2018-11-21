#include "Util.h"
#include <fstream>

GLuint Util::createAndCompileShader(int shaderType, const char* file) {
	GLuint shader;
	shader = glCreateShader(shaderType);
	std::ifstream data(file, std::ios::binary | std::ios::ate);
	std::streamsize fileSize = data.tellg();
	data.seekg(0, std::ios::beg);
	char *shaderText = new char[fileSize + 1];
	shaderText[fileSize] = '\0';
	if (data.read(shaderText, fileSize)) {
		glShaderSource(shader, 1, (const GLchar**)&shaderText, 0);
		glCompileShader(shader);
		GLint isFine, maxLength;
		char *errorLog;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isFine);
		if (isFine == FALSE) {
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
			errorLog = new char[maxLength];
			glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog);
			fprintf(stderr, "An error occurred while compiling shader from file '%s'. %s\n", file, errorLog);
			exit(1);
			return NULL;
		}
		return shader;
	} else {
		printf("Unable to read the shader file '%s'.", file);
		exit(1);
		return NULL;
	}
}

void Util::linkShaderProgram(GLuint shaderProgram) {
	glLinkProgram(shaderProgram);
	GLint isFine, maxLength;
	char *errorLog;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isFine);
	if (isFine == FALSE) {
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		errorLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, errorLog);
		fprintf(stderr, "An error occurred while linking the shader program. %s\n", errorLog);
		delete errorLog;
		exit(1);
	}
}

void Util::bindUniformBlock(GLuint shaderProgram, const char* blockName, GLuint binding) {
	GLuint index = glGetUniformBlockIndex(shaderProgram, blockName);
	glUniformBlockBinding(shaderProgram, index, binding);
}

GLuint Util::createUbo(unsigned int size, GLuint binding) {
	GLuint ubo;
	glGenBuffers(1, &ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, binding, ubo, 0, size);
	return ubo;
}

GLuint Util::createUboViewProjection(GLuint binding) {
	return createUbo(2 * sizeof(glm::mat4), binding);
}

void Util::injectUboViewProjection(GLuint ubo, glm::mat4 view, glm::mat4 projection) {
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

GLuint Util::createUboLight(GLuint binding) {
	return createUbo(2 * sizeof(glm::vec4), binding);
}

void Util::injectUboLight(GLuint ubo, glm::vec4 lightPosition, glm::vec4 lightColor) {
	glBindBuffer(GL_UNIFORM_BUFFER, ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(lightPosition));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(lightColor));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}