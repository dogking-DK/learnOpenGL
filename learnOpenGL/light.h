#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"
class Light
{
	
};
/**
 * ƽ�й�
 */
class DirectionalLight
{
public:
	DirectionalLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : direction(dir), ambient(amb), diffuse(diff), specular(spec) {}
	void set_direction(glm::vec3 dir) { this->specular = dir; }					// ���÷���
	void set_ambient(glm::vec3 amb)   { this->specular = amb; }					// ���û�����
	void set_diffuse(glm::vec3 diff)  { this->specular = diff; }				// �����������
	void set_specular(glm::vec3 spec) { this->specular = spec; }				// ���ø߹�
	
	
	glm::vec3 direction;				// ���߷���

	glm::vec3 ambient;					// ��������ǿ��
	glm::vec3 diffuse;					// ������ǿ��
	glm::vec3 specular;					// �߹�ǿ��
};
#endif