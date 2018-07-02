#include "common.h"
#include "test.h"

void processInput(GLFWwindow *window);

void createUniformWithMatrix(glInt shaderProgram, const char* name) {
	glm::mat4 trans;
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
//	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
	glInt transformLoc = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void createUniformMatrixWithTime(glInt shaderProgram, const char* name) {
	glm::mat4 trans;
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
glInt transformLoc = glGetUniformLocation(shaderProgram, name);
glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void createUniformMatrixWithTime2(glInt shaderProgram, const char* name) {
	glm::mat4 trans;

	float sinValue = abs(sin((float)glfwGetTime()));
	trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	trans = glm::scale(trans, glm::vec3(sinValue, sinValue, sinValue));
	glInt transformLoc = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

//
//float vertices[] = {
//	// positions          // colors           // texture coords
//	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
//	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
//};

float fov = 45.0f;
float aspectRatio = 1.33f;
float viewLength = 0.0f;
float viewHor = 0.0f;
float radius = 10.0f;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
bool firstMouse = true;
double lastX, lastY;
float pitch = 0.0f, yaw = -90.0f;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
Camera cameraInst;
Shader programShader, lampShader;
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
int main()
{
	foo(3);
	GLFWwindow* window = NULL;
	if (!initCreateWindow(&window))
	{
		return -1;
	}
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glEnable(GL_DEPTH_TEST);

	programShader = Shader("texture.vs", "texture.fs");
	lampShader = Shader("texture.vs", "light.fs");
	cameraInst = Camera();

	glInt VAOs[2], VBO[2];
	VAOs[0] = createVerticesWithArrays(VAOs[0], &VBO[0]);
	std::cout << "VBO" << VBO[0] << std::endl;
	programShader.use();

	//generateTexture() begin
	string name[] = { "container.jpg", "awesomeface.png" };
	int n = 2;
	int textureID[2] = { 0, 0 };
	string preTexture = "texture";
	string textureName;
	for (int i = 0; i < n; i++) {
		textureName = preTexture + std::to_string(i + 1);
		programShader.setVec1(textureName.c_str(), i);
	}
	createTexture(name, textureID, 2);


	glm::vec3 cubePositions[] = {
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



	createUniformMVP(programShader);
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);

	//programShader.setVec3("objectColor", objectColor);
	//programShader.setVec3("lightColor", lightColor);
	programShader.setVec3("lightPos", lightPos);
	programShader.setVec3("viewPos", cameraInst.Position);

	bool btest = true;
	if(btest){
		//programShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//programShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//programShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//programShader.setFloat("material.shininess", 32.0f);
		programShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
		programShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		programShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		programShader.setFloat("material.shininess", 32.0f);
	}
	else {
		programShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
		programShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		programShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		programShader.setFloat("material.shininess", 32.0f);
	}

	lampShader.use();
	lampShader.setVec3("objectColor", objectColor);
	lampShader.setVec3("lightColor", lightColor);

	createUniformMVP(lampShader);
	//glm::mat4 model;
	//model = glm::translate(model, lightPos);
	//model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
	//lampShader.setMat4("model", model);

	//glm::mat4 trans;
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));


	//lampShader.use();
	glm::mat4 view, projection;
	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.3f, 0.5f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = cameraInst.GetViewMatrix();
		projection = glm::perspective(glm::radians(cameraInst.Zoom), aspectRatio, 0.1f, 100.0f);
		//激活程序
		programShader.use();
		programShader.setVec3("viewPos", cameraInst.Position);
		//lightPos.x = 4 * sin((float)glfwGetTime());
		programShader.setVec3("lightPos", lightPos);

		programShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
		programShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f); // 将光照调暗了一些以搭配场景
		programShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(VAOs[0]);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			float angle = 20.0f * (i + 1);
			if ((((i+1) % 3) != 0) && (i != 0)) {
				angle = 0;
			}
			//glm::vec3 lightColor;
			//lightColor.x = sin(glfwGetTime() * (2.0f + i));
			//lightColor.y = sin(glfwGetTime() * (0.7f + i));
			//lightColor.z = sin(glfwGetTime() * (1.3f + i));
			//glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f); // 降低影响
			//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

			//programShader.setVec3("light.ambient", ambientColor);
			//programShader.setVec3("light.diffuse", diffuseColor);

			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			//model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			programShader.setMat4("model", model);
			programShader.setMat4("view", view);
			programShader.setMat4("projection", projection);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		lampShader.use();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);

		glm::mat4 model;
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
		lampShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAOs[0]);
	glDeleteBuffers(1, &VBO[0]);
	// end
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraInst.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraInst.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraInst.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraInst.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		firstMouse = false;
		lastX = xpos;
		lastY = ypos;
	}
	float offsetX = lastX - xpos;
	float offsetY = ypos - lastY;
	lastX = xpos;
	lastY = ypos;
	cameraInst.ProcessMouseMovement(offsetX, offsetY);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	cameraInst.ProcessMouseScroll(yoffset);
}
