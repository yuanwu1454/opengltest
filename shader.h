#ifndef SHADER_H
#define SHADER_H


char infoLog[512];
int success;
void getFileString(const char* fileName, std::string* vertexCode) {
	// 1.从文件路径中获取顶点/片段着色器
	std::ifstream vShaderFile;
	// 保证ifstream对象可以抛出异常：
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		vShaderFile.open(fileName);
		std::stringstream vShaderStream;
		// 读取文件的缓冲内容到数据流中
		vShaderStream << vShaderFile.rdbuf();
		// 关闭文件处理器
		vShaderFile.close();
		// 转换数据流到string
		*vertexCode = vShaderStream.str();
		//std::cout << (*vertexCode).c_str() << std::endl;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
}

class Shader {
public:
	glInt vertexShader;
	glInt fragmentShader;
	glInt geometryShader;
	glInt shaderProgram;
	Shader()
	{
	}

	void initShader(const char* vertexSourceName, const char* fragmentShaderName, const char* geometryShaderName = "") {
		vertexShader = createShader(GL_VERTEX_SHADER, vertexSourceName);
		fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderName);
		geometryShader = createShader(GL_GEOMETRY_SHADER, geometryShaderName);
		shaderProgram = createProgram();
	}

	void use() 
	{
		glUseProgram(shaderProgram);
	}

	//set 操作之前都一定要先调用对应的glUseProgram
	void setVec3(const char* uniformName, glm::vec3 &value) {
		glUniform3fv(glGetUniformLocation(shaderProgram, uniformName), 1, glm::value_ptr(value));
	}

	void setVec3(const char* uniformName, float x, float y, float z) {
		glUniform3f(glGetUniformLocation(shaderProgram, uniformName), x, y, z);
	}

	void setMat4(const char* uniformName, glm::mat4 &value) {
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, uniformName), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setVec1(const char* uniformName, int value) {
		glUniform1i(glGetUniformLocation(shaderProgram, uniformName), value);
	}
	void setInt(const char* uniformName, int value) {
		glUniform1i(glGetUniformLocation(shaderProgram, uniformName), value);
	}
	void setFloat(const char* uniformName, float value) {
		glUniform1f(glGetUniformLocation(shaderProgram, uniformName), value);
	}
	void bindingUniformBlock(const char* blockName, glInt blockIndex) {
		glInt uniformBlockIndex = glGetUniformBlockIndex(shaderProgram, "Matrices");
		glUniformBlockBinding(shaderProgram, uniformBlockIndex, blockIndex);
	}	
private:
	glInt createProgram() {
		glInt shaderProgram;
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		if(geometryShader != 0){
			glAttachShader(shaderProgram, geometryShader);
		}
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		if (geometryShader != 0) {
			glDeleteShader(geometryShader);
		}
		return shaderProgram;
	}

	glInt createShader(int shaderType, const char* sourceName) {
		//顶点着色器vertexShader
		if (sourceName == "") {
			return 0;
		}
		glInt vertexShader;
		std::string vertexCode;
		getFileString(sourceName, &vertexCode);
		const char* source = vertexCode.c_str();
		vertexShader = glCreateShader(shaderType);
		glShaderSource(vertexShader, 1, &source, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return vertexShader;
	}
};

#endif