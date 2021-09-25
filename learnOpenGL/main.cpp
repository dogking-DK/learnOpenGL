
#include "header_collection.h"
// �Ƿ�ʹ���߿���Ⱦģʽ
//#define __USE_LINE_MODE__

using std::cout;			using std::endl;

float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
glm::vec3 cubePositions[] = {
	   glm::vec3(0.0f,  0.0f,  0.0f),
	   glm::vec3(2.0f,  5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(1.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f)
};
unsigned int indices[] = {
	// note that we start from 0!
	0, 1, 3, // first triangle
	1, 2, 3, // second triangle
};

// ��Ļ�Ŀ��
const unsigned int screen_width = 800;
const unsigned int screen_height = 600;

// vertex buffer object
unsigned int VBO;
// vertex array object
unsigned int VAO;
// element buffer object
unsigned int EBO;

int fps = 0;
double second_start;
double second_end;
float vis_degree = 0.2;

int main(int argc, char* argv[])
{
	// ��ʼ��glfw
	glfwInit();
	// ����ʹ�õ�OpenGL�����С�汾��Ϊ3��Ҳ����ֻ����3.X�İ汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// ����ʹ��core model
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ��������
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "learnOpenGL", nullptr, nullptr);
	// ��鴰���Ƿ񴴽��ɹ�
	if (window == nullptr)
	{
		cout << "fail to create window with glfw\n";
		glfwTerminate();
		return -1;
	}
	// ���˴�������Ϊ��ǰ״̬����ǰ����Ⱦ��ʾĿ�꣩
	glfwMakeContextCurrent(window);
	// ���û���resizeʱ���õĺ���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// ��GLAD��ȡ������OpenGL��Ϣ����ʼ��
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		cout << "Failed to initialize GLAD\n";
		return -1;
	}

	
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// --------------------------------------------------------------

	shader ourShader("shader/vertex_shader.glsl", "shader/fragment_shader.glsl");
	
	// ---------------------------------------------------------------
	// ���ö���������飨vertex array object��
	glGenVertexArrays(1, &VAO);
	// ��VA
	glBindVertexArray(VAO);

	// ���ö��㻺�棨vertex buffer object��
	glGenBuffers(1, &VBO);
	// ��VB
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ��ȡVB��Ҫ���������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ���ö�����������
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// ����VAָ��BV�ķ�ʽϸ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	// ���ÿ��Ի�ȡVA����Ϣ
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// --------------------------------------------------------

	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// ������ͼ��s����x��ˮƽ�����ϣ��ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// ������ͼ��t����y�ᴹֱ�����ϣ��ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ������ͼ�ڷŴ���Сʱ��β�����LINEAR->���Բ�ֵ NEAREST->ѡ���������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("texture/container.jpg", &width, &height, &nrChannels, 0);
	cout << width << "||" << height << "||" << nrChannels << endl;
	if (data)
	{
		// �����ͼ
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// Ϊ��ͼ����mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);

	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// ������ͼ��s����x��ˮƽ�����ϣ��ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// ������ͼ��t����y�ᴹֱ�����ϣ��ظ�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// ������ͼ�ڷŴ���Сʱ��β�����LINEAR->���Բ�ֵ NEAREST->ѡ���������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("texture/awesomeface.png", &width, &height, &nrChannels, 0);
	stbi_set_flip_vertically_on_load(false);
	cout << width << "||" << height << "||" << nrChannels << endl;
	if (data)
	{
		// �����ͼ
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// Ϊ��ͼ����mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);
	
	// --------------------------------------------------------

	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 trans2 = glm::mat4(1.0f);

	// ������ȾģʽΪ�߿�ģʽ
#ifdef __USE_LINE_MODE__
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glEnable(GL_DEPTH_TEST);
	
	ourShader.use();
	ourShader.setInt("texture1", 0);
	ourShader.setInt("texture2", 1);
	ourShader.setFloat("vis_degree", vis_degree);

	
	// λ�ƾ���
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	// ��ͼ����
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0));
	// ͸��ͶӰ����
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(screen_width) / static_cast<float>(screen_height), 0.1f, 100.0f);

	
	second_start = glfwGetTime();
	// ---------------------------------------------------------
	// ��ͣ����Ⱦ���棬�����¼�
	while (!glfwWindowShouldClose(window))
	{
		// �������
		processInput(window);

		// ������Ⱦ����
		
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glBindVertexArray(VAO);
		
		ourShader.use();

		trans = glm::rotate(trans, glm::radians(0.1f), glm::vec3(0.0, 0.0, 1.0));
		ourShader.setMat4f("transform", trans);
		ourShader.setMat4f("model", model);
		ourShader.setMat4f("view", view);
		ourShader.setMat4f("projection", projection);

		for(unsigned int i = 0; i < 10; ++i)
		{
			ourShader.setMat4f("translate", glm::translate(glm::mat4(1.0f), cubePositions[i]));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		
		
		// ���»��棬�����¼�	
		glfwSwapBuffers(window);
		glfwPollEvents();
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		{
			vis_degree += 0.001f;
			if (vis_degree > 1.0f)
				vis_degree = 1.0f;
		}
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		{
			vis_degree -= 0.001f;
			if (vis_degree < 0.0f)
				vis_degree = 0.0f;
		}
		ourShader.setFloat("vis_degree", vis_degree);

		// ����S
		second_end = glfwGetTime();
		++fps;
		if(second_end - second_start >= 1.0)
		{
			glfwSetWindowTitle(window, std::to_string(fps).c_str());
			fps = 0;
			second_start += 1.0;
		}
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(ourShader.ID);
	
	// ��ֹ����
	glfwTerminate();
	return 0;
}
