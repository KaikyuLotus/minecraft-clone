#ifndef CAMERA_H
#define CAMERA_H

#include "cube.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	DOWN,
	UP
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 7.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 70.0f;
const float GRAVITY = 15.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	float jumpPower = 4.0;

	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	float Gravity;

	bool isFlying;
	bool hasJumped;
	bool jumping;
	double jumpTime;

	// Constructor with vectors
	Camera(glm::vec3 position, float pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM){
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		Position = position;
		WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		Yaw = YAW;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix() {
		return glm::lookAt(Position, Position + Front, Up);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime, bool shiftPressed = false) {
		// std::cout << Front.x << " " << Front.y << " " << Front.z << std::endl;

		float velocity = MovementSpeed * deltaTime;

		if (shiftPressed) velocity *= 2;

		if (direction == FORWARD) {
			glm::vec3 tmp = glm::vec3(Front.x, 0.0f, Front.z);
			Position += tmp * velocity;
		}
		if (direction == BACKWARD) {
			glm::vec3 tmp = glm::vec3(Front.x, 0.0f, Front.z);
			Position -= tmp * velocity;
		}
		if (direction == LEFT) {
			glm::vec3 tmp = glm::vec3(Right.x, 0.0f, Right.z);
			Position -= tmp * velocity;
		}
		if (direction == RIGHT) {
			glm::vec3 tmp = glm::vec3(Right.x, 0.0f, Right.z);
			Position += tmp * velocity;
		}

		if (direction == UP) {
			Position += WorldUp * velocity;
		}

		/*if (!isFlying) {
			if (direction == UP) {
				if (!hasJumped) {
					// Position += WorldUp * (jumpPower);
					hasJumped = true;
					jumpTime = glfwGetTime();
					jumping = true;
				}
			}
		}*/
		if (direction == DOWN)
			Position -= WorldUp * velocity;
	}

	glm::vec3 getFront() {
		return Front;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true) {
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw = std::fmod((Yaw + xoffset), (GLfloat)360.0f);
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch) {
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset) {
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

	void applyGravity(float deltaTime, std::vector<Cube>& cubes) {
		bool falling = false;

		if (jumping) {
			double time = glfwGetTime();
		 	if (time - jumpTime <= 0.2) {
				Position += WorldUp * jumpPower * deltaTime;
				return;
			} else if (time - jumpTime <= 2) {
				falling = true;
			} else {
				falling = false;
				jumping = false;
			}

			
		}

		for (int i = 0; i < cubes.size(); i++) {
			glm::vec3 position = cubes[i].getPosition();
			float yDistance = Position.y - position.y;
			if (yDistance < 3) {
				float distance = Position.x - position.x;
				if (distance >= 0.0f && distance <= 1) {
					distance = Position.z - position.z;
					if (distance >= 0 && distance <= 1) {
						if (yDistance < 10) {
							isFlying = false;
							hasJumped = false;
							return;
						}
					}
				}
				
			}
		}

		isFlying = true;
		if (falling) Position -= WorldUp * jumpPower * deltaTime;
		else Position -= WorldUp * GRAVITY * deltaTime;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors() {
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		
		/// Front = glm::normalize(glm::cross(Front, WorldUp));
		
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif