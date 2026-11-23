#pragma once
#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glfw3.h>

class CameraManager {
public:
	glm::vec3 cameraPosition{20.f,20.f,20.f};
	glm::vec3 cameraTarget{ 0.f };
	glm::vec3 cameraUpDirection{0,1.f,0};

	bool perspectiveMode = true;
	float orthoHalfHeight = 12.f;
	float orthoMinHalfWidth = 0.f;

	// 마우스 회전 변수
	float yaw = 0.f;
	float pitch = -30.f;
	float sensitivity = 0.1f;

	bool firstMouse = true;
	double lastX, lastY;

	// 원근시의 각도
	float fov = 45.f;
	float nearClip = 0.1f;
	float farClip = 100.f;

	// 튜영 결과 
	glm::mat4 projection;

	float pitchLimit = 90.f;

public:
	CameraManager() {
		syncAnglesFromTarget();
	}

	glm::mat4 getProjection(float aspect) const {
		if (perspectiveMode) {
			// 원근 투영 (기본 값)
			return glm::perspective(glm::radians(fov), aspect, nearClip, farClip);
		}

		// 직각 투영
		const float halfHeight = glm::max(orthoHalfHeight, orthoMinHalfWidth / aspect);
		const float halfWidth = halfHeight * aspect;
		return glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, nearClip, farClip);
	}

	void syncAnglesFromTarget() {
		// 방향 값 계산
		glm::vec3 direction = glm::normalize(cameraTarget - cameraPosition);

		yaw = glm::degrees(glm::atan(direction.z, direction.x));
		pitch = glm::degrees(glm::asin(glm::clamp(direction.y, -1.f, 1.f)));
	}

	glm::mat4 getView() const {
		return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
	}

	void move(glm::vec3& offset) {
		cameraPosition += offset;
		cameraTarget += offset;
	}

	void mouseLook(float dx, float dy) {
		yaw += dx * sensitivity;
		pitch += dy * sensitivity;

		// pitch 값 clamp
		pitch = glm::clamp(pitch, -pitchLimit, pitchLimit);
		
		float y = glm::radians(yaw);
		float p = glm::radians(pitch);

		glm::vec3 direction{
			glm::cos(p) * glm::cos(y),
			glm::sin(p),
			glm::cos(p) * glm::sin(y)
		};

		cameraTarget = cameraPosition + direction;
	}

	void cameraMousePosCallback(GLFWwindow* window, double x, double y) {
		if (firstMouse) {
			lastX = x;
			lastY = y;
			firstMouse = false;
			return;
		}

		float dx = float(x - lastX);
		float dy = float(lastY - y);

		lastX = x;
		lastY = y;

		mouseLook(dx, dy); 
	}

	void zoom(float amount) {
		glm::vec3 direction =glm::normalize(cameraTarget - cameraPosition);

		float distance = glm::length(cameraTarget - cameraPosition);
		distance = glm::max(distance - amount, 0.5f);
		cameraPosition = cameraTarget - direction * distance;

		orthoHalfHeight = distance * glm::tan(glm::radians(fov * 0.5f));
	}

	void orbitY(float degrees) {
		glm::mat4 rotation = glm::rotate(glm::mat4(1.f),glm::radians(degrees),glm::vec3(0.f, 1.f, 0.f));

		cameraPosition = glm::vec3(rotation * glm::vec4(cameraPosition, 1.f));
		cameraTarget = glm::vec3(rotation * glm::vec4(cameraTarget, 1.f));

		syncAnglesFromTarget();
	}
};