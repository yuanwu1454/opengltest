#include "common.h"
#define NR_POINT_LIGHTS 4
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
void doLampShader(glm::mat4& view, glm::mat4& projection);

void initProgramShader(glm::vec3 objectColor, glm::vec3 lightColor);
void doProgramShader(glm::mat4& view, glm::mat4& projection);

void initAssimpShader(glm::vec3 objectColor, glm::vec3 lightColor);
void doAssimpShader(glm::mat4& view, glm::mat4& projection);

void initStencilShader(glm::vec3 objectColor, glm::vec3 lightColor);
void doStencilShader(glm::mat4& view, glm::mat4& projection);

void doShaderWithVAO(glm::mat4& view, glm::mat4& projection, string shaderName);
void doStencilTest(glm::mat4& view, glm::mat4& projection);

Camera cameraInst;
Shader programShader, lampShader, assimpShader, stencilShader;
Uniform dirlight, material, pointLights[4];
Model assimpModel;

glm::vec3 lightPos(-0.0973745, 0.420927, - 5.06509);
float ambientlight[3] = { 1.0f, 1.0f, 1.0f };
float diffuselight[3] = { 1.0f, 1.0f, 1.0f };
float specularlight[3] = { 1.0f, 1.0f, 1.0f };
float direction[3] = { 1.0f, 1.0f, 1.0f };
float *curlight = ambientlight;
string lastlight = "ambientlight";
glm::vec3 pointLightsPosition[4] = {
	glm::vec3(1.0f,  0.0f,  1.0f),
	glm::vec3(1.0f,  6.0f,  1.0f),
	glm::vec3(.0f,  0.0f,  5.0f),
	glm::vec3(1.0f,  0.0f,  1.0f)
};
glInt VAOs[2];
bool bAssimp = false;
int main()
{
	bool bTest = false;
	if(bTest){
		foo(5);	
		system("pause");
		return 0;
	}

	GLFWwindow* window = NULL;
	if (!initCreateWindow(&window))
	{
		return -1;
	}
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	glEnable(GL_DEPTH_TEST);
	/*
		glDepthMask(GL_FALSE); //禁止深度缓冲的写入
		glDepthFunc(GL_LESS);  //深度缓冲测试函数
	*/
	glEnable(GL_STENCIL_TEST);


//	glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
	//光照
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
	initAssimpShader(lightColor, objectColor);
	initProgramShader(lightColor, objectColor);
	initLampShader(lightColor, objectColor);
	initStencilShader(lightColor, objectColor);
	cameraInst = Camera();
	dirlight = Uniform();
	
	//顶点
	glInt VBO[2];
	VAOs[0] = createVerticesWithArrays(VAOs[0], &VBO[0]);
	programShader.setFloat("material.shininess", 32.0f);
	assimpShader.setFloat("material.shininess", 32.0f);
	glm::mat4 view, projection;

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//这个只是用来进行测试的函数不是用来写入模版缓冲的函数
	while (!glfwWindowShouldClose(window))
	{
		updateFrame();
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		view = cameraInst.GetViewMatrix();
		projection = glm::perspective(glm::radians(cameraInst.Zoom), aspectRatio, 0.1f, 100.0f);

		doAssimpShader(view, projection);
		//doShaderWithVAO(view, projection, "Program");
		//doStencilTest(view, projection);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAOs[0]);
	glDeleteBuffers(1, &VBO[0]);
	glfwTerminate();
	return 0;
}
bool checkValid(int action) {
	return action == GLFW_REPEAT || action == GLFW_PRESS;
}

float pointPosition[3] = {0.0f, 0.0f, 0.0f};
int pointIdx = 0;
float dirSpeed = 5.0f;
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
	else if (key == GLFW_KEY_I || key == GLFW_KEY_J || key == GLFW_KEY_K || key == GLFW_KEY_L) {
		pointIdx = key - GLFW_KEY_I;
		lightPos[0] = pointLightsPosition[pointIdx].x;
		lightPos[1] = pointLightsPosition[pointIdx].y;
		lightPos[2] = pointLightsPosition[pointIdx].z;
	}

	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3 ) && (checkValid(action)))
	{
		int operad = 0;
		if (mods == GLFW_MOD_SHIFT) {
			operad = 1;
		}
		else if (mods == GLFW_MOD_CONTROL) {
			operad = -1;
		}
		*(key - GLFW_KEY_1 + curlight) = *(key - GLFW_KEY_1 + curlight) + operad * deltaTime;
	}

	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3) && (action == GLFW_RELEASE))
	{
		std::cout << lastlight << "  "<< *(curlight) << "  "<< *(curlight + 1) << "  " << *(curlight + 2) << std::endl;
	}
	if (key == GLFW_KEY_F) {
		if(mods == GLFW_MOD_SHIFT)
			cameraInst.ProcessKeyboardEx(Camera_Movement::PITCHUP, deltaTime);
		if(mods == GLFW_MOD_CONTROL)
			cameraInst.ProcessKeyboardEx(Camera_Movement::PITCHDOWN, deltaTime);
	}
	if (key == GLFW_KEY_Y) {
		if (mods == GLFW_MOD_SHIFT)
			cameraInst.ProcessKeyboardEx(Camera_Movement::YAWUP, deltaTime);
		if (mods == GLFW_MOD_CONTROL)
			cameraInst.ProcessKeyboardEx(Camera_Movement::YAWDOWN, deltaTime);
	}
	if (key == GLFW_KEY_R) {
		if (mods == GLFW_MOD_SHIFT)
			cameraInst.ProcessKeyboardEx(Camera_Movement::ROLLUP, deltaTime);
		if (mods == GLFW_MOD_CONTROL)
			cameraInst.ProcessKeyboardEx(Camera_Movement::ROLLDOWN, deltaTime);
	}

}
bool bView = true;
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
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraInst.ProcessKeyboard(Camera_Movement::UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraInst.ProcessKeyboard(Camera_Movement::DOWN, deltaTime);

	if (bView) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			lightPos = lightPos - cameraInst.Front * deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			lightPos = lightPos + cameraInst.Front * deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			lightPos = lightPos - cameraInst.Right * deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			lightPos = lightPos + cameraInst.Right * deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			lightPos = lightPos + cameraInst.Up * deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			lightPos = lightPos - cameraInst.Up * deltaTime * dirSpeed;
		pointLightsPosition[pointIdx].x = lightPos[0];
		pointLightsPosition[pointIdx].y = lightPos[1];
		pointLightsPosition[pointIdx].z = lightPos[2];
	}
	else {
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
			lightPos[0] = lightPos[0] - deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS)
			lightPos[0] = lightPos[0] + deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			lightPos[1] = lightPos[1] - deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			lightPos[1] = lightPos[1] + deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			lightPos[2] = lightPos[2] + deltaTime * dirSpeed;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			lightPos[2] = lightPos[2] - deltaTime * dirSpeed;
		pointLightsPosition[pointIdx].x = lightPos[0];
		pointLightsPosition[pointIdx].y = lightPos[1];
		pointLightsPosition[pointIdx].z = lightPos[2];
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
bool bPrint = false;
void doLampShader(glm::mat4& view, glm::mat4& projection) {
	lampShader.use();

	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		glm::mat4 model;
		if (bPrint)
		{
			printf("init;\n");
			printMat44(model);
			printVec3(pointLightsPosition[1]);
		}
		model = glm::translate(model, pointLightsPosition[i]);
		if (bPrint)
		{
			printf("translate;\n");
			printMat44(model);
		}
		//float scalef = 0.05;
		//model = glm::scale(model, glm::vec3(scalef, scalef, scalef));
		useUniformMVP(lampShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		if (bPrint)
		{
			printf("uniform;\n");
			printMat44(model);
		}
	}
	bPrint = false;
}

