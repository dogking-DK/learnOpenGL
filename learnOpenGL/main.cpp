#include "header_collection.h"
// �Ƿ�ʹ���߿���Ⱦģʽ
//#define __USE_LINE_MODE__

using std::cout;			using std::endl;
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow* window, double x_offset, double y_offset);
GLFWwindow* init();

// ��Ļ�Ŀ��
const unsigned int screen_width = 800;
const unsigned int screen_height = 600;
// ����ʼλ��
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
	Shader colorShader("shader/model_vs.glsl", "shader/shader_single_color.fs");
	Shader screen_shader("shader/frame.vs", "shader/frame.fs");
	Shader skybox_shader("shader/skybox.vs", "shader/skybox.fs");
	
	// --------------------------------------------------------

	glm::mat4 trans = glm::mat4(1.0f);

	//Model temp_model("model/pack/backpack.obj");
	//Model temp_model("model/mug/mug.obj");
	
	Cube cube;
	cube.add_texture("texture/matrix.jpg", "diffuse_texture");
	Plane plane;
	plane.add_texture("texture/awesomeface.png", "diffuse_texture");
	plane.move(glm::vec3(0.0, -1.0, 0.0));
	plane.scale(glm::vec3(10.0, 10.0, 10.0));
	Plane rear_mirror;
	
	// λ�ƾ���
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(-55.0f), glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	// ͸��ͶӰ����
	glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(screen_width) / static_cast<float>(screen_height), 0.1f, 100.0f);

	std::vector<std::string> faces = { "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg" };
	CubeMap skybox;
	skybox.load_tex("texture/skybox", faces);
	
	// --------------------------------------------------------
	/*
	unsigned int quad_vao, quad_vbo;
	glGenVertexArrays(1, &quad_vao);
	glGenBuffers(1, &quad_vbo);
	glBindVertexArray(quad_vao);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/

	/*
	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);	
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	unsigned int textureColorbuffer;
	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	rear_mirror.add_texture(textureColorbuffer, "texture_diffuse");
	rear_mirror.move(glm::vec3(0.0, 0.8, 0.0));	
	rear_mirror.scale(glm::vec3(0.3, 0.2, 1.0));
	*/


	
	// ---------------------------------------------------------
	second_start = glfwGetTime();
	std::cout << "-----------------------------------------\n";
	std::cout << "start rendering...\n";
	std::cout << "-----------------------------------------\n";
	// ��ͣ����Ⱦ���棬�����¼�
	while (!glfwWindowShouldClose(window))
	{
		// �������
		processInput(window);
		
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
		// ������Ⱦ����
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		projection = glm::perspective(glm::radians(fov), static_cast<float>(screen_width) / static_cast<float>(screen_height), 0.1f, 100.0f);
		/*
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
		glDrawArrays(GL_TRIANGLES, 0, 36);*/



		
		
		tempShader.use();
		tempShader.setMat4f("model", model);
		//glm::mat4 temp_view = glm::rotate(cam.view(), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tempShader.setMat4f("view", cam.view());
		tempShader.setMat4f("projection", projection);
		cube.draw(tempShader);
		tempShader.setMat4f("model", plane.get_model());
		plane.draw(tempShader);

		skybox_shader.use();
		skybox_shader.setMat4f("projection", projection);
		skybox_shader.setMat4f("view", glm::mat4(glm::mat3(cam.view())));
		skybox.draw(skybox_shader);
		
			/*
			glStencilMask(0x00);
			tempShader.setMat4f("model", plane.get_model());
			plane.draw(tempShader);

			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilMask(0xFF);
			tempShader.setMat4f("model", model);
			cube.draw(tempShader);

			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			colorShader.use();
			colorShader.setMat4f("view", cam.view());
			colorShader.setMat4f("projection", projection);
			cube.scale(glm::vec3(1.25, 1.25, 1.25));
			colorShader.setMat4f("model", cube.get_model());
			cube.draw(colorShader);
			cube.scale(glm::vec3(0.8, 0.8, 0.8));
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glEnable(GL_DEPTH_TEST);
			*/
		/*
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		tempShader.setMat4f("view", cam.view());
		tempShader.setMat4f("model", model);
		cube.draw(tempShader);
		tempShader.setMat4f("model", plane.get_model());
		plane.draw(tempShader);
		*/

		
		
		// glDisable(GL_DEPTH_TEST | GL_STENCIL_TEST);
		// screen_shader.use();
		//glBindVertexArray(quadVAO);
		//glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		// screen_shader.setMat4f("model", rear_mirror.get_model());
		// rear_mirror.draw(screen_shader);
		
		// ���»��棬�����¼�	
		glfwSwapBuffers(window);
		glfwPollEvents();
		
		// ����FPS
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
	
	// ��ֹ����
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	// �������Esc�����رմ���
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	// �������ʿɼ���
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
		return nullptr;
	}
	// ���˴�������Ϊ��ǰ״̬����ǰ����Ⱦ��ʾĿ�꣩
	glfwMakeContextCurrent(window);
	// ���û���resizeʱ���õĺ���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	// �����ڴ��������ع��
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// ���ù���
	glfwSetScrollCallback(window, scroll_callback);
	// ����ʹ�õĹ�����������
	glfwSetCursorPosCallback(window, mouse_callback);
	// ��GLAD��ȡ������OpenGL��Ϣ����ʼ��
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		cout << "Failed to initialize GLAD\n";
		return nullptr;
	}

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;
	
	// ������ȾģʽΪ�߿�ģʽ
