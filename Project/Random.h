#pragma once
#include<random>
#include<glm/glm.hpp>

class Random {
public:
	static std::mt19937& engine();
	static int integer(int min, int max);
	static glm::vec3 randomColor();
	static float flt(float min, float max);
};