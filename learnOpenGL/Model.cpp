#include "Model.h"

void Model::draw(Shader& shader)
{
	for(unsigned int i = 0; i < meshes.size(); ++i)
	{
		meshes[i].draw(shader);
	}
}
/**
 * @brief ��ȡģ��
 * @param path ģ��λ��
 */
void Model::load_model(std::string path)
{
	std::cout << "loading model...\n";
	Assimp::Importer importer;
	/*
	 * aiProcess_Triangulate��importer ��ȡʱ������faceת��Ϊ������
	 * aiProcess_FlipUVs �ߵ�UVͼ
	 * aiProcess_GenNormals ���ģ��û�з��ߣ����Զ����ɷ���
	 * aiProcess_SplitLargeMeshes �Ѵ��͵�ģ�ͷֳ�Сģ�͵ļ���
	 * aiProcess_OptimizeMeshes ��Сģ�ͺϲ��ɴ�ģ��
	 */
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// ����ȡģ���Ƿ�ɹ�
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
 * @brief ����ڵ�
 * @param node �ڵ�
 * @param scene ��ģ��
 */
void Model::process_node(aiNode* node, const aiScene* scene)
{
	// �������ģ�ͽڵ������
	for (unsigned int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh(mesh, scene));
	}
	// �������ģ�ͽڵ���ӽڵ������
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
	// ������񶥵�
	for (unsigned int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		// ת������
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		// ת������
		if (mesh->HasNormals())
		{
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.normal = vector;
		}
		// ת����������
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.tex_coords = vec;
		}
		else
			vertex.tex_coords = glm::vec2(0.0f, 0.0f);
		// �����������
		vertices.push_back(vertex);
	}
	// ���������Ķ�������
	for (unsigned int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; ++j)
			indices.push_back(face.mIndices[j]);
	}
	// �����ͼ�͸߹���ͼ
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
		// ������ͼ�Ƿ��Ѿ���ȡ
		for (unsigned int j = 0; j < texture_loaded.size(); ++j)
		{
			if (std::strcmp(str.C_Str(), texture_loaded[j].path.data()) == 0)
			{
				textures.push_back(texture_loaded[j]);
				skip = true;
				break;
			}
		}
		// δ��ȡ���ճ���ȡ
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
