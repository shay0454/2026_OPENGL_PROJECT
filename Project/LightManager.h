#pragma once
#include"OpenGLInit.h"
#include<vector>
#include<string>
#include"Light.h"
#include<algorithm>

class LightManager {
public:
	std::vector<Light*> lights;
	const int lightLimit = 16;

public:
	LightManager() {}
	void add(Light& light) {
		if (lights.size() >= lightLimit) return;
		lights.emplace_back(&light);
	}

	void remove(Light& light) {
		auto iter = std::find(lights.begin(), lights.end(), &light);
		if(iter !=lights.end())
			lights.erase(iter);
	}

	void uploadLights();

};
