#ifndef CUBE_H
#define CUBE_H
#include "Mesh.h"
#include "texture.h"
class Cube 
{
public:
	void draw(Shader& shader);
	Cube();
	void move(glm::vec3 dis);
	void scale(const glm::vec3 scal);
	void rotate(const float angle, const glm::vec3 axis);
	void add_texture(const char* path, std::string type_name);
	glm::mat4 get_model() const;

private:
	Mesh mesh;
	glm::mat4 model_mat = glm::mat4(1.0f);
};


inline Cube::Cube()
{
	const std::vector <glm::vec3> vertex = { glm::vec3(-1.0f, -1.0f,  1.0f),
											 glm::vec3( 1.0f, -1.0f,  1.0f),
											 glm::vec3(-1.0f,  1.0f,  1.0f),
											 glm::vec3( 1.0f,  1.0f,  1.0f),
											 glm::vec3(-1.0f,  1.0f, -1.0f),
											 glm::vec3( 1.0f,  1.0f, -1.0f),
											 glm::vec3(-1.0f, -1.0f, -1.0f),
											 glm::vec3( 1.0f, -1.0f, -1.0f)};
	const std::vector <glm::vec2> texture = { glm::vec2(0.0f, 0.0f),
											  glm::vec2(1.0f, 0.0f),
											  glm::vec2(0.0f, 1.0f),
											  glm::vec2(1.0f, 1.0f)};
	const std::vector <glm::vec3> normal = { glm::vec3( 0.0f,  0.0f,  1.0f),
											 glm::vec3( 0.0f,  1.0f,  0.0f),
									   	     glm::vec3( 0.0f,  0.0f, -1.0f),
											 glm::vec3( 0.0f, -1.0f,  0.0f),
											 glm::vec3( 1.0f,  0.0f,  0.0f),
											 glm::vec3(-1.0f,  0.0f,  0.0f)};

	std::vector<Vertex> vertices;
	vertices.emplace_back(Vertex(vertex[0], normal[0], texture[0]));
	vertices.emplace_back(Vertex(vertex[1], normal[0], texture[1]));
	vertices.emplace_back(Vertex(vertex[2], normal[0], texture[2]));
	vertices.emplace_back(Vertex(vertex[3], normal[0], texture[3]));
			 
	vertices.emplace_back(Vertex(vertex[2], normal[1], texture[0]));
	vertices.emplace_back(Vertex(vertex[3], normal[1], texture[1]));
	vertices.emplace_back(Vertex(vertex[4], normal[1], texture[2]));
	vertices.emplace_back(Vertex(vertex[5], normal[1], texture[3]));
			 
	vertices.emplace_back(Vertex(vertex[4], normal[2], texture[3]));
	vertices.emplace_back(Vertex(vertex[5], normal[2], texture[2]));
	vertices.emplace_back(Vertex(vertex[6], normal[2], texture[1]));
	vertices.emplace_back(Vertex(vertex[7], normal[2], texture[0]));
	
	vertices.emplace_back(Vertex(vertex[6], normal[3], texture[0]));
	vertices.emplace_back(Vertex(vertex[7], normal[3], texture[1]));
	vertices.emplace_back(Vertex(vertex[0], normal[3], texture[2]));
	vertices.emplace_back(Vertex(vertex[1], normal[3], texture[3]));
			 
	vertices.emplace_back(Vertex(vertex[1], normal[4], texture[0]));
	vertices.emplace_back(Vertex(vertex[7], normal[4], texture[1]));
	vertices.emplace_back(Vertex(vertex[3], normal[4], texture[2]));
	vertices.emplace_back(Vertex(vertex[5], normal[4], texture[3]));
			 
	vertices.emplace_back(Vertex(vertex[6], normal[5], texture[0]));
	vertices.emplace_back(Vertex(vertex[0], normal[5], texture[1]));
	vertices.emplace_back(Vertex(vertex[4], normal[5], texture[2]));
	vertices.emplace_back(Vertex(vertex[2], normal[5], texture[3]));

	mesh.vertices = vertices;

	std::vector<unsigned int> indices = {  0,  1,  2,  2,  1,  3,
										   4,  5,  6,  6,  5,  7,
										   8,  9, 10, 10,  9, 11,
										  12, 13, 14, 14, 13, 15,
										  16, 17, 18, 18, 17, 19,
										  20, 21, 22, 22, 21, 23};
	mesh.indices = indices;
	mesh.setup_mesh();
}

inline void Cube::draw(Shader& shader)
{
	mesh.draw(shader);
}

// »√∑ΩøÈ“∆∂Ødistanceµƒæ‡¿Î
inline void Cube::move(glm::vec3 dis)
{
	model_mat = glm::translate(model_mat, dis);
}
inline void Cube::scale(const glm::vec3 scal)
{
	model_mat = glm::scale(model_mat, scal);
}
inline void Cube::rotate(const float angle, const glm::vec3 axis)
{
	model_mat = glm::rotate(model_mat, glm::radians(angle), axis);
}

inline void Cube::add_texture(const char* path, std::string type_name)
{
	unsigned int id = load_texture(path);
	Texture texture;
	texture.id = id;
	texture.path = path;
	texture.type = type_name;
	mesh.textures.push_back(texture);
	std::cout << "--------------cube texture address-----------------\n";
	for (unsigned int i = 0; i < mesh.textures.size(); ++i)
	{
		std::cout << "cube texture" + i << ": " << mesh.textures[i].id << std::endl;
	}
	std::cout << "--------------cube texture address-----------------\n";
}
inline glm::mat4 Cube::get_model() const
{
	return model_mat;
}

#endif