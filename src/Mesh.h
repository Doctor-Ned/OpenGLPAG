#ifndef MESH_H
#define MESH_H

#include "Util.h"
#include <string>
#include <vector>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

struct Texture {
	GLuint id;
	std::string type;
	std::string path;
};

class Mesh {
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(GLuint shaderProgram);
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	GLuint VAO;
	GLuint VBO, EBO;
	void setupMesh();
};

#endif