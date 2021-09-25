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
	// program的ID
	unsigned int ID;
	// 读取顶点和片元着色器的构造器
	shader(const char* vertex_path, const char* fragment_path);

	// 启动program
	void use();
	// 设置glsl中uniform变量的值
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4f(const std::string& name, const glm::mat4& value) const;
	void setVec3f(const std::string& name, const glm::vec3& value) const;
	
};

#endif
