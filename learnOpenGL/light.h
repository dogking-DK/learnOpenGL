#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"
class Light
{
	
};
/**
 * 平行光
 */
class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : direction(dir), ambient(amb), diffuse(diff), specular(spec) {}
	void set_direction(glm::vec3 dir) { this->specular = dir; }					// 设置方向
	void set_ambient(glm::vec3 amb)   { this->specular = amb; }					// 设置环境光
	void set_diffuse(glm::vec3 diff)  { this->specular = diff; }				// 设置漫反射光
	void set_specular(glm::vec3 spec) { this->specular = spec; }				// 设置高光
	
	
	glm::vec3 direction;				// 光线方向

	glm::vec3 ambient;					// 环境反射强度
	glm::vec3 diffuse;					// 漫反射强度
	glm::vec3 specular;					// 高光强度
};
#endif