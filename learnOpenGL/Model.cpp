#include "Model.h"

void Model::draw(Shader& shader)
{
	for(unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].draw(shader);
	}
}
/**
 * @brief 读取模型
 * @param path 模型位置
 */
void Model::load_model(std::string path)
{
	std::cout << "loading model...\n";
	Assimp::Importer importer;
	/*
	 * aiProcess_Triangulate让importer 读取时将所有face转化为三角形
	 * aiProcess_FlipUVs 颠倒UV图
	 * aiProcess_GenNormals 如果模型没有法线，则自动生成法线
	 * aiProcess_SplitLargeMeshes 把大型的模型分成小模型的集合
	 * aiProcess_OptimizeMeshes 把小模型合并成大模型
	 */
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// 检查读取模型是否成功
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	std::cout << "the path is: " + path << std::endl;
	directory = path.substr(0, path.find_last_of('/'));
	std::cout << "the directory is: " + directory << std::endl;
	std::cout << "load model done\n";
	std::cout << "process the node\n\n";

	process_node(scene->mRootNode, scene);
	std::cout << "-------------------total info-------------------\n";
	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		std::cout << scene->mMaterials[i]->GetName().C_Str() << std::endl;
	}
	for (unsigned int i = 0; i < scene->mNumTextures; ++i)
	{
		std::cout << scene->mTextures[i]->mFilename.C_Str() << std::endl;
	}
	std::cout << "the texture has been loaded:" << texture_loaded.size() << std::endl;

	std::cout << "-------------------model info-------------------\n";
	std::cout << "mesh number: " << this->meshes.size() << std::endl;
	for (unsigned int i = 0; i < meshes.size(); ++i)
	{
		std::cout << "mesh" << i << " ";
		std::cout << "mesh vertex: " << this->meshes[i].vertices.size() << " ";
		std::cout << "mesh indices: " << this->meshes[i].indices.size() << " ";
		std::cout << "mesh textures: " << this->meshes[i].textures.size() << std::endl;
		for (unsigned int j = 0; j < meshes[i].textures.size(); ++j)
		{
			std::cout << meshes[i].textures[j].id << " " << meshes[i].textures[j].path << " " << meshes[i].textures[j].type << std::endl;
		}
	}
	std::cout << "------------------|model info|------------------\n";

}
/**
 * @brief 处理节点
 * @param node 节点
 * @param scene 总模型
 */
void Model::process_node(aiNode* node, const aiScene* scene)
{
	// 处理这个模型节点的网格
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
	}
	// 处理这个模型节点的子节点的网格
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		process_node(node->mChildren[i], scene);
	}
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	// 获得网格顶点
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// 转换顶点
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// 转换法线
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		// 转换材质坐标
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec;
		}
		else
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);
		// 入列这个顶点
		vertices.push_back(vertex);
	}
	// 获得网格面的顶点索引
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}
	// 获得贴图和高光贴图
	std::cout << "loading texture in one mesh...\n";
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	std::vector<Texture> diffuse_maps = load_material_texture(material, aiTextureType_DIFFUSE, "texture_diffuse");
	textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
	std::vector<Texture> specular_maps = load_material_texture(material, aiTextureType_SPECULAR, "texture_specular");
	textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
	std::cout << "loading texture in one mesh done\n";
	

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_texture(aiMaterial* material, aiTextureType type, std::string type_name)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(type); ++i)
	{
		aiString str;
		material->GetTexture(type, i, &str);
		//std::cout << "texture name:" << str.C_Str() << std::endl;

		bool skip = false;
		// 检查该贴图是否已经读取
		for (unsigned int j = 0; j < texture_loaded.size(); ++j)
		{
			if (std::strcmp(str.C_Str(), texture_loaded[j].path.data()) == 0)
			{
				textures.push_back(texture_loaded[j]);
				skip = true;
				break;
			}
		}
		// 未读取则照常读取
		if (!skip)
		{
			Texture texture;
			std::cout << "FAIL TO LOAD TEXTURE:" << str.C_Str() << std::endl;
			texture.id = load_texture(str.C_Str(), directory.c_str());
			texture.type = type_name;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texture_loaded.push_back(texture);
		}
	}
	return textures;
}


Model::Model(const char* path)
{
	load_model(path);
}