#ifdef __USE_LINE_MODE__
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

	glEnable(GL_DEPTH_TEST);						// �Ƿ�����Ȳ���
	//glDepthMask(GL_TRUE);							// ������Ȳ���ʱ���Ƿ񸲸���Ȼ��棬Ĭ��GL_TRUE
	/*
	 * GL_ALWAYS			һֱ������Ƚ��
	 * GL_NEVER				�Ӳ�������Ƚ��
	 * GL_LESS				���С�ڴ���ֵʱ���ݣ�Ĭ���������GL_LESS������cam�ǳ���z��������
	 * GL_EQUAL				������ڴ���ֵʱ����
	 * GL_LEQUAL			���С�ڵ��ڴ���ֵʱ����
	 * GL_GREATER			������ڴ���ֵʱ����
	 * GL_NOTEQUAL			��������ڴ���ֵʱ����
	 * GL_GEQUAL			������ڵ��ڴ���ֵʱ����
	 */
	//glDepthFunc(GL_LESS);							// ��Ȳ��Է�ʽ��Ĭ��GL_LESS

	glEnable(GL_STENCIL_TEST);						// �Ƿ���ģ�建��
	/*
	 * ����������ڶ�ÿ��stencilȡ���롱��һ��Ҫôȫ����0Ҫô��1
	 * 0xFF					����д�룬��1�൱��û��
	 * 0x00					ȫ�����㣬��0ȫ����0
	 */
	//glStencilMask(0xFF);							// ����ģ�����ʱ���Ƿ��ģ�帲�ǣ�Ĭ��0xFF
	/*
	 * func						�ȽϷ�ʽ
	 * ref						�Ƚ϶���
	 * mask						���Ƿ�ʽ
	 */
	//glStencilFunc(GL_EQUAL, 1, 0xFF);
	/*
	 * sfail					stencilδͨ���Ĵ���ʽ
	 * dpfail					stencilͨ����depthδͨ���Ĵ���ʽ
	 * dppass					stencil��depth��ͨ����Ĵ���ʽ
	 *
	 * ��ʽ��
	 * GL_KEEP					���ֵ�ǰֵ
	 * GL_ZERO					��Ϊ0
	 * GL_REPLACE				�滻ΪglStencilFunc��ref��ֵ
	 * GL_INCR					С�����ֵʱ+1
	 * GL_INCR_WRAP				С�����ֵʱ+1���������ֵʱ���Ϊ0
	 * GL_DECR					������Сֵʱ-1
	 * GL_DECR_WRAP				������Сֵʱ-1��С����Сֵʱ���Ϊ���ֵ
	 * GL_INVERT				��ÿ��λȡ��
	 */
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	
	//glEnable(GL_CULL_FACE);							// ���������޳�
	//glCullFace(GL_BACK);								// �޳��������

	//glEnable(GL_PROGRAM_POINT_SIZE);					// ����GLSL�е�gl_PointSize
	return window;
}