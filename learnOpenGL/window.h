#ifndef WINDOW_H
#define WINDOW_H
#include <GLFW/glfw3.h>

inline void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ��������
//inline void processInput(GLFWwindow *window)
//{
//	// �������Esc�����رմ���
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//	{
//		glfwSetWindowShouldClose(window, true);
//	}
//}

#endif