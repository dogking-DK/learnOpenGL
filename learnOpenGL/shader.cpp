#include "shader.h"

shader::shader(const char* vertex_path, const char* fragment_path)
{
	std::string vertex_code;
	std::string fragment_code;
	// vertex shader file
	std::ifstream vs_file;
	// fragment shader file
	std::ifstream fs_file;

	// 检查istream能否正常抛出异常
	vs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fs_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vs_file.open(vertex_path);
		fs_file.open(fragment_path);
		std::stringstream vs_stream, fs_stream;

		vs_stream << vs_file.rdbuf();
		fs_stream << fs_file.rdbuf();

		vs_file.close();
		fs_file.close();

		vertex_code = vs_stream.str();
		fragment_code = fs_stream.str();
	}
	catch (std::ifstream::failure* e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
	}

	// --------------------声明临时的着色器相关变量--------------------
	// 顶点着色器代码串的指针
	const char* vs_code = vertex_code.c_str();
	// 片元着色器代码串的指针
	const char* fs_code = fragment_code.c_str();
	// 一个储存运行结果的整数
	int success;
	// 运行日志缓存
	char info_log[512];
	// 顶点着色器（准确说是的指针）
	unsigned int vertex_shader;
	// 片元着色器
	unsigned int fragment_shader;
	
	// --------------------设置顶点着色器--------------------
	// 创造对应类型为顶点着色器的着色器
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// 获得着色器的源文件（glsl文件）
	glShaderSource(vertex_shader, 1, &vs_code, nullptr);
	// 编译着色器，注意着色器读取后仅仅是一个文本，要编译才能使用
	glCompileShader(vertex_shader);
	// 检查顶点着色器
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// --------------------设置片元着色器--------------------
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fs_code, nullptr);
	glCompileShader(fragment_shader);
	// 检查片元着色器
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// --------------------链接所用的着色器--------------------
	ID = glCreateProgram();
	glAttachShader(ID, vertex_shader);
	glAttachShader(ID, fragment_shader);
	glLinkProgram(ID);
	// 检查program链接是否成功
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << info_log << std::endl;
	}
	// 删除使用后的着色器
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void shader::use()
{
	glUseProgram(ID);
}

void shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<int>(value));
}

void shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void shader::setMat4f(const std::string& name, const glm::mat4& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void shader::setMat3f(const std::string& name, const glm::mat3& value) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void shader::setVec3f(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void shader::setVec3f(const std::string& name, const float x, const float y, const float z) const
{
	
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(glm::vec3(x, y, z)));
}
