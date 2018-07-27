
glInt uboMatrices;
void initUniformBufferShader(Shader* shader) {
	//��������ɫ����Uniform������Ϊ�󶨵�0
	shader->initShader("uniformtest.vs", "uniformtest.fs");
	shader->use();
	shader->bindingUniformBlock("Matrices", 0);
	//����Uniform���������������󶨵��󶨵�0
	glGenBuffers(1, &uboMatrices);

	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	//�����ڴ�
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));
}

void fillUniformBufferShader(glInt uboMatrices, glm::mat4 projection, glm::mat4 view) {
	//��仺��
	//ͶӰ����洢��Uniform�����ǰ�벿��
	//glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)SCR_WIDTH / (float)SCR_HEIGHT), 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//ͶӰ����洢��Uniform�����ǰ�벿��
	//glm::mat4 view = cameraInst.GetViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

vector<glm::vec3> color = {
	glm::vec3(0.0, 0.0, 1.0),
	glm::vec3(1.0, 0.0, 1.0),
	glm::vec3(1.0, 0.0, 0.0),
	glm::vec3(0.0, 1.0, 0.0),
};

vector<glm::vec3> position = {
	glm::vec3(-0.75f, 0.75f, 0.0f),
	glm::vec3(0.75f, -0.75f, 0.0),
	glm::vec3(-0.75f, -0.75f, 0.0f),
	glm::vec3(0.75f, 0.75f, 0.0f),
};

void doUniformBufferShader(Shader* shader, glInt vao) {
	glBindVertexArray(vao);
	shader->use();
	for (int i = 0; i < color.size(); i++) {
		glm::mat4 model;
		model = glm::translate(model, position[i]);  // �ƶ������Ͻ�
		shader->setMat4("model", model);
		shader->setVec3("color", color[i]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void doDrawCube(Shader *shader, glInt vao) {
	glBindVertexArray(vao);
	shader->use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-0.75f, 0.75f, 0.0f));  // �ƶ������Ͻ�
	shader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void doUniformBufferShaderTest(glInt vao, glm::mat4 projection, glm::mat4 view, Shader* shader) {
	fillUniformBufferShader(uboMatrices, projection, view);
	doUniformBufferShader(shader, vao);
}
