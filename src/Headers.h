#ifndef HEADERS_H
#define HEADERS_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "ft2build.h"

#define GLM_ENABLE_EXPERIMENTAL

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <vector>
#include <string>

#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif


#include <glm/gtx/rotate_vector.hpp>

const float WINDOW_WIDTH = 1280.0f, WINDOW_HEIGHT = 720.0f;

static glm::vec3 *createHorizontalTransformArray(int width, int length, glm::vec2 min, glm::vec2 max, float yPosition = 0.0f) {
	glm::vec3 *result = new glm::vec3[width * length];
	float firstX = width == 1 ? (max.x + min.x) / 2.0f : min.x;
	float xStep = width == 1 ? 0.0f : (max.x - min.x) / (float)(width - 1);
	float firstZ = length == 1 ? (max.y - min.y) / 2.0f : min.y;
	float zStep = length == 1 ? 0.0f : (max.y - min.y) / (float)(length - 1);
	int counter = 0;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < length; j++) {
			result[counter++] = glm::vec3(i*xStep + firstX, yPosition, j*zStep + firstZ);
		}
	}
	return result;
}

struct Texture {
	GLuint id;
	std::string path;
};

static Texture createTexture(char *textureFile) {
	Texture texture;
	int imgWidth, imgHeight, imgChannels;
	unsigned char *imgData = stbi_load(textureFile, &imgWidth, &imgHeight, &imgChannels, STBI_rgb_alpha);
	if (!imgData) {
		fprintf(stderr, "Failed to load texture from file \"%s\"!", textureFile);
		exit(1);
	}
	GLuint imgTexture;
	glGenTextures(1, &imgTexture);
	glBindTexture(GL_TEXTURE_2D, imgTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgWidth, imgHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData);
	texture.id = imgTexture;
	texture.path = textureFile;
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture;
}

static GLuint loadCubemap(std::vector<std::string> faces) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrComponents;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		} else {
			printf("Cubemap texture failed to load at path '%s'!\n", faces[i].c_str());
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

#define MAX_LIGHTS_OF_TYPE 16          // this MUST be identical to the value from the shader

#define DIRLIGHT_SIZE 4 * 4 * 4 + 4 * 4 * 4           // 4*vec4 + mat4

#define POINTLIGHT_SIZE 4 * 4 + 4 * 4 * 4 + 4 * 4 * 4  // 4*float + 4*vec4 + mat4

#define SPOTLIGHT_SIZE 5 * 4 + 5 * 4 * 4 + 4 * 4 * 4 + 4 * 3   // 5*float + 5*vec4 + mat4 + vec3

static double remap(double value, double sourceMin, double sourceMax, double targetMin, double targetMax,
	bool revertTarget = false, bool snapIfInvalid = true) {
	if (value < sourceMin || value > sourceMax) {
		if (snapIfInvalid) {
			return value < sourceMin ? targetMin : targetMax;
		}
	}
	double result = (value - sourceMin) / (sourceMax - sourceMin) * (targetMax - targetMin) + targetMin;
	if (revertTarget) {
		result = targetMax - result + targetMin;
	}

	return result;
}

struct DirLight {
	glm::vec4 direction;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::mat4 model;
};

struct PointLight {
	glm::vec4 position;
	float constant;
	float linear;
	float quadratic;
	float padding;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::mat4 model;
};

struct SpotLight {
	glm::vec4 position;
	glm::vec4 direction;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::mat4 model;
	glm::vec3 padding;
	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
};

#endif