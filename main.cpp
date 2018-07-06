#include "common.h"
#include "test.h"
#define NR_POINT_LIGHTS 2
void processInput(GLFWwindow *window);
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

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

void updateFrame();
void initLampShader(glm::vec3 objectColor, glm::vec3 lightColor);
void doLampShader(glm::mat4 view, glm::mat4 projection);
void initProgramShader(glm::vec3 objectColor, glm::vec3 lightColor);
void doProgramShader();


Camera cameraInst;
Shader programShader, lampShader;
Uniform dirlight, material, pointLights[4];

glm::vec3 lightPos(-0.0973745, 0.420927, - 5.06509);
float ambientlight[3] = { 1.0f, 1.0f, 1.0f };
float diffuselight[3] = { 1.0f, 1.0f, 1.0f };
float specularlight[3] = { 1.0f, 1.0f, 1.0f };
float direction[3] = { 1.0f, 1.0f, 1.0f };
float *curlight = ambientlight;
string lastlight = "ambientlight";
glm::vec3 pointLightsPosition[4] = {
	glm::vec3(1.0f,  0.0f,  1.0f),
	glm::vec3(2.0f,  5.0f, -3.0f),
	glm::vec3(-14.5f, -5.2f, -2.5f),
	glm::vec3(-8.8f, -2.0f, -12.3f)
};
int main()
{
	GLFWwindow* window = NULL;
	if (!initCreateWindow(&window))
	{
		return -1;
	}
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	glEnable(GL_DEPTH_TEST);

	//光照
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);

	initProgramShader(lightColor, objectColor);
	initLampShader(lightColor, objectColor);

	cameraInst = Camera();
	dirlight = Uniform();

	//顶点
	glInt VAOs[2], VBO[2];
	VAOs[0] = createVerticesWithArrays(VAOs[0], &VBO[0]);
	//programShader.setVec3("light.position", lightPos);
	//programShader.setVec3("light.direction", 0.2f, 1.0f, 0.3f);
	//programShader.setFloat("light.cutOff", glm::cos(glm::radians(10.5f)));
	//programShader.setFloat("light.outerCutOff", glm::cos(glm::radians(50.5f)));

	bool btest = true;
	if(btest){
		//programShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//programShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//programShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		//programShader.setFloat("material.shininess", 32.0f);
		//programShader.setVec3("material.ambient", 0.0f, 0.1f, 0.06f);
		//programShader.setVec3("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		//programShader.setVec3("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		programShader.setFloat("material.shininess", 32.0f);
	}
	else {
		//programShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
		//programShader.setVec3("material.diffuse", 1.0f, 1.0f, 1.0f);
		//programShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
		programShader.setFloat("material.shininess", 32.0f);
	}

	//programShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
	//programShader.setFloat("light.constant", 1.0f);
	//programShader.setFloat("light.linear", 0.09f);
	//programShader.setFloat("light.quadratic", 0.032f);
	//glm::mat4 model;
	//model = glm::translate(model, lightPos);
	//model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
	//lampShader.setMat4("model", model);
	glm::mat4 view, projection;
	bool bStatic = true;
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

	while (!glfwWindowShouldClose(window))
	{


		updateFrame();
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = cameraInst.GetViewMatrix();
		projection = glm::perspective(glm::radians(cameraInst.Zoom), aspectRatio, 0.1f, 100.0f);
		//lightPos.x = 4 * sin((float)glfwGetTime());
		//programShader.setVec3("lightPos", lightPos);
		doProgramShader();
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
			if (bStatic == false) {
				model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			useUniformMVP(programShader, model, view, projection);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		doLampShader(view,projection);
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
bool checkValid(int action) {
	return action == GLFW_REPEAT || action == GLFW_PRESS;
}
static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	
	if (key == GLFW_KEY_Z) {
		curlight = ambientlight;
		lastlight = "ambientlight";
	}
	else if (key == GLFW_KEY_X) {
		curlight = diffuselight;
		lastlight = "diffuselight";
	}
	else if (key == GLFW_KEY_C) {
		curlight = specularlight;
		lastlight = "specularlight";
	}
	else if (key == GLFW_KEY_V) {
		curlight = direction;
		lastlight = "direction";
	}
	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3 ) && (checkValid(action)))
	{
		//if (action == GLFW_PRESS) {

		//}
		if (mods == GLFW_MOD_SHIFT) {
			*(key - GLFW_KEY_1 + curlight) = *(key - GLFW_KEY_1 + curlight) - 0.01f;
		}
		else if (mods == GLFW_MOD_CONTROL) {
			*(key - GLFW_KEY_1 + curlight) = *(key - GLFW_KEY_1 + curlight) + 0.01f;
		}
		//std::cout << *(key - GLFW_KEY_1 + curlight) << std::endl;
	}

	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3) && (action == GLFW_RELEASE))
	{
		std::cout << lastlight << "  "<< *(curlight) << "  "<< *(curlight + 1) << "  " << *(curlight + 2) << std::endl;
	}
}
bool bView = true;
void processInput(GLFWwindow *window)
{
	float dirSpeed = 5.0f;
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

	if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
		if (bView) {
			lightPos = lightPos - cameraInst.Front * deltaTime * dirSpeed;
		}
		else {
			lightPos[0] = lightPos[0] - deltaTime * dirSpeed;
		}
	if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT ) == GLFW_PRESS)
		if (bView) {
			lightPos = lightPos + cameraInst.Front * deltaTime * dirSpeed;
		}
		else {
			lightPos[0] = lightPos[0] + deltaTime * dirSpeed;
		}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		if (bView) {
			lightPos = lightPos - cameraInst.Right * deltaTime * dirSpeed;
		}
		else {
			lightPos[1] = lightPos[1] - deltaTime * dirSpeed;
		}
		
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		if (bView) {
			lightPos = lightPos + cameraInst.Right * deltaTime * dirSpeed;
		}
		else {
			lightPos[1] = lightPos[1] + deltaTime * dirSpeed;
		}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		if (bView) {
			lightPos = lightPos + cameraInst.Up * deltaTime * dirSpeed;
		}
		else {
			lightPos[2] = lightPos[2] + deltaTime * dirSpeed;
		}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		if (bView) {
			lightPos = lightPos - cameraInst.Up * deltaTime * dirSpeed;
		}
		else {
			lightPos[2] = lightPos[2] - deltaTime * dirSpeed;
		}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		std::cout << lightPos[0] << " " << lightPos[1] << " " << lightPos[2] << std::endl;
	}
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

