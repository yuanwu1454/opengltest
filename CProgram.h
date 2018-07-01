#pragma once

#include "common.h"
#include <glad/glad.h>
class Program
{
public:
	glInt _vertexShaderID;
	glInt _fragmentShaderID;
	glInt _programID;
	Program();
	virtual ~Program();
protected:
	void initFile(const char* vShaderFile, const char* fShaderFile);
	void initString(const char* vShader, const char* fShader);
	void initShader(const char* source, glInt shaderType);
private:
};