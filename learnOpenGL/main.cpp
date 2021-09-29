
#include "header_collection.h"
// 是否使用线框渲染模式
//#define __USE_LINE_MODE__

using std::cout;			using std::endl;
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

// 屏幕的宽高
const unsigned int screen_width = 800;
const unsigned int screen_height = 600;
// 光标初始位置
float last_x = 400, last_y = 300;

float fov = 45.0f;

// vertex buffer object
unsigned int VBO;
// vertex array object
unsigned int VAO;
unsigned int light_VAO;
// element buffer object
unsigned int EBO;

int fps = 0;
double second_start;
double second_end;
double last_time;
float vis_degree = 0.2f;
bool first_mouse = true;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 light_pos = glm::vec3(2.0f, 1.0f, 5.0f);
camera cam;

int main(int argc, char* argv[])
{
	// 初始化glfw
	glfwInit();
	// 设置使用的OpenGL最大最小版本都为3，也就是只是用3.X的版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 设置使用core model
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// 创建窗口
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "learnOpenGL", nullptr, nullptr);
	// 检查窗口是否创建成功
	if (window == nullptr)
	{
		cout << "fail to create window with glfw\n";
		glfwTerminate();
		return -1;
	}
	// 将此窗口设置为当前状态（当前的渲染显示目标）
	glfwMakeContextCurrent(window);
	// 设置画面resize时调用的函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// 设置在窗口中隐藏光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 设置滚轮
	glfwSetScrollCallback(window, scroll_callback);
	// 设置使用的光标参数处理函数
	glfwSetCursorPosCallback(window, mouse_callback);
	// 让GLAD获取本机的OpenGL信息并初始化
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		cout << "Failed to initialize GLAD\n";
		return -1;
	}
	
	
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	// --------------------------------------------------------------
	cout << "link basic shader...\n";
	shader ourShader("shader/vertex_shader.glsl", "shader/fragment_shader.glsl");
	cout << "link done\n";
	cout << "link light shader...\n";
	shader lightShader("shader/vertex_shader.glsl", "shader/light.glsl");
	cout << "link done\n";
	// ---------------------------------------------------------------
	// 设置顶点调用数组（vertex array object）
	glGenVertexArrays(1, &VAO);
	// 绑定VA
	glBindVertexArray(VAO);

	
	// 设置顶点缓存（vertex buffer object）
	glGenBuffers(1, &VBO);
	// 绑定VB
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 读取VB需要缓存的数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 设置顶点索引缓存
	//glGenBuffers(1, &EBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	// 设置VA指向BV的方式细节
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
	// 设置可以获取VA的信息
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glGenVertexArrays(1, &light_VAO);
	glBindVertexArray(light_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(0));
	glEnableVertexAttribArray(0);

	// --------------------------------------------------------
	
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// 设置贴图在s（即x轴水平方向上）重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	// 设置贴图在t（即y轴垂直方向上）重复
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// 设置贴图在放大缩小时如何采样，LINEAR->线性插值 NEAREST->选择最近像素
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("texture/container2.png", &width, &height, &nrChannels, 0);
	cout << width << "||" << height << "||" << nrChannels << endl;
	if (data)
	{
		// 获得贴图
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// 为贴图生成mipmap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture\n";
	}
	stbi_image_free(data);
	
	// --------------------------------------------------------

	glm::mat4 trans = glm::mat4(1.0f);

	// 设置渲染模式为线框模式
#ifdef __USE_LINE_MODE__
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glEnable(GL_DEPTH_TEST);
	
	ourShader.use();
	ourShader.setFloat("vis_degree", vis_degree);
	ourShader.setVec3f("object_color", glm::vec3(1.0f, 0.5f, 0.31f));
	ourShader.setVec3f("light_color", glm::vec3(1.0f, 1.0f, 1.0f));
	ourShader.setVec3f("light_pos", light_pos);
	
	
	
	// 位移矩阵
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	// 透视投影矩阵
	glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(screen_width) / static_cast<float>(screen_height), 0.1f, 100.0f);

	second_start = glfwGetTime();
	// ---------------------------------------------------------
	// 不停地渲染画面，监听事件
	while (!glfwWindowShouldClose(window))
	{
		// 检测输入
		processInput(window);

		// 进行渲染更新
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::vec3 light_vec = glm::vec3(1.0f);
		//light_vec.x = sin(glfwGetTime() * 2.0f) + 0.5;
		//light_vec.y = sin(glfwGetTime() * 0.7f) + 0.5;
		//light_vec.z = sin(glfwGetTime() * 1.3f) + 0.5;
		light_vec.x = 1.0f;
		light_vec.y = 1.0f;
		light_vec.z = 1.0f;
		glm::vec3 diffuseColor = light_vec * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence
		glm::vec3 specularColor = glm::vec3(1.0f, 1.0f, 1.0f); // low influence
		
		lightShader.use();
		model = glm::mat4(1.0);
		//model = glm::rotate(model, glm::radians(static_cast<float>(glfwGetTime() * 100.0)), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
		model = glm::translate(model, light_pos);
		glm::mat4 temp = model;
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4f("transform", trans);
		lightShader.setMat4f("model", model);
		lightShader.setMat4f("view", cam.view());
		lightShader.setMat4f("projection", projection);
		lightShader.setVec3f("light.ambient", diffuseColor);
		lightShader.setVec3f("light.diffuse", ambientColor);
		lightShader.setVec3f("light.spectral", specularColor);
		glBindVertexArray(light_VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		
		glBindVertexArray(VAO);
		ourShader.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		ourShader.setVec3f("light.position", glm::vec3(temp * glm::vec4(light_pos, 0.0f)));
		model = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), static_cast<float>(screen_width) / static_cast<float>(screen_height), 0.1f, 100.0f);
		ourShader.setMat4f("transform", trans);
		ourShader.setMat4f("model", model);
		glm::mat3 normal_model = glm::mat3(glm::transpose(glm::inverse(model)));
		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			cout << endl;
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					cout << normal_model[i][j] << " ";
				}
				cout << endl;
			}
		}
		ourShader.setMat3f("normal_model", glm::mat3(model));
		ourShader.setMat4f("view", cam.view());
		ourShader.setMat4f("projection", projection);
		ourShader.setVec3f("cam_pos", cam.position());

		ourShader.setVec3f("light.ambient", ambientColor);
		ourShader.setVec3f("light.diffuse", diffuseColor);
		ourShader.setVec3f("light.specular", specularColor);


		ourShader.setVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
		ourShader.setVec3f("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
		ourShader.setFloat("material.shininess", 32.0f);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		
		
		// 更新画面，处理事件	
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		ourShader.setFloat("vis_degree", vis_degree);

		// 测量FPS
		last_time = second_end;
		second_end = glfwGetTime();
		++fps;
		if(second_end - second_start >= 1.0)
		{
			glfwSetWindowTitle(window, std::to_string(fps).c_str());
			fps = 0;
			second_start += 1.0;
		}
		cam.update_camera_speed(static_cast<float>(second_end - last_time));
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &light_VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(ourShader.ID);
	glDeleteProgram(lightShader.ID);
	
	// 终止窗口
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	// 如果按下Esc键，关闭窗口
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// 调整材质可见度
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
	
	cam.process_key_press(window);
}

void mouse_callback(GLFWwindow* window, double x_pos, double y_pos)
{
	if (first_mouse)
	{
		last_x = x_pos;
		last_y = y_pos;
		first_mouse = false;
	}
	
	float x_offset = x_pos - last_x;
	float y_offset = last_y - y_pos;
	last_x = x_pos;
	last_y = y_pos;


	
	cam.process_cursor_motion(x_offset, y_offset);
}

void scroll_callback(GLFWwindow* window, double x_offset, double y_offset)
{
	fov -= y_offset;

	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 60.0f)
		fov = 60.0f;
}