#include "Shader.h"
#include <fstream>

Shader::Shader(char * vertexPath, char * fragmentPath) : Shader(vertexPath, fragmentPath, true) {}

Shader::Shader(char * vertexPath, char * fragmentPath, bool initialise) {
	this->vertexPath = vertexPath;
	this->fragmentPath = fragmentPath;
	id = glCreateProgram();
	if (initialise) {
		createShaders();
		linkShaderProgram();
		deleteShaders();
	}
}

void Shader::createShaders() {
	shaders.push_back(createAndCompileShader(GL_VERTEX_SHADER, vertexPath));
	shaders.push_back(createAndCompileShader(GL_FRAGMENT_SHADER, fragmentPath));
}

void Shader::deleteShaders() {
	while (!shaders.empty()) {
		glDeleteShader(shaders.back());
		shaders.pop_back();
	}
}

GLuint Shader::getID() {
	return id;
}

void Shader::use() {
	glUseProgram(id);
}

void Shader::remove() {
	glDeleteProgram(id);
}

void Shader::refreshUniforms() {
	if (disableTexture != NULL) {
		setDisableTexture(*disableTexture);
	}
	if (scale != NULL) {
		setScale(*scale);
	}
	if (color != NULL) {
		setColor(*color);
	}
	if (model != NULL) {
		setModel(*model);
	}
}

// deprecated, should be injected via UboTextureColor
void Shader::setDisableTexture(bool disable) {
	if (this->disableTexture == NULL) {
		this->disableTexture = new bool();
	}
	glUniform1i(getUniformLocation("disableTexture"), disable ? 1 : 0);
	*disableTexture = disable;
}

void Shader::setScale(float scale) {
	if (this->scale == NULL) {
		this->scale = new float();
	}
	glUniform1f(getUniformLocation("scale"), scale);
	*(this->scale) = scale;
}

// currently injected via UboTextureColor, though it might be useful when dealing with non-texture shaders
void Shader::setColor(glm::vec4 color) {
	if (this->color == NULL) {
		this->color = new glm::vec4();
	}
	glUniform4f(getUniformLocation("color"), color.x, color.y, color.z, color.w);
	*(this->color) = color;
}

void Shader::setModel(glm::mat4 model) {
	if (this->model == NULL) {
		this->model = new glm::mat4();
	}
	glUniformMatrix4fv(getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
	*(this->model) = model;
}

bool Shader::getTextureDisabled() {
	return *disableTexture;
}

float Shader::getScale() {
	return *scale;
}

glm::vec4 Shader::getColor() {
	return *color;
}

glm::mat4 Shader::getModel() {
	return *model;
}

void Shader::bind(Ubo * ubo) {
	GLuint index = glGetUniformBlockIndex(id, ubo->getBlockName());
	glUniformBlockBinding(id, index, ubo->getBinding());
}

GLint Shader::getUniformLocation(const char * name) {
	return glGetUniformLocation(id, name);
}

GLuint Shader::createAndCompileShader(int shaderType, const char* file) {
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

void Shader::linkShaderProgram() {
	for (int i = 0; i < shaders.size(); i++) {
		glAttachShader(id, shaders[i]);
	}
	glLinkProgram(id);
	GLint isFine, maxLength;
	char *errorLog;
	glGetProgramiv(id, GL_LINK_STATUS, (int *)&isFine);
	if (isFine == FALSE) {
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
		errorLog = new char[maxLength];
		glGetProgramInfoLog(id, maxLength, &maxLength, errorLog);
		fprintf(stderr, "An error occurred while linking the shader program. %s\n", errorLog);
		delete errorLog;
		exit(1);
	}
}
