#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "stb_image.h"
#include <GLFW/glfw3.h>
inline unsigned int load_texture(const char* name, const char* directory);
inline unsigned int load_texture(const char* name);
inline unsigned int load_cubemap(const char* directory, const std::vector<std::string>& faces);

inline unsigned int load_texture(const char* name, const char* directory)
{
	std::string dir;
	if (directory != nullptr && directory[0])
	{
		dir = std::string(directory) + '/';
	}
	std::string file_name = dir + name;

	return load_texture(file_name.c_str());
	
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置贴图在s（即x轴水平方向上）重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// 设置贴图在t（即y轴垂直方向上）重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置贴图在放大缩小时如何采样，LINEAR->线性插值 NEAREST->选择最近像素
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true);			// 读取贴图是是否颠倒
	unsigned char* data = stbi_load(file_name.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4)							// RGBA类型
		{
			// 获得贴图
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			// 为贴图生成mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (nrChannels == 3)						// RGB类型
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}

	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);

	return texture;
}

inline unsigned int load_texture(const char* name)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// 设置贴图在s（即x轴水平方向上）重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// 设置贴图在t（即y轴垂直方向上）重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置贴图在放大缩小时如何采样，LINEAR->线性插值 NEAREST->选择最近像素
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(name, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 4)							// RGBA类型
		{
			// 获得贴图
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			// 为贴图生成mipmap
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else if (nrChannels == 3)						// RGB类型
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		
	}
	else
	{
		std::cout << "Failed to load texture" << name << std::endl;
	}
	stbi_image_free(data);

	return texture;
}

inline unsigned int load_cubemap(const char* directory, const std::vector<std::string>& faces)
{
	std::string dir;
	if (directory != nullptr && directory[0])
	{
		dir = std::string(directory) + '/';
	}
	unsigned int cubemap;
	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	int width, height, nrChannels;

	for (unsigned int i = 0; i < faces.size(); ++i)
	{
		unsigned char* data = stbi_load((dir + faces[i]).c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			if (nrChannels == 4)							// RGBA类型
			{
				// 获得贴图
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				// 为贴图生成mipmap
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else if (nrChannels == 3)						// RGB类型
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
		}
		else
		{
			std::cout << "Failed to load cubemap texture:" << directory << "/" << faces[i] << std::endl;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	return cubemap;
}

#endif