void initStencilShader(glm::vec3 objectColor, glm::vec3 lightColor) {
	stencilShader = Shader("texture.vs", "stencil.fs");
	stencilShader.use();
	createUniformMVP(stencilShader);
}
void doStencilShader(glm::mat4& view, glm::mat4& projection) {
	stencilShader.use();
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		glm::mat4 model;
		model = glm::translate(model, pointLightsPosition[i]);
		float scalef = 1.1;
		model = glm::scale(model, glm::vec3(scalef, scalef, scalef));
		useUniformMVP(stencilShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}


void initProgramShader(glm::vec3 objectColor, glm::vec3 lightColor) {
	programShader = Shader("texture.vs", "texture.fs");
	programShader.use();
	vector<string> name{ "container2.png", "container2_specular.png" };
	vector<string> textureName{ "material.diffuse", "material.specular" };
	createTexture(programShader, name, textureName);
	createUniformMVP(programShader);
}

bool bStatic = true;
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

void doProgramShader(glm::mat4& view, glm::mat4& projection) {

	//激活程序
	programShader.use();
	programShader.setVec3("viewPos", cameraInst.Position);

	dirlight.setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
	dirlight.setAttrName("direction", aToVec3(direction));
	dirlight.bind(programShader, "dirLight");

	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		pointLights[i].setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
		pointLights[i].setAttrName("position", pointLightsPosition[i]);
		pointLights[i].setAttenuationAttrName(1.0f, 0.09f,0.032f);
		pointLights[i].bind(programShader, ("pointLights[" + std::to_string(i) + "]").c_str());
	}

	for (unsigned int i = 0; i < cubePositions.size(); i++)
	{
		glm::mat4 model;
		float angle = 20.0f * (i + 1);
		if ((((i + 1) % 3) != 0) && (i != 0)) {
			angle = 0;
		}
		model = glm::translate(model, cubePositions[i]);
		if (bStatic == false) {
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		}
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		useUniformMVP(programShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void initAssimpShader(glm::vec3 objectColor, glm::vec3 lightColor) {
	assimpShader = Shader("Assimp.vs", "Assimp.fs");
	assimpShader.use();
	assimpModel = Model("./nanosuit/nanosuit.obj");
}

void doAssimpShader(glm::mat4& view, glm::mat4& projection) {
	dirlight.setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
	dirlight.setAttrName("direction", aToVec3(direction));
	dirlight.bind(assimpShader, "dirLight");

	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		pointLights[i].setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
		pointLights[i].setAttrName("position", pointLightsPosition[i]);
		pointLights[i].setAttenuationAttrName(1.0f, 0.09f, 0.032f);
		pointLights[i].bind(assimpShader, ("pointLights[" + std::to_string(i) + "]").c_str());
	}

	assimpShader.setVec3("viewPos", cameraInst.Position);

	glm::mat4 model;
	useUniformMVP(assimpShader, model, view, projection);
	assimpShader.use();
	assimpModel.Draw(assimpShader);
	model = glm::translate(model, glm::vec3(10.0f,0.0f,0.0f));
	useUniformMVP(assimpShader, model, view, projection);
	assimpShader.use();
	assimpModel.Draw(assimpShader);
}

void doShaderWithVAO(glm::mat4& view, glm::mat4& projection, string shaderName) {
	glBindVertexArray(VAOs[0]);
	if (shaderName.compare("Program") == 0) {
		doProgramShader(view, projection);
	}
	else if (shaderName.compare("Stencil") == 0) {
		doStencilShader(view, projection);
	}
	else if (shaderName.compare("Lamp") == 0) {
		doLampShader(view, projection);
	}
	glBindVertexArray(0);
}

void doStencilTest(glm::mat4& view, glm::mat4& projection) {
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//当测试通过的时候 才会用来进行写入模版缓冲的函数
	glStencilMask(0xFF); //启用模版写入
						 //这个函数是用来对写入的模版缓冲进行限制的函数
	doShaderWithVAO(view, projection, "Lamp");
	glDisable(GL_DEPTH_TEST); //禁用深度测试
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //没有通过模版测试 则片段会被丢弃
	glStencilMask(0x0); //禁用模版写入

	doShaderWithVAO(view, projection, "Stencil");

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
}