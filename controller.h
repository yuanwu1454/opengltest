#ifndef CONTROLL_H
#define CONTROLL_H
float ambientlight[3] = { 1.0f, 1.0f, 1.0f };
float diffuselight[3] = { 1.0f, 1.0f, 1.0f };
float specularlight[3] = { 1.0f, 1.0f, 1.0f };
float direction[3] = { 1.0f, 1.0f, 1.0f };

float *curlight = ambientlight;
string lastlight = "ambientlight";
int pointIdx = 0;
float dirSpeed = 5.0f;


glm::vec3 pointLightsPosition[4] = {
	glm::vec3(1.0f,  0.0f,  1.0f),
	glm::vec3(1.0f,  6.0f,  1.0f),
	glm::vec3(.0f,  0.0f,  5.0f),
	glm::vec3(1.0f,  0.0f,  1.0f)
};
glm::vec3 lightPos(-0.0973745, 0.420927, -5.06509);
float pointPosition[3] = { 0.0f, 0.0f, 0.0f };
bool checkValid(int action) {
	return action == GLFW_REPEAT || action == GLFW_PRESS;
}

void keyDelegate(GLFWwindow *window, int key, int scancode, int action, int mods) {
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

	if ((key == GLFW_KEY_1 || key == GLFW_KEY_2 || key == GLFW_KEY_3) && (checkValid(action)))
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
		std::cout << lastlight << "  " << *(curlight) << "  " << *(curlight + 1) << "  " << *(curlight + 2) << std::endl;
	}
	if (key == GLFW_KEY_F) {
		if (mods == GLFW_MOD_SHIFT)
			cameraInst.ProcessKeyboardEx(Camera_Movement::PITCHUP, deltaTime);
		if (mods == GLFW_MOD_CONTROL)
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
void inputDelegate(GLFWwindow *window) {
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
#endif