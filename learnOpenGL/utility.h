#ifndef UTILITY_H
#define UTILITY_H

#include <iomanip>
#include <iostream>

#include "glm/glm.hpp"

inline void print_mat4(const glm::mat4& mat)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		for (unsigned int j = 0; j < 4; ++j)
		{
			std::cout << std::setw(5) << std::setprecision(3) << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
inline void print_mat3(const glm::mat3& mat)
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		for (unsigned int j = 0; j < 3; ++j)
		{
			std::cout << std::setw(5) << std::setprecision(3) << mat[i][j] << " ";
		}
		std::cout << std::endl;
	}
}
inline void print_vec(const glm::vec3& vec)
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		std::cout << std::setw(5) << std::setprecision(3) << vec[i] << " ";
	}
	std::cout << std::endl;
}
inline void print_vec(const glm::vec4& vec)
{
	for (unsigned int i = 0; i < 4; ++i)
	{
		std::cout << std::setw(5) << std::setprecision(3) << vec[i] << " ";
	}
	std::cout << std::endl;
}
#endif
