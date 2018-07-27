Shader antiShader, bufferShader;
glInt antiVAO[2], antiVBO[2];
CCVertex* antiVertex;
void initAntiAlias() {
	glfwWindowHint(GLFW_SAMPLES, 4);
	antiVertex = CCVertex::create();
	createCellCubeWithArrays(antiVertex);
	antiShader.initShader("anti.vs", "anti.fs");
	//mat4:
	antiShader.use();
	antiShader.setVec3("color", glm::vec3(0.0, 1.0, 0.0));
	antiShader.setMat4("model", model);
}

void doAntiAlias(glm::mat4 view, glm::mat4 projection) {
	glEnable(GL_MULTISAMPLE);
	antiShader.use();
	useUniformVP(antiShader, view, projection);
	antiVertex->Draw();
}

//多重采样 帧缓冲
glInt aliaFrameBuffer, aliaTextureColorBuffer, aliaRboBuffer, intermediateFBO, screenBufferTexture;
void initAliaFrameBuffer() {
	bufferShader.initShader("buffer.vs", "buffer.fs");
	bufferShader.use();
	bufferShader.setInt("screenTexture", 0);
	antiVAO[1] = createFrameBufferVerticesWithArrays(antiVAO[1], &antiVBO[1]);

	//生成帧缓冲 并绑定
	glGenFramebuffers(1, &aliaFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, aliaFrameBuffer);

	// 生成纹理 缓冲对象
	glGenTextures(1, &aliaTextureColorBuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, aliaTextureColorBuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, SCR_WIDTH, SCR_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, aliaTextureColorBuffer, 0);

	//生成一个渲染缓冲对象
	glGenRenderbuffers(1, &aliaRboBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, aliaRboBuffer);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, aliaRboBuffer);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// configure second post-processing framebuffer
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
	// create a color attachment texture
	glGenTextures(1, &screenBufferTexture);
	glBindTexture(GL_TEXTURE_2D, screenBufferTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenBufferTexture, 0);	// we only need a color buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Intermediate framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void doAliaFrameBuffer() {
	glBindVertexArray(antiVAO[1]);
	bufferShader.use();
	glActiveTexture(GL_TEXTURE0);
	//这个置换成 screenBufferTexture 可以提高视觉质量
	glBindTexture(GL_TEXTURE_2D, aliaTextureColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void initAliaFrameBufferTest() {
	initAntiAlias();
	initAliaFrameBuffer();
}

void doAliaFrameBufferTest(glm::mat4 view, glm::mat4 projection) {
	glBindFramebuffer(GL_FRAMEBUFFER, aliaFrameBuffer);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	doAntiAlias(view, projection);

	//因为多重采样缓冲有一点特别，我们不能直接将它们的缓冲图像用于其他运算，比如在着色器中对它们进行采样。
	// 2. now blit multisampled buffer(s) to normal colorbuffer of intermediate FBO. Image is stored in screenTexture
	glBindFramebuffer(GL_READ_FRAMEBUFFER, aliaFrameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
	glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
							  // clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);
	doAliaFrameBuffer();
}