void updateFrame() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void initLampShader(glm::vec3 objectColor, glm::vec3 lightColor) {
	lampShader = Shader("texture.vs", "light.fs");
	lampShader.use();
	lampShader.setVec3("objectColor", objectColor);
	lampShader.setVec3("lightColor", lightColor);
	createUniformMVP(programShader);
}

void doLampShader(glm::mat4 view, glm::mat4 projection) {
	lampShader.use();

	glm::mat4 model;
	model = glm::translate(model, lightPos);
	float scalef = 0.05;
	model = glm::scale(model, glm::vec3(scalef, scalef, scalef));

	useUniformMVP(lampShader, model, view, projection);
}

void initProgramShader(glm::vec3 objectColor, glm::vec3 lightColor) {
	programShader = Shader("texture.vs", "texture.fs");
	programShader.use();
	createTexture(programShader);	//纹理
	createUniformMVP(programShader);	//变换
}

void doProgramShader() {
	//激活程序
	programShader.use();
	programShader.setVec3("viewPos", cameraInst.Position);

	dirlight.setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
	dirlight.setAttrName("direction", aToVec3(direction));
	dirlight.bind(programShader, "dirLight");

	//pointLights[0].setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
	//pointLights[1].setAttrName("position", aToVec3(position[0]));
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		pointLights[i].setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
		pointLights[i].setAttrName("position", pointLightsPosition[i]);
		pointLights[i].setAttenuationAttrName(1.0f, 0.09f,0.032f);
		std::ostringstream ostr;
		ostr << "pointLights[" << i << "]";
		pointLights[i].bind(programShader, ostr.str());		
	}
}