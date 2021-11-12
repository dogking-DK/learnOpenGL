#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

// 点光源衰减系数
static const float point_light_term[][3] = { {1.0f, 0.7000f, 1.800000f},			// 7（光照有效距离）
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
// 聚光灯照射范围系数（夹角的cosine）
static const float spot_light_term[][2] = { {0.9100f, 0.8200f},
											{0.9990f, 0.9500f} };

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
class PointLight
{
public:
	/**
	 * @param pos 光源位置
	 * @param amb 环境光强度
	 * @param diff 漫反射强度
	 * @param spec 高光强度
	 * @param type 衰减有效距离 0:7 1:13 2:20 3:32 4:50 5:65 6:100 7:160 8:200 9:325 10:600 11:3250
	 */
	PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, int type = 6) : position(pos), ambient(amb), diffuse(diff), specular(spec)
	{
		constant = point_light_term[type][0];
		linear = point_light_term[type][1];
		quadratic = point_light_term[type][2];
	}

	glm::vec3 position;					// 光源位置

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	/*
	 * 光线强度为二次函数的倒数，这三个对应二次的三个变量
	 */
	float constant;						// 常数项
	float linear;						// 一次项
	float quadratic;					// 二次项
};

class SpotLight
{
public:

	/**
	 * @param pos 光源位置
	 * @param dir 光线方向
	 * @param amb 环境光强度
	 * @param diff 漫反射强度
	 * @param spec 高光强度
	 * @param type 光照角度范围
	 */
	SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, int type = 0) : position(pos), direction(glm::normalize(dir)), ambient(amb), diffuse(diff), specular(spec)
	{
		cutoff = spot_light_term[type][0];
		outer_cutoff = spot_light_term[type][1];

	}


	glm::vec3 position;					// 光源位置
	glm::vec3 direction;				// 光线方向

	glm::vec3 ambient;					// 环境反射强度
	glm::vec3 diffuse;					// 漫反射强度
	glm::vec3 specular;					// 高光强度

	float cutoff;						// 内光圈角度大小（cutoff是cosine，所以越大，实际角度越小）
	float outer_cutoff;					// 外光圈角度大小（内光圈到外光圈，光强呈衰减）
};
#endif