#include "CProgram.h"

Program::Program()
:_programID(0)
,_vertexShaderID(0)
,_fragmentShaderID(0)
{
}


Program::~Program()
{
}

void Program::initFile(const char* vShaderFile, const char* fShaderFile)
{

}
void Program::initString(const char * vShader, const char * fShader)
{
	_programID = glCreateProgram();

	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &source, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	return vertexShader;

	glAttachShader(_programID, vertexShader);
	glAttachShader(_programID, fragmentShader);
}

void Program::initShader(const char * source, glInt shaderType)
{
	//¶¥µã×ÅÉ«Æ÷vertexShader
	unsigned int vertexShader;

	std::string vertexCode;
	getFileString(sourceName, &vertexCode);
	const char* source = vertexCode.c_str();
	std::cout << source << std::endl;
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
