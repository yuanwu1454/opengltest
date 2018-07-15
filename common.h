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
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;
#include "texture.h"
#include "camera.h"
#include "shader.h"
#include "uniform.h"
#include "mesh.h"
#include "model.h"
#include "controller.h"
#include "test.h"



const glInt SCR_WIDTH = 800;
const glInt SCR_HEIGHT = 600;

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
void createTexture(Shader programShader, vector<string> name, vector<string> textureName, bool balpha = false) {
	programShader.use();
	int n = name.size();
	vector<int> textureID(n, 0);
	for (int i = 0; i < n; i++) {
		textureID[i] = generateTexture(name[i].c_str(), balpha);
	}

	for (int i = 0; i < n; i++) {
		programShader.setVec1(textureName[i].c_str(), i);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
	}
}

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

void printMat44(glm::mat4& model) {
	printf("begin;\n");
	printf("%f, %f, %f, %f\n", model[0][1], model[0][1], model[0][2], model[0][3]);
	printf("%f, %f, %f, %f\n", model[1][1], model[1][1], model[1][2], model[1][3]);
	printf("%f, %f, %f, %f\n", model[2][1], model[2][1], model[2][2], model[2][3]);
	printf("%f, %f, %f, %f\n", model[3][1], model[3][1], model[3][2], model[3][3]);
	printf("end;  \n");
}
void printVec3(glm::vec3& model) {
	printf("begin;\n");
	printf("%f, %f, %f\n", model[0], model[1], model[2]);
	printf("end;  \n");
}

void initShader(Shader iShader, const char* vs, const char* fs) {
	iShader = Shader(vs, fs);
	iShader.use();
	createUniformMVP(iShader);
}