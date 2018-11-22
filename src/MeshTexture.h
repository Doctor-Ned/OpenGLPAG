#ifndef MESHTEXTURE_H
#define MESHTEXTURE_H

#include "Mesh.h"

struct TextureVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	std::string path;
};

class MeshTexture : public Mesh {
public:
	MeshTexture(std::vector<TextureVertex> vertices, std::vector<unsigned int> indices, char *textureFile);
	void Draw(Shader shader);
	static Texture createTexture(char *textureFile);
protected:
	MeshTexture();
	void setupMesh();
	Texture texture;
	std::vector<TextureVertex> vertices;
};

#endif