#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// program的ID
	unsigned int ID;
	// 读取顶点和片元着色器的构造器
	Shader(const char* vertex_path, const char* fragment_path);
	Shader(const char* vertex_path, const char* geometry_path,const char* fragment_path);
	// 启动program
	void use();
	// 设置glsl中uniform变量的值
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setMat4f(const std::string& name, const glm::mat4& value) const;
	void setMat3f(const std::string& name, const glm::mat3& value) const;
	void setVec3f(const std::string& name, const glm::vec3& value) const;
	void setVec3f(const std::string& name, const float x, const float y, const float z) const;
	//void setSampler2D(const std::string& name, const )
	
};

#endif
