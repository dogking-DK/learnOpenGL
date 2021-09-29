#include "camera.h"

camera::camera(const glm::vec3& pos, const glm::vec3& direct, const glm::vec3& up)
{
	this->camera_position = glm::normalize(pos);
	this->camera_direction = glm::normalize(direct);
	this->camera_up = glm::normalize(up);
}

glm::mat4 camera::view() const
{
	return glm::lookAt(camera_position, camera_position + camera_direction, camera_up);
}

void camera::process_cursor_motion(float x_offset, float y_offset)
{
	x_offset *= mouse_sensitivity;
	y_offset *= mouse_sensitivity;

	yaw += x_offset;
	pitch += y_offset;

	// 防止视角倒转
	if (pitch > 89.0f)
		pitch = 89.0;
	if (pitch < -89.0f)
		pitch = -89.0;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_direction = glm::normalize(direction);

	direction.x = -cos(glm::radians(yaw)) * sin(glm::radians(pitch));
	direction.y = cos(glm::radians(pitch));
	direction.z = -sin(glm::radians(yaw)) * sin(glm::radians(pitch));
	camera_up = direction;
}

void camera::process_key_press(GLFWwindow* window)
{
	// 调整相机位置
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera_position += camera_direction * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera_position -= camera_direction * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera_position += glm::normalize(glm::cross(camera_up, camera_direction)) * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera_position -= glm::normalize(glm::cross(camera_up, camera_direction)) * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		camera_position -= camera_up * camera_speed;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera_position += camera_up * camera_speed;
}

void camera::update_camera_speed(const float adjust)
{
	camera_speed = camera_basic_speed * adjust;
}

glm::vec3 camera::position() const
{
	return camera_position;
}