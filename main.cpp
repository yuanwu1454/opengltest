#include "common.h"
#define NR_POINT_LIGHTS 4
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void updateFrame();

void initLampShader();
void doLampShader();

void initProgramShader();
void doProgramShader();

void initAssimpShader();
void doAssimpShader();

void initStencilShader();
void doStencilShader();

void doShaderWithVAO(string shaderName);
void doStencilTest();

void initGrassShader();
void doGrassShader();

void initBlendShader();
void doBlendShader();
void doBlendTest();

Shader programShader, lampShader, assimpShader, stencilShader, grassShader, blendShader;
Uniform dirlight, material, pointLights[4];
Model assimpModel;
glm::mat4 view, projection;
glm::vec3 lightColor, objectColor;

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
		glDepthMask(GL_FALSE); //��ֹ��Ȼ����д��
		glDepthFunc(GL_LESS);  //��Ȼ�����Ժ���
	*/
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glStencilMask(0xFF); // ÿһλд��ģ�建��ʱ������ԭ��
	//����
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
	//initAssimpShader();
	initProgramShader();
	initLampShader();
	initStencilShader();
	initGrassShader();
	initBlendShader();

	dirlight = Uniform();
	
	//����
	glInt VBO[2];
	VAOs[0] = createVerticesWithArrays(VAOs[0], &VBO[0]);
	programShader.setFloat("material.shininess", 32.0f);
	assimpShader.setFloat("material.shininess", 32.0f);


	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//���ֻ���������в��Եĺ�����������д��ģ�滺��ĺ���
	while (!glfwWindowShouldClose(window))
	{
		updateFrame();
		processInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		view = cameraInst.GetViewMatrix();
		projection = glm::perspective(glm::radians(cameraInst.Zoom), aspectRatio, 0.1f, 100.0f);

		//doAssimpShader(view, projection);
		//doShaderWithVAO(view, projection, "Program");
		//doStencilTest(view, projection);
		//doGrassShader(view, projection);
		//doStencilShader();
		//doBlendShader();
		doStencilTest();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAOs[0]);
	glDeleteBuffers(1, &VBO[0]);
	glfwTerminate();
	return 0;
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	keyDelegate(window, key, scancode, action, mods);
}

void processInput(GLFWwindow *window)
{
	inputDelegate(window);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	cameraInst.preProcessMouseMovement(xpos, ypos);
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

void initLampShader() {
	lampShader = Shader("texture.vs", "light.fs");
	lampShader.use();
	lampShader.setVec3("objectColor", objectColor);
	lampShader.setVec3("lightColor", lightColor);
	createUniformMVP(programShader);
}

void doLampShader() {
	lampShader.use();
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		glm::mat4 model;
		model = glm::translate(model, pointLightsPosition[i]);
		useUniformMVP(lampShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void initStencilShader() {
	stencilShader = Shader("texture.vs", "stencil.fs");
	stencilShader.use();
	createUniformMVP(stencilShader);
	initShader(stencilShader, "texture.vs", "stencil.fs");
}
void doStencilShader() {
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

void initGrassShader() {
	grassShader = Shader("texture.vs", "discard.fs");
	grassShader.use();
	vector<string> name{"grass.png"};
	vector<string> textureName{"texture1"};
	//�˴����ò���Ϊtrue ���Դ����Ӧ��alpha
	createTexture(grassShader, name, textureName, true); 
	createUniformMVP(grassShader);
}
vector<glm::vec3> vegetation = {
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

void doGrassShader() {
	glBindVertexArray(VAOs[0]);
	grassShader.use();
	for (int i = 0; i < vegetation.size(); i++)
	{
		glm::mat4 model;
		model = glm::translate(model, vegetation[i]);
		useUniformMVP(grassShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	glBindVertexArray(0);
}

void initBlendShader() {
	blendShader = Shader("texture.vs", "blend.fs");
	blendShader.use();
	vector<string> name{ "blending_transparent_window.png" };
	vector<string> textureName{ "texture1" };
	//�˴����ò���Ϊtrue ���Դ����Ӧ��alpha
	createTexture(blendShader, name, textureName, true);
	createUniformMVP(blendShader);
}
vector<glm::vec3> blendPos = {
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(-1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
};

void doBlendShader() {
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < blendPos.size(); i++)
	{
		float distance = glm::length(cameraInst.Position - blendPos[i]);
		sorted[distance] = blendPos[i];
	}

	//����ʹ�ù̶�λ�� һ��Ҫ�����ӵ���ʱ�ı����˳��
	glBindVertexArray(VAOs[0]);
	blendShader.use();
	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		glm::mat4 model;
		model = glm::mat4();
		model = glm::translate(model, it->second);
		useUniformMVP(blendShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	//����˳�����Ҫ ԭ����Ȼ���Ӱ�쵼�µ�
	glBindVertexArray(0);
}

void initProgramShader() {
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

void doProgramShader() {

	//�������
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

void initAssimpShader() {
	assimpShader = Shader("Assimp.vs", "Assimp.fs");
	assimpShader.use();
	assimpModel = Model("./nanosuit/nanosuit.obj");
}

void doAssimpShader() {
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

void doShaderWithVAO(string shaderName) {
	glBindVertexArray(VAOs[0]);
	if (shaderName.compare("Program") == 0) {
		doProgramShader();
	}
	else if (shaderName.compare("Stencil") == 0) {
		doStencilShader();
	}
	else if (shaderName.compare("Lamp") == 0) {
		doLampShader();
	}
	glBindVertexArray(0);
}

void doStencilTest() {
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//������ͨ����ʱ�� �Ż���������д��ģ�滺��ĺ���
	glStencilMask(0xFF); //����ģ��д��
						 //���������������д���ģ�滺��������Ƶĺ���
	doShaderWithVAO("Lamp");
	glDisable(GL_DEPTH_TEST); //������Ȳ���
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //û��ͨ��ģ����� ��Ƭ�λᱻ����
	glStencilMask(0x0); //����ģ��д��

	doShaderWithVAO("Stencil");

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
}

void doBlendTest() {
	//blending_transparent_window.png
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
