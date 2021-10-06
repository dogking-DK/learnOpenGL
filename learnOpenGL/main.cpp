
#include "header_collection.h"
#include "include/glad/glad.h"
// 是否使用线框渲染模式
//#define __USE_LINE_MODE__

using std::cout;			using std::endl;
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
GLFWwindow* init();

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
	GLFWwindow* window = init();
	if (window == nullptr)
	{
		return -1;
	}

	// --------------------------------------------------------------
	cout << "link light shader...\n";
	Shader lightShader("shader/vertex_shader.glsl", "shader/light.glsl");
	cout << "link done\n";
	cout << "link temp shader...\n";
	Shader tempShader("shader/model_vs.glsl", "shader/model_fs.glsl");
	cout << "link done\n";
	
	unsigned int texture1, texture2, texture3;
	texture1 = load_texture("model/pack/diffuse.jpg");
	texture2 = load_texture("texture/container_spec.png");
	texture3 = load_texture("texture/matrix.jpg");

	
	// --------------------------------------------------------

	glm::mat4 trans = glm::mat4(1.0f);


	
	//Model temp_model("model/pack/backpack.obj");
	Model temp_model("model/pack/backpack.obj");

	
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
		
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		glm::vec3 light_vec = glm::vec3(1.0f);
		light_vec.x = 1.0f;
		light_vec.y = 1.0f;
		light_vec.z = 1.0f;
		glm::vec3 diffuseColor = light_vec * glm::vec3(0.5f); // decrease the influence
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.5f); // low influence
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

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, texture3);
		glBindVertexArray(VAO);

		model = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(fov), static_cast<float>(screen_width) / static_cast<float>(screen_height), 0.1f, 100.0f);
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

		tempShader.use();
		tempShader.setMat4f("model", model);
		tempShader.setMat4f("view", cam.view());
		tempShader.setMat4f("projection", projection);
		tempShader.setVec3f("cam_pos", light_pos);

		temp_model.draw(tempShader);

		
		// 更新画面，处理事件	
		glfwSwapBuffers(window);
		glfwPollEvents();
		
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

GLFWwindow* init()
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
		return nullptr;
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
		return nullptr;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
	
	// 设置渲染模式为线框模式
#ifdef __USE_LINE_MODE__
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glEnable(GL_DEPTH_TEST);						// 是否开启深度测试
	glDepthMask(GL_TRUE);							// 开启深度测试时，是否覆盖深度缓存，默认GL_TRUE
	/*
	 * GL_ALWAYS			一直传递深度结果
	 * GL_NEVER				从不传递深度结果
	 * GL_LESS				结果小于储存值时传递，默认情况下是GL_LESS，所以cam是朝向z轴正方向
	 * GL_EQUAL				结果等于储存值时传递
	 * GL_LEQUAL			结果小于等于储存值时传递
	 * GL_GREATER			结果大于储存值时传递
	 * GL_NOTEQUAL			结果不等于储存值时传递
	 * GL_GEQUAL			结果大于等于储存值时传递
	 */
	glDepthFunc(GL_LESS);							// 深度测试方式，默认GL_LESS

	glEnable(GL_STENCIL_TEST);						// 是否开启模板缓存
	/*
	 * 这个函数用于对每个stencil取“与”，一般要么全部与0要么与1
	 * 0xFF					正常写入，与1相当于没变
	 * 0x00					全部清零，与0全部变0
	 */
	glStencilMask(0xFF);							// 开启模板测试时，是否对模板覆盖，默认0xFF
	/*
	 * func						比较方式
	 * ref						比较对象
	 * mask						覆盖方式
	 */
	glStencilFunc(GL_EQUAL, 1, 0xFF);
	/*
	 * sfail					stencil未通过的处理方式
	 * dpfail					stencil通过而depth未通过的处理方式
	 * dppass					stencil和depth都通过后的处理方式
	 *
	 * 方式：
	 * GL_KEEP					保持当前值
	 * GL_ZERO					设为0
	 * GL_REPLACE				替换为glStencilFunc中ref的值
	 * GL_INCR					小于最大值时+1
	 * GL_INCR_WRAP				小于最大值时+1，大于最大值时设回为0
	 * GL_DECR					大于最小值时-1
	 * GL_DECR_WRAP				大于最小值时-1，小于最小值时设回为最大值
	 * GL_INVERT				对每个位取反
	 */
	glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
	return window;
}