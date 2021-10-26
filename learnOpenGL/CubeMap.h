#ifndef CUBE_MAP_H
#define CUBE_MAP_H
#include "Mesh.h"
#include "texture.h"
class CubeMap
{
public:
	void draw(Shader& shader) const;
	CubeMap();
	void move(glm::vec3 dis);
	void scale(const glm::vec3 scal);
	void rotate(const float angle, const glm::vec3 axis);
	void load_tex(const char* directory, const std::vector<std::string>& faces);
	glm::mat4 get_model() const;

private:
	std::vector <glm::vec3> vertex_coord;
	std::vector <glm::vec2> texture_coord;
	std::vector <glm::vec3> normal_coord;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int texture = 0;								//
	glm::mat4 model_mat = glm::mat4(1.0f);
	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
};


inline CubeMap::CubeMap()
{
	vertex_coord = { glm::vec3(-1.0f, -1.0f,  1.0f),
					 glm::vec3( 1.0f, -1.0f,  1.0f),
					 glm::vec3(-1.0f,  1.0f,  1.0f),
					 glm::vec3( 1.0f,  1.0f,  1.0f),
					 glm::vec3(-1.0f,  1.0f, -1.0f),
					 glm::vec3( 1.0f,  1.0f, -1.0f),
					 glm::vec3(-1.0f, -1.0f, -1.0f),
					 glm::vec3( 1.0f, -1.0f, -1.0f) };
	texture_coord = { glm::vec2(0.0f,  0.0f),
					  glm::vec2(1.0f,  0.0f),
					  glm::vec2(0.0f,  1.0f),
					  glm::vec2(1.0f,  1.0f) };
	normal_coord = { glm::vec3( 0.0f,  0.0f,  1.0f),
					 glm::vec3( 0.0f,  1.0f,  0.0f),
					 glm::vec3( 0.0f,  0.0f, -1.0f),
					 glm::vec3( 0.0f, -1.0f,  0.0f),
					 glm::vec3( 1.0f,  0.0f,  0.0f),
					 glm::vec3(-1.0f,  0.0f,  0.0f) };

	vertices.emplace_back(Vertex(vertex_coord[0], normal_coord[0], texture_coord[0]));
	vertices.emplace_back(Vertex(vertex_coord[1], normal_coord[0], texture_coord[1]));
	vertices.emplace_back(Vertex(vertex_coord[2], normal_coord[0], texture_coord[2]));
	vertices.emplace_back(Vertex(vertex_coord[3], normal_coord[0], texture_coord[3]));

	vertices.emplace_back(Vertex(vertex_coord[2], normal_coord[1], texture_coord[0]));
	vertices.emplace_back(Vertex(vertex_coord[3], normal_coord[1], texture_coord[1]));
	vertices.emplace_back(Vertex(vertex_coord[4], normal_coord[1], texture_coord[2]));
	vertices.emplace_back(Vertex(vertex_coord[5], normal_coord[1], texture_coord[3]));
	
	vertices.emplace_back(Vertex(vertex_coord[4], normal_coord[2], texture_coord[3]));
	vertices.emplace_back(Vertex(vertex_coord[5], normal_coord[2], texture_coord[2]));
	vertices.emplace_back(Vertex(vertex_coord[6], normal_coord[2], texture_coord[1]));
	vertices.emplace_back(Vertex(vertex_coord[7], normal_coord[2], texture_coord[0]));

	vertices.emplace_back(Vertex(vertex_coord[6], normal_coord[3], texture_coord[0]));
	vertices.emplace_back(Vertex(vertex_coord[7], normal_coord[3], texture_coord[1]));
	vertices.emplace_back(Vertex(vertex_coord[0], normal_coord[3], texture_coord[2]));
	vertices.emplace_back(Vertex(vertex_coord[1], normal_coord[3], texture_coord[3]));

	vertices.emplace_back(Vertex(vertex_coord[1], normal_coord[4], texture_coord[0]));
	vertices.emplace_back(Vertex(vertex_coord[7], normal_coord[4], texture_coord[1]));
	vertices.emplace_back(Vertex(vertex_coord[3], normal_coord[4], texture_coord[2]));
	vertices.emplace_back(Vertex(vertex_coord[5], normal_coord[4], texture_coord[3]));

	vertices.emplace_back(Vertex(vertex_coord[6], normal_coord[5], texture_coord[0]));
	vertices.emplace_back(Vertex(vertex_coord[0], normal_coord[5], texture_coord[1]));
	vertices.emplace_back(Vertex(vertex_coord[4], normal_coord[5], texture_coord[2]));
	vertices.emplace_back(Vertex(vertex_coord[2], normal_coord[5], texture_coord[3]));

	indices = { 0,  1,  2,  2,  1,  3,
			    4,  5,  6,  6,  5,  7,
			    8,  9, 10, 10,  9, 11,
			   12, 13, 14, 14, 13, 15,
			   16, 17, 18, 18, 17, 19,
			   20, 21, 22, 22, 21, 23 };

	glGenVertexArrays(1, &VAO);						// 设置顶点调用数组（Vertex array object）
	glGenBuffers(1, &VBO);							// 设置顶点缓存（Vertex buffer object）
	glGenBuffers(1, &EBO);							// 设置顶点索引缓存（element buffer object）

	glBindVertexArray(VAO);							// 绑定VA
	glBindBuffer(GL_ARRAY_BUFFER, VBO);				// 绑定VB
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);		// 绑定EB

	// 获取VB和EB的数据
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

	// 设置顶点
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);
	// 设置法线
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	// 设置材质
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tex_coords)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

inline void CubeMap::draw(Shader& shader) const
{
	// 使天空盒渲染在未着色的像素上
	glDepthFunc(GL_LEQUAL);
	// 绑定cubemap
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	// 绑定VAO
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}

// 让方块移动distance的距离
inline void CubeMap::move(glm::vec3 dis)
{
	model_mat = glm::translate(model_mat, dis);
}
inline void CubeMap::scale(const glm::vec3 scal)
{
	model_mat = glm::scale(model_mat, scal);
}
inline void CubeMap::rotate(const float angle, const glm::vec3 axis)
{
	model_mat = glm::rotate(model_mat, glm::radians(angle), axis);
}


inline glm::mat4 CubeMap::get_model() const
{
	return model_mat;
}
inline void CubeMap::load_tex(const char* directory, const std::vector<std::string>& faces)
{
	texture = load_cubemap(directory, faces);
	std::cout << "cubemap:" << texture << std::endl;
}

#endif