#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "Shader.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 tex_coords;
};

struct Texture
{
	unsigned int id;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	std::vector<Vertex> vertices;				// 顶点
	std::vector<unsigned int> indices;			// 面的顶点索引
	std::vector<Texture> textures;				// 材质

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader& shader);

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void setup_mesh();
};


#endif
