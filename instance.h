Shader instanceShader;
glInt instanceVAO;

void initInstanceTest() {
	instanceShader.initShader("instance.vs", "instance.fs", "");
	instanceShader.use();

	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	vector<float> quadVertices = {
		// λ��          // ��ɫ
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};
	glInt VBO;
	vector<int> attribute = { 2, 3 };
	instanceVAO = createVertices(instanceVAO, &VBO, quadVertices, attribute);
	glBindVertexArray(instanceVAO);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);
}

void doInstanceTest() {
	instanceShader.use();
	glBindVertexArray(instanceVAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
	glBindVertexArray(0);
}

Shader planetShader;
glInt amount = 1000;
glm::mat4 *modelMatrices;
Model planetModel, rocketModel;
void initPlanetInstanceTest() {
	planetModel = Model("./nanosuit_reflection/nanosuit.obj");
	rocketModel = Model("./nanosuit_reflection/nanosuit.obj");

	amount = 1000;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // ��ʼ���������    
	float radius = 50.0;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ��ҪС
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. ���ţ��� 0.05 �� 0.25f ֮������
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. ��ת������һ�����룩���ѡ�����ת�����������������ת
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. ��ӵ������������
		modelMatrices[i] = model;
	}
}

void doPlanetInstanceTest(glm::mat4 view, glm::mat4 projection) {
	// ��������
	planetShader.use();
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	useUniformMVP(planetShader, model, view, projection);
	planetModel.Draw(planetShader);

	// ����С����
	for (unsigned int i = 0; i < amount; i++)
	{
		useUniformMVP(planetShader, model, view, projection);
		rocketModel.Draw(planetShader);
	}
}

void doPlanetInstanceEnd() {
	delete [] modelMatrices;
}