#ifndef CAMERA_H
#define CAMERA_H


float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
float fov = 45.0f;
float aspectRatio = 1.33f;
float viewLength = 0.0f;
float viewHor = 0.0f;
float radius = 10.0f;

bool firstMouse = true;
double lastX, lastY;
float pitch = 0.0f, yaw = -90.0f;

enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	PITCHUP,
	PITCHDOWN,
	YAWUP,
	YAWDOWN,
	ROLLUP,
	ROLLDOWN,
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float ROLL = 0.0f;
const float SPEED = 7.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Euler Angles
	float Yaw;
	float Pitch;
	float Roll;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float roll = ROLL) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Roll = ROLL;
		Pitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		if (direction == UP)
			Position += Up * velocity;
		if (direction == DOWN)
			Position -= Up * velocity;

	
	}
	void ProcessKeyboardEx(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		float EulerSpeed = 20.0f;
		if (direction == PITCHUP)
			Pitch += velocity * EulerSpeed;
		if (direction == PITCHDOWN)
			Pitch -= velocity * EulerSpeed;
		if (direction == YAWUP)
			Yaw += velocity * EulerSpeed;
		if (direction == YAWDOWN)
			Yaw -= velocity * EulerSpeed;
		if (direction == ROLLUP)
			Roll += velocity * EulerSpeed;
		if (direction == ROLLDOWN)
			Roll -= velocity * EulerSpeed;
		updateCameraVectors();
	}
	
	void preProcessMouseMovement(double xpos, double ypos) {
		if (firstMouse) {
			firstMouse = false;
			lastX = xpos;
			lastY = ypos;
		}
		float offsetX = lastX - xpos;
		float offsetY = ypos - lastY;
		lastX = xpos;
		lastY = ypos;
		ProcessMouseMovement(offsetX, offsetY);
	}
	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}
protected:

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void setRollMatrix(glm::mat3x3& m, glm::vec3 f) {
		float rcos = cos(glm::radians(Roll));
		float rsin = sin(glm::radians(Roll));

		m[0][0] = rcos + (1 - rcos)*f.x*f.x;
		m[0][1] = (1 - rcos)*f.x*f.y + rsin*f.z;
		m[0][2] = (1 - rcos)*f.z*f.x - rsin*f.y;

		m[1][0] = (1 - rcos)*f.x*f.y - rsin*f.z;
		m[1][1] = rcos + (1 - rcos)*f.y*f.y;
		m[1][2] = (1 - rcos)*f.z*f.y + rsin*f.x;

		m[2][0] = (1 - rcos)*f.x*f.z + rsin*f.y;
		m[2][1] = (1 - rcos)*f.y*f.z - rsin*f.x;
		m[2][2] = rcos + (1 - rcos)*f.z*f.z;
	}
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);

		//	control the rotate view
		glm::mat3x3 rollMatrix;
		setRollMatrix(rollMatrix, front);

		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, rollMatrix * WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};

static Camera cameraInst = Camera();;

#endif CAMERA_H
