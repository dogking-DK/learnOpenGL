#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class camera
{
public:

	// Ĭ�����λ�ڣ�0��0��3��������-z����ֱ�Ϸ���Ϊy
	camera(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 3.0f), const glm::vec3& direct = glm::vec3(0.0f, 0.0f, -1.0f), const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 view() const;															// ���view����
	glm::vec3 position() const;														// ������λ��
	void process_cursor_motion(float x_offset, float y_offset);						// ͨ������ƶ����¾�ͷ����
	void process_key_press(GLFWwindow* window);										// ͨ������������¾�ͷλ���ƶ�
	void update_camera_speed(const float adjust);									// ����������٣�ƽ����֡�������Ĳ��ȶ�
	glm::mat4 back_view() const;													// ��ñ�������view����
private:
	glm::vec3 camera_position = glm::vec3(1.0f);								// �����λ��
	glm::vec3 camera_direction = glm::vec3(1.0f);								// ����ķ���
	glm::vec3 camera_up = glm::vec3(1.0f);										// ������Ϸ�
	static constexpr float mouse_sensitivity = 0.1f;								// ���������
	static constexpr float camera_basic_speed = 5.0f;								// ����ľ������٣�����֡�ʸı䣩
	float camera_speed = 0.5f;														// �����������٣�ƽ��֡�ʺ���ٶȣ�
	float yaw = -90.0f;																// ƫ���ǣ�xzƽ�棩
	float pitch = 0.0f;																// �����ǣ�yzƽ�棩
	
};

#endif
