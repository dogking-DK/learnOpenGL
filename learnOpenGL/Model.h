#ifndef MODEL_H
#define MODEL_H
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Shader.h"
#include "texture.h"
class Model
{
public:
	Model(const char* path);
	void draw(Shader& shader);

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texture_loaded;

	void load_model(std::string path);
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> load_material_texture(aiMaterial* material, aiTextureType type, std::string type_name);
};

#endif
