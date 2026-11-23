#include"Random.h"

std::mt19937& Random::engine() {
	static std::mt19937 gen(std::random_device{}());
	return gen;
}

int Random::integer(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine());
}

glm::vec3 Random::randomColor() {
	std::uniform_real_distribution<float> dist(0.f, 1.f);
	return{ dist(engine()),dist(engine()),dist(engine()) };
}

float Random::flt(float min, float max) {
	std::uniform_real_distribution<float> flt(min, max);
	return flt(engine());
}