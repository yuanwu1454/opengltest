//可能是cubeTexture 与 vao 错误设置的问题
Model skyboxModel;
Shader skyBoxModelShader, skyboxShader, cubeShader;
glInt cubemapTexture;
glInt skyboxVAO, cubeVAO;


void initSkyBoxTest() {
	vector<std::string> faces{
		"right.jpg",
		"left.jpg",
		"top.jpg",
		"bottom.jpg",
		"back.jpg",
		"front.jpg"
	};
	cubemapTexture = loadCubemap(faces, "skybox");
	glEnable(GL_DEPTH_TEST);

	glInt VBO[2];
	skyboxVAO = createSkyBoxVerticesWithArrays(skyboxVAO, &VBO[0]);
	cubeVAO = createVerticesWithArrays(cubeVAO, &VBO[1]);


	skyboxShader.initShader("skybox.vs", "skybox.fs");
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	cubeShader.initShader("skyobject.vs", "skyobject.fs");
	cubeShader.use();
	cubeShader.setInt("iReflect", 0);

	//先导入模型 再导入天空盒 会将对应的天空盒 的位置发生错乱 以后有能力之后再来查找这个问题
	skyboxModel = Model("./nanosuit_reflection/nanosuit.obj");
	skyBoxModelShader.initShader("Assimp.vs", "Assimp.fs");
	skyBoxModelShader.setFloat("material.shininess", 32.0f);
	skyBoxModelShader.setInt("skybox", 3);
}

void doSkyBoxTest(glm::mat4 view, glm::mat4 projection ) {
	skyBoxModelShader.use();
	glm::mat4 model;
	Uniform dirlight = Uniform();
	dirlight.setLightAttrName(aToVec3(ambientlight), aToVec3(diffuselight), aToVec3(specularlight));
	float _direction[] = { 1.0f, 1.0f, -1.0f };
	dirlight.setAttrName("direction", aToVec3(_direction));
	dirlight.bind(skyBoxModelShader, "dirLight");

	glActiveTexture(GL_TEXTURE3);
	skyBoxModelShader.setInt("skybox", 3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	useUniformMVP(skyBoxModelShader, model, view, projection);
	skyBoxModelShader.setVec3("viewPos", cameraInst.Position);
	skyboxModel.Draw(skyBoxModelShader);


	//cube
	cubeShader.use();
	cubeShader.setVec3("cameraPos", cameraInst.Position);
	glBindVertexArray(cubeVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	useUniformMVP(cubeShader, glm::translate(model, cubePositions[0]), view, projection);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	//skybox
	glDepthFunc(GL_LEQUAL);
	skyboxShader.use();

	glm::mat4 skyModel;
	glm::mat4 skyView = glm::mat4(glm::mat3(view));
	useUniformMVP(skyboxShader, skyModel, skyView, projection);

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}