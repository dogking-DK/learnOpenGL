#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class camera
{
public:

	// 默认相机位于（0，0，3），面向-z，竖直上方向为y
	camera(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 3.0f), const glm::vec3& direct = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 view() const;															// 获得view矩阵
	glm::vec3 position() const;														// 获得相机位置
	void process_cursor_motion(float x_offset, float y_offset);						// 通过鼠标移动更新镜头方向
	void process_key_press(GLFWwindow* window);										// 通过键盘输入更新镜头位置移动
	void update_camera_speed(const float adjust);									// 调整相机移速，平衡因帧数带来的不稳定
	glm::mat4 back_view() const;													// 获得背后的相机view矩阵
private:
	glm::vec3 camera_position = glm::vec3(1.0f);								// 相机的位置
	glm::vec3 camera_direction = glm::vec3(1.0f);								// 相机的方向
	glm::vec3 camera_up = glm::vec3(1.0f);										// 相机的上方
	static constexpr float mouse_sensitivity = 0.1f;								// 鼠标灵敏度
	static constexpr float camera_basic_speed = 5.0f;								// 相机的绝对移速（不因帧率改变）
	float camera_speed = 0.5f;														// 相机的相对移速（平衡帧率后的速度）
	float yaw = -90.0f;																// 偏航角（xz平面）
	float pitch = 0.0f;																// 俯仰角（yz平面）
	
};

#endif
