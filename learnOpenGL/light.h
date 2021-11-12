#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

// ���Դ˥��ϵ��
static const float point_light_term[][3] = { {1.0f, 0.7000f, 1.800000f},			// 7��������Ч���룩
											 {1.0f, 0.3500f, 0.440000f},			// 13
											 {1.0f, 0.2200f, 0.200000f},			// 20
											 {1.0f, 0.1400f, 0.070000f},			// 32
											 {1.0f, 0.0900f, 0.032000f},			// 50
											 {1.0f, 0.0700f, 0.017000f},			// 65
											 {1.0f, 0.0450f, 0.007500f},			// 100
											 {1.0f, 0.0270f, 0.002800f},			// 160
											 {1.0f, 0.0220f, 0.001900f},			// 200
											 {1.0f, 0.0140f, 0.000700f},			// 325
											 {1.0f, 0.0070f, 0.000200f},			// 600
											 {1.0f, 0.0014f, 0.000007f} };			// 3250
// �۹�����䷶Χϵ�����нǵ�cosine��
static const float spot_light_term[][2] = { {0.9100f, 0.8200f},
											{0.9990f, 0.9500f} };

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
class PointLight
{
public:
	/**
	 * @param pos ��Դλ��
	 * @param amb ������ǿ��
	 * @param diff ������ǿ��
	 * @param spec �߹�ǿ��
	 * @param type ˥����Ч���� 0:7 1:13 2:20 3:32 4:50 5:65 6:100 7:160 8:200 9:325 10:600 11:3250
	 */
	PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, int type = 6) : position(pos), ambient(amb), diffuse(diff), specular(spec)
	{
		constant = point_light_term[type][0];
		linear = point_light_term[type][1];
		quadratic = point_light_term[type][2];
	}

	glm::vec3 position;					// ��Դλ��

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	/*
	 * ����ǿ��Ϊ���κ����ĵ�������������Ӧ���ε���������
	 */
	float constant;						// ������
	float linear;						// һ����
	float quadratic;					// ������
};

class SpotLight
{
public:

	/**
	 * @param pos ��Դλ��
	 * @param dir ���߷���
	 * @param amb ������ǿ��
	 * @param diff ������ǿ��
	 * @param spec �߹�ǿ��
	 * @param type ���սǶȷ�Χ
	 */
	SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, int type = 0) : position(pos), direction(glm::normalize(dir)), ambient(amb), diffuse(diff), specular(spec)
	{
		cutoff = spot_light_term[type][0];
		outer_cutoff = spot_light_term[type][1];

	}


	glm::vec3 position;					// ��Դλ��
	glm::vec3 direction;				// ���߷���

	glm::vec3 ambient;					// ��������ǿ��
	glm::vec3 diffuse;					// ������ǿ��
	glm::vec3 specular;					// �߹�ǿ��

	float cutoff;						// �ڹ�Ȧ�Ƕȴ�С��cutoff��cosine������Խ��ʵ�ʽǶ�ԽС��
	float outer_cutoff;					// ���Ȧ�Ƕȴ�С���ڹ�Ȧ�����Ȧ����ǿ��˥����
};
#endif