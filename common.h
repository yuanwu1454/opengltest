#pragma once
typedef unsigned int glInt;

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>
#include <set>
#include <map>
#include "camera.h"
#include "shader.h"
#include "uniform.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

const glInt SCR_WIDTH = 800;
const glInt SCR_HEIGHT = 600;


glInt generateTexture(const char* resName) {
	glInt textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// 加载并生成纹理
	int width, height, nrChannels;
	unsigned char *data = stbi_load(resName, &width, &height, &nrChannels, 0);
	std::string fileExtension;

	string filename = resName;
	size_t pos = filename.find_last_of('.');
	if (pos != std::string::npos)
	{
		fileExtension = filename.substr(pos, filename.length());
	}
	glInt rgb = GL_RGB;
	if (fileExtension == ".png") {
		rgb = GL_RGBA;
	}

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, rgb, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return textureId;
}

bool initCreateWindow(GLFWwindow** window) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	*window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (*window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(*window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}
	glViewport(0, 0, 800, 600);   //设置窗体坐标
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return true;
}
void createTexture(vector<string> textureName, vector<int> textureId) {
	int n = textureName.size();
	for (int i = 0; i < n; i++) {
		textureId[i] = generateTexture(textureName[i].c_str());
	}
	for (int i = 0; i < n; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureId[i]);
	}
}
void createTexture(Shader programShader) {
	programShader.use();
	vector<string> name{ "container2.png", "container2_specular.png" };
	vector<string> textureName{ "material.diffuse", "material.specular" };
	int n = name.size();
	vector<int> textureID(n, 0);
	for (int i = 0; i < n; i++) {
		programShader.setVec1(textureName[i].c_str(), i);
	}
	createTexture(name, textureID);
}



//glInt createVerticesWithElements(glInt VAO, float* vertices, glInt *indices) {

glInt createVerticesWithElements(glInt VAO) {
	glInt VBO, EBO;
	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	glInt indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	//生成缓存对象
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//绑定VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) , vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	return VAO;
}

//glInt createVerticesWithArrays(glInt VAO, float sideLength, glm::vec3 originPos) {
//	glInt VBO;
//	// positions          // colors           // texture coords
//	glm::vec3 cubePos[8];
//	glm::vec3 offsetPos[8] = {
//		glm::vec3(1.0,1.0,0.0),//A
//		glm::vec3(1.0,0.0,0.0),//B
//		glm::vec3(0.0,0.0,0.0),//C
//		glm::vec3(0.0,1.0,0.0),//D
//
//		glm::vec3(1.0,1.0,-1.0),//A'
//		glm::vec3(1.0,0.0,-1.0),//B'
//		glm::vec3(0.0,0.0,-1.0),//C'
//		glm::vec3(0.0,1.0,-1.0),//D'
//	};
//	glm::vec3 temp;
//	for (int i = 0; i < 7; i++) {
//		cubePos[i] = offsetPos[i] * sideLength + originPos;
//	}
//	float vertices2[36 * 8];
//	for (int i = 0; i < 6; i++) {
//
//	}
//}
glInt createVerticesWithArrays(glInt VAO, glInt* VBO) {
//	glInt VBO;
	// positions          // colors           // texture coords   //normal 
	float vertices[] = {
	//	back
		-0.5f,  0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // top left   D'
		-0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // bottom left C'
		0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // top right	 A'

		-0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // bottom left C'
		0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f, 0.0f, -1.0f,  // bottom right B'
		0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f, 0.0f, 0.0f, -1.0f,  // top right	 A'
	//	front
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  0.0f, 0.0f, 1.0f,// top right	 A
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f,// bottom right B
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // top left   D

		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom right B
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left C
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  // top left   D
	//right
		0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right B
		0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top left	 A
		0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top right    B'

		0.5f, 0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top right   A
		0.5f, 0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  1.0f, 0.0f, 0.0f,// bottom left A'
		0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right B'

	// left
		-0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,// top left	 D
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom right C
		-0.5f, 0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top right  D'

		-0.5f, 0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, -1.0f, 0.0f, 0.0f,// top right   D'
		-0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, -1.0f, 0.0f, 0.0f,// bottom left C
		-0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, -1.0f, 0.0f, 0.0f,// bottom right C'
	//	top
		0.5f,  0.5f, -1.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  0.0f, 1.0f, 0.0f,// top right	 A'
		0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   0.0f, 1.0f, 0.0f, // bottom right A
		-0.5f,  0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// top left   D'

		0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f,    1.0f, 0.0f,   0.0f, 1.0f, 0.0f,// bottom right A
		-0.5f, 0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f,// bottom left D
		-0.5f,  0.5f, -1.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f, 0.0f,// top left   D'
	// bottom

		0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f, -1.0f, 0.0f,// bottom right B
		0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  0.0f, -1.0f, 0.0f,// top left   B'
		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  0.0f, -1.0f, 0.0f,// bottom left C

		-0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  0.0f, -1.0f, 0.0f,// bottom right C
		0.5f, -0.5f, -1.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  0.0f, -1.0f, 0.0f,// top left   B'
		-0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom left C'
	};

	//生成缓存对象
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, &VAO);

	//绑定VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int offset = 0;
	int step = 11;
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(0+offset));
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)((3+offset) * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)((6+offset) * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)((8+offset) * sizeof(float)));
	glEnableVertexAttribArray(3);

	//glDrawArrays(GL_TRIANGLES, 0, 36);
	return VAO;
}

void useUniformMVP(Shader shaderProgram, glm::mat4& model, glm::mat4& view, glm::mat4& projection) {
	shaderProgram.setMat4("model", model);
	shaderProgram.setMat4("view", view);
	shaderProgram.setMat4("projection", projection);
}

void createUniformMVP(Shader shaderProgram)
{
	glm::mat4 model;
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	// 注意，我们将矩阵向我们要进行移动场景的反方向移动。
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), ((float)SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 100.0f);

	useUniformMVP(shaderProgram, model, view, projection);
}


glm::vec3 aToVec3(float v[]) {
	return glm::vec3(v[0], v[1], v[2]);
}
