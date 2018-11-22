#ifndef MODEL_H
#define MODEL_H

#include "MeshModel.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

class Model {
public:
	Model(char *path);
	void Draw(Shader shader);
private:
	std::vector<ModelTexture> textures_loaded;
	std::vector<MeshModel> meshes;
	std::string directory;
	void loadModel(std::string const &path);
	void processNode(aiNode *node, const aiScene *scene);
	MeshModel processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<ModelTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char *path, const std::string &directory);
};

#endif