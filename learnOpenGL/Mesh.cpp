#include "Mesh.h"


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setup_mesh();
}

void Mesh::setup_mesh()
{
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

void Mesh::draw(Shader& shader)
{
	unsigned int diffuse_num = 1;
	unsigned int specular_num = 1;

	for (unsigned int i = 0; i < textures.size(); ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;								// 序号
		std::string name = textures[i].type;			// 类型
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuse_num++);
		}
		else if (name == "specular_diffuse")
		{
			number = std::to_string(specular_num++);
		}

		shader.setInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	// 重置启用的贴图
	glActiveTexture(GL_TEXTURE0);

	// 绘图
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
