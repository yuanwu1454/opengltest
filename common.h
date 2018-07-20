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
vector<glm::vec3> cubePositions = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};


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
void createTextureCache(vector<std::string> strVec) {
	map<std::string, glInt> textureMap;
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
glInt createFrameBufferVerticesWithArrays(glInt VAO, glInt* VBO) {
	float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	//生成缓存对象
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, &VAO);

	//绑定VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	int offset = 0;
	int step = 4;
	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(0 + offset));
	glEnableVertexAttribArray(0);

	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)((2 + offset) * sizeof(float)));
	glEnableVertexAttribArray(1);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	return VAO;
}

glInt createSkyBoxVerticesWithArrays(glInt VAO, glInt* VBO) {
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};
	//生成缓存对象
	glGenBuffers(1, VBO);
	glGenVertexArrays(1, &VAO);

	//绑定VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, *VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	int offset = 0;
	int step = 3;
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, step * sizeof(float), (void*)(0 + offset));
	glEnableVertexAttribArray(0);
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

Shader* createNewShader(const char* vs, const char* fs) {
	return new Shader(vs, fs);
}

void cacheAllImage(TextureCache* inst) {
	vector<std::string> allImage = {
		"matrix.jpg",
		"container.jpg",
		"awesomeface.png",
		"container2.png",
		"container2_specular.png",
		"lighting_maps_specular_color.png",
		"wall.jpg",
	};

	vector<string> alphaImage = {
		"grass.png",
		"blending_transparent_window.png"
	};

	for (int i = 0; i < allImage.size(); i++){
		inst->cacheTexture(allImage[i]);
	}
	
	for (int i = 0; i < alphaImage.size(); i++){
		inst->cacheTexture(alphaImage[i]);
	}
}