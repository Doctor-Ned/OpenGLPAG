#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

class Model {
public:
	Model(char *path);
	void Draw(GLuint shaderProgram);
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
private:
	void loadModel(std::string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char *path, const std::string &directory);
};

#endif