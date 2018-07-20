#include "common.h"
#define NR_POINT_LIGHTS 4

void processInput(GLFWwindow *window);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void updateFrame();

void doAssimpShader();
void doShaderWithVAO(string shaderName, glInt vao);
void doStencilTest();
void doGrassShader();
void doBlendShader();
void doBlendTest();
void doSkyBoxTest();

void initShader(Shader* shader, const char* vs, const char* fs);
glInt initFrameBufferTest(glInt* textureColorbuffer, glInt* rbo);
void doFrameBufferTest(glInt frameBuffer);

Shader programShader, lampShader, assimpShader, stencilShader, grassShader,
		blendShader, screenShader, skyboxShader, skyObjectShader;
Uniform dirlight, material, pointLights[4];
Model assimpModel;
glm::mat4 view, projection;
glm::vec3 lightColor, objectColor;
TextureCache textureCacheInst;

vector<glInt> VAOs(3);
glInt textureColorbuffer;
bool bAssimp = false;
glInt cubemapTexture;
int main()
{
	bool bTest = false;
	if (bTest) {
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
	//glDepthMask(GL_FALSE); //禁止深度缓冲的写入
	glDepthFunc(GL_LEQUAL);  //深度缓冲测试函数
	glEnable(GL_STENCIL_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	vector<std::string> faces{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	cubemapTexture = loadCubemap(faces, "skybox");

//	glStencilMask(0xFF); // 每一位写入模板缓冲时都保持原样
	//光照
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	objectColor = glm::vec3(1.0f, 1.0f, 1.0f);
	cacheAllImage(&textureCacheInst);
	//initShader(&programShader, "texture.vs", "texture.fs");
	//initShader(&screenShader, "screen.vs", "screen.fs");
	initShader(&skyboxShader, "skybox.vs", "skybox.fs");
	//initShader(&skyObjectShader, "skyobject.vs", "skyobject.fs");
	initShader(&assimpShader, "Assimp.vs", "Assimp.fs");
	//initShader(&lampShader, "texture.vs", "light.fs");
	//initShader(&stencilShader, "texture.vs", "stencil.fs");
	//initShader(&grassShader, "texture.vs", "discard.fs");
	//initShader(&blendShader, "texture.vs", "blend.fs");
	glInt rbo;
	glInt frameBuffer = initFrameBufferTest(&textureColorbuffer, &rbo);
	dirlight = Uniform();

	//顶点
	vector<glInt>VBO(VAOs.size());
	VAOs[0] = createVerticesWithArrays(VAOs[0], &VBO[0]);
	VAOs[1] = createFrameBufferVerticesWithArrays(VAOs[1], &VBO[1]);
	VAOs[2] = createSkyBoxVerticesWithArrays(VAOs[2], &VBO[2]);

	//programShader.setFloat("material.shininess", 32.0f);
	assimpShader.setFloat("material.shininess", 32.0f);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CW);
	glEnable(GL_DEPTH_TEST);
	//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//这个只是用来进行测试的函数不是用来写入模版缓冲的函数
	while (!glfwWindowShouldClose(window))
	{
		updateFrame();
		processInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		view = cameraInst.GetViewMatrix();
		projection = glm::perspective(glm::radians(cameraInst.Zoom), aspectRatio, 0.1f, 100.0f);

		//doFrameBufferTest(frameBuffer);
		doSkyBoxTest();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	for (int i = 0; i < VAOs.size(); i++)
	{
		if(VAOs[i] == 0){
			break;
		}
		glDeleteVertexArrays(1, &VAOs[i]);
		glDeleteBuffers(1, &VBO[i]);
	}

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

//描绘灯光
void doLampShader() {
	lampShader.use();
	for (int i = 0; i < NR_POINT_LIGHTS; i++) {
		glm::mat4 model;
		model = glm::translate(model, pointLightsPosition[i]);
		useUniformMVP(lampShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

//描绘边框
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

vector<glm::vec3> vegetation = {
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

//alpha 测试
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

vector<glm::vec3> blendPos = {
	glm::vec3(-1.5f, 0.0f, -0.48f),
	glm::vec3(-1.5f, 0.0f, 0.51f),
	glm::vec3(0.0f, 0.0f, 0.7f),
};

//混合测试
void doBlendShader() {
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < blendPos.size(); i++)
	{
		float distance = glm::length(cameraInst.Position - blendPos[i]);
		sorted[distance] = blendPos[i];
	}

	//不能使用固定位置 一定要根据视点随时改变绘制顺序
	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		glm::mat4 model;
		model = glm::mat4();
		model = glm::translate(model, it->second);
		useUniformMVP(blendShader, model, view, projection);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	//绘制顺序很重要 原因深度缓冲影响导致的
}

void initShader(Shader* shader, const char* vs, const char* fs) {
	shader->initShader(vs, fs);
	shader->use();
	createUniformMVP(*shader);
	if (shader == &programShader) {
		//此处应该做一个texture 管理器 以值做key textureID 为value
		
		//vector<string> name{ "container2.png", "container2_specular.png" };
		//vector<string> textureName{ "material.diffuse", "material.specular" };
		//textureCacheInst.cacheTexture("container2.png");
		//textureCacheInst.getCacheTextureID("container2.png");
		//createTexture(*shader, name, textureName);

		programShader.setVec3("objectColor", objectColor);
		programShader.setVec3("lightColor", lightColor);
	}
	else if (shader == &assimpShader) {
		assimpModel = Model("./nanosuit_reflection/nanosuit.obj");
	}
	else if (shader == &lampShader ) {
		lampShader.setVec3("objectColor", objectColor);
		lampShader.setVec3("lightColor", lightColor);
	}
	else if (shader == &stencilShader) {
	}
	else if (shader == &grassShader)
	{
		vector<string> name{ "grass.png" };
		vector<string> textureName{ "texture1" };
		createTexture(grassShader, name, textureName, true);
	}
	else if (shader == &blendShader) {
		vector<string> name{ "blending_transparent_window.png" };
		vector<string> textureName{ "texture1" };
		createTexture(blendShader, name, textureName, true);
	}
	else if (shader == &screenShader) {
		screenShader.setInt("screenTexture", 0);
	}
}

void doProgramShader() {
	bool bStatic = true;
	//激活程序
	programShader.use();
	
	vector<string> name{ "container2.png", "container2_specular.png" };
	vector<string> textureName{ "material.diffuse", "material.specular" };
	for (int i = 0; i < name.size(); i++)
	{
		glInt textureID = textureCacheInst.getCacheTextureID(name[i].c_str());
		glActiveTexture(GL_TEXTURE0 + i);
		programShader.setFloat(textureName[i].c_str(), textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

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


void doAssimpShader() {
	dirlight.setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
	float _direction[] = { 1.0f, 1.0f, -1.0f };
	dirlight.setAttrName("direction", aToVec3(_direction));
	dirlight.bind(assimpShader, "dirLight");


	//for (int i = 0; i < NR_POINT_LIGHTS; i++) {
	//	pointLights[i].setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
	//	pointLights[i].setAttrName("position", pointLightsPosition[i]);
	//	pointLights[i].setAttenuationAttrName(1.0f, 0.09f, 0.032f);
	//	pointLights[i].bind(assimpShader, ("pointLights[" + std::to_string(i) + "]").c_str());
	//}

	glm::mat4 model;
	assimpShader.use();

	//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glActiveTexture(GL_TEXTURE3); // We already have 3 texture units active (in this shader) so set the skybox as the 4th texture unit (texture units are 0 based so index number 3)
	assimpShader.setInt("skybox", 3);
//	glUniform1i(glGetUniformLocation(shader.Program, "skybox"), 3);
	//// Now draw the nanosuit
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

	model = glm::translate(model, glm::vec3(0.0f, -15.0f, 0.0f));
	useUniformMVP(assimpShader, model, view, projection);
	assimpShader.setVec3("viewPos", cameraInst.Position);
	assimpModel.Draw(assimpShader);

	model = glm::translate(model, glm::vec3(20.0f, 0.0f, 0.0f));
	useUniformMVP(assimpShader, model, view, projection);
	assimpModel.Draw(assimpShader);
	//useUniformMVP(assimpShader, model, view, projection);
	//assimpShader.use();
	//assimpModel.Draw(assimpShader);
}
void doScreenShader() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
void doSkyBoxShader() {
	glDepthMask(GL_FALSE);
	skyboxShader.use();
	glm::mat4 model;
	glm::mat4 view = glm::mat4(glm::mat3(cameraInst.GetViewMatrix()));
	useUniformMVP(skyboxShader, model, view, projection);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
}

void doSkyObjectShader() {
	//激活程序
	skyObjectShader.use();

	vector<string> name{ "container2.png", "container2_specular.png" };
	vector<string> textureName{ "material.diffuse", "material.specular" };
	for (int i = 0; i < name.size(); i++)
	{
		glInt textureID = textureCacheInst.getCacheTextureID(name[i].c_str());
		glActiveTexture(GL_TEXTURE0 + i);
		skyObjectShader.setFloat(textureName[i].c_str(), textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	skyObjectShader.setVec3("cameraPos", cameraInst.Position);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	//skyObjectShader.setInt("cameraPos", cameraInst.Position);
	glm::mat4 model;
	model = glm::translate(model, cubePositions[0]);
	useUniformMVP(skyObjectShader, model, view, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void doShaderWithVAO(string shaderName, glInt vao) {
	glBindVertexArray(vao);
	if (shaderName.compare("Program") == 0) {
		doProgramShader();
	}
	else if (shaderName.compare("Stencil") == 0) {
		doStencilShader();
	}
	else if (shaderName.compare("Lamp") == 0) {
		doLampShader();
	}
	else if (shaderName.compare("Blend") == 0) {
		doBlendShader();
	}
	else if (shaderName.compare("Screen") == 0) {
		doScreenShader();
	}
	else if (shaderName.compare("SkyBox") == 0) {
		doSkyBoxShader();
	}
	else if (shaderName.compare("SkyObject") == 0) {
		doSkyObjectShader();
	}
	glBindVertexArray(0);
}
void doStencilTest() {
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//当测试通过的时候 才会用来进行写入模版缓冲的函数
	glStencilMask(0xFF); //启用模版写入
						 //这个函数是用来对写入的模版缓冲进行限制的函数
	doShaderWithVAO("Lamp", VAOs[0]);
	glDisable(GL_DEPTH_TEST); //禁用深度测试
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF); //没有通过模版测试 则片段会被丢弃
	glStencilMask(0x0); //禁用模版写入

	doShaderWithVAO("Stencil", VAOs[0]);

	glStencilMask(0xFF);
	glEnable(GL_DEPTH_TEST);
}

void doBlendTest() {
	//blending_transparent_window.png
	//glBlendEquation(GL_FUNC_ADD);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
glInt initFrameBufferTest(glInt* textureColorbuffer, glInt* rbo) {
	//生成帧缓冲 并绑定
	glInt framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// 生成纹理 缓冲对象
	//unsigned int texColorBuffer;
	glGenTextures(1, textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, *textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glBindTexture(GL_TEXTURE_2D, 0);
	// 将它附加到当前绑定的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, *textureColorbuffer, 0);

	//生成一个渲染缓冲对象
	//unsigned int rbo;
	glGenRenderbuffers(1, rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, *rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//将渲染缓冲对象绑定到对应的帧缓冲上
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, *rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return framebuffer;
}
void doFrameBufferTest(glInt frameBuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	doShaderWithVAO("Program", VAOs[0]);

	// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);

	screenShader.use();
	doShaderWithVAO("Screen", VAOs[1]);
}

void doSkyBoxTest() {
	//doShaderWithVAO("SkyObject", VAOs[0]);
	doAssimpShader();
	doShaderWithVAO("SkyBox", VAOs[2]);
}