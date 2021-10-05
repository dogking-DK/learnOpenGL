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
	glGenVertexArrays(1, &VAO);						// ���ö���������飨Vertex array object��
	glGenBuffers(1, &VBO);							// ���ö��㻺�棨Vertex buffer object��
	glGenBuffers(1, &EBO);							// ���ö����������棨element buffer object��

	glBindVertexArray(VAO);							// ��VA
	glBindBuffer(GL_ARRAY_BUFFER, VBO);				// ��VB
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);		// ��EB

	// ��ȡVB��EB������
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

	// ���ö���
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);				    	   
	// ���÷���
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glEnableVertexAttribArray(1);
	// ���ò���
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

		std::string number;								// ���
		std::string name = textures[i].type;			// ����
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
	// �������õ���ͼ
	glActiveTexture(GL_TEXTURE0);

	// ��ͼ
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
