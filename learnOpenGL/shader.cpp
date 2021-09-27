#include "shader.h"

shader::shader(const char* vertex_path, const char* fragment_path)
{
	std::string vertex_code;
	std::string fragment_code;
	// vertex shader file
	std::ifstream vs_file;
	// fragment shader file
	std::ifstream fs_file;

	// ���istream�ܷ������׳��쳣
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

	// --------------------������ʱ����ɫ����ر���--------------------
	// ������ɫ�����봮��ָ��
	const char* vs_code = vertex_code.c_str();
	// ƬԪ��ɫ�����봮��ָ��
	const char* fs_code = fragment_code.c_str();
	// һ���������н��������
	int success;
	// ������־����
	char info_log[512];
	// ������ɫ����׼ȷ˵�ǵ�ָ�룩
	unsigned int vertex_shader;
	// ƬԪ��ɫ��
	unsigned int fragment_shader;
	
	// --------------------���ö�����ɫ��--------------------
	// �����Ӧ����Ϊ������ɫ������ɫ��
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	// �����ɫ����Դ�ļ���glsl�ļ���
	glShaderSource(vertex_shader, 1, &vs_code, nullptr);
	// ������ɫ����ע����ɫ����ȡ�������һ���ı���Ҫ�������ʹ��
	glCompileShader(vertex_shader);
	// ��鶥����ɫ��
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// --------------------����ƬԪ��ɫ��--------------------
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fs_code, nullptr);
	glCompileShader(fragment_shader);
	// ���ƬԪ��ɫ��
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
	}

	// --------------------�������õ���ɫ��--------------------
	ID = glCreateProgram();
	glAttachShader(ID, vertex_shader);
	glAttachShader(ID, fragment_shader);
	glLinkProgram(ID);
	// ���program�����Ƿ�ɹ�
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, nullptr, info_log);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << info_log << std::endl;
	}
	// ɾ��ʹ�ú����ɫ��
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
