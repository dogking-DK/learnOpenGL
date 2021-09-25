#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class shader
{
public:
	// program��ID
	unsigned int ID;
	// ��ȡ�����ƬԪ��ɫ���Ĺ�����
	shader(const char* vertex_path, const char* fragment_path);

	// ����program
	void use();
	// ����glsl��uniform������ֵ
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4f(const std::string& name, const glm::mat4& value) const;
	void setVec3f(const std::string& name, const glm::vec3& value) const;
	
};

#endif
