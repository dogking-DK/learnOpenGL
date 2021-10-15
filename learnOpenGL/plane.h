#ifndef PLANE_H
#define PLANE_H
#include "Mesh.h"
#include "texture.h"
class Plane
{
public:
	void draw(Shader& shader);
	Plane();
	void move(const glm::vec3 dis);
	void scale(const glm::vec3 scal);
	void rotate(const float angle, const glm::vec3 axis);
	void add_texture(const char* path, const std::string& type_name);
	void add_texture(const unsigned int tex, const std::string& type_name);
	glm::mat4 get_model() const;
	void model_vertices();
private:
	Mesh mesh;
	//glm::vec3 distance = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 model_mat = glm::mat4(1.0f);
};


inline Plane::Plane()
{
	const std::vector <glm::vec3> vertex = { glm::vec3(-1.0f,  0.0f,  -1.0f),
											 glm::vec3(-1.0f,  0.0f,   1.0f),
											 glm::vec3( 1.0f,  0.0f,   1.0f),
											 glm::vec3( 1.0f,  0.0f,  -1.0f)};
	const std::vector <glm::vec2> texture = { glm::vec2(0.0f, 0.0f),
											  glm::vec2(0.0f, 1.0f),
											  glm::vec2(1.0f, 1.0f),
											  glm::vec2(1.0f, 0.0f) };
	const std::vector <glm::vec3> normal = { glm::vec3(0.0f,  1.0f,  0.0f) };

	std::vector<Vertex> vertices;
	vertices.emplace_back(Vertex(vertex[0], normal[0], texture[0]));
	vertices.emplace_back(Vertex(vertex[1], normal[0], texture[1]));
	vertices.emplace_back(Vertex(vertex[2], normal[0], texture[2]));
	vertices.emplace_back(Vertex(vertex[3], normal[0], texture[3]));

	mesh.vertices = vertices;

	std::vector<unsigned int> indices = { 0, 1, 3, 3, 1, 2 };
	mesh.indices = indices;
	mesh.setup_mesh();
}

inline void Plane::draw(Shader& shader)
{
	mesh.draw(shader);
}

// 让平面移动distance的距离
inline void Plane::move(const glm::vec3 dis)
{
	model_mat = glm::translate(model_mat, dis);
}
// 缩放平面
inline void Plane::scale(const glm::vec3 scal)
{
	model_mat = glm::scale(model_mat, scal);
}
// 旋转平面
inline void Plane::rotate(const float angle, const glm::vec3 axis)
{
	model_mat = glm::rotate(model_mat, glm::radians(angle), axis);
}

inline glm::mat4 Plane::get_model() const
{
	return model_mat;
}

inline void Plane::add_texture(const char* path, const std::string& type_name)
{
	const unsigned int id = load_texture(path);
	Texture texture;
	texture.id = id;
	texture.path = path;
	texture.type = type_name;
	mesh.textures.push_back(texture);
}
inline void Plane::add_texture(const unsigned int tex, const std::string& type_name)
{
	Texture texture;
	texture.id = tex;
	texture.type = type_name;
	mesh.textures.push_back(texture);
}
inline void Plane::model_vertices()
{
	for ( Vertex vec : mesh.vertices)
	{
		vec.position = glm::vec3(model_mat * glm::vec4(vec.position, 1.0f));
	}
	model_mat = glm::mat4(1.0f);
}

#endif