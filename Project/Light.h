#pragma once
#include"OpenGLInit.h"

struct Light {
	glm::vec3 position;
	glm::vec3 color;
	float intensity;
	Light(glm::vec3 p, glm::vec3 c, float i = 1) :position(p), color(c), intensity(i) {}

	void move(const glm::vec3& offset) {
		position += offset;
	}

	void orbitY(const glm::vec3& center, float degrees) {
		const glm::mat4 rotation = glm::rotate(
			glm::mat4(1.f),
			glm::radians(degrees),
			glm::vec3(0.f, 1.f, 0.f)
		);

		position = center + glm::mat3(rotation) * (position - center);
	}
};
