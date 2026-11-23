#pragma once
#include "OpenGLInit.h"
#include <array>
#include<vector>

// 세 실수 저장용 자료형
using Arr3 = std::array<GLuint, 3>;

using Position = glm::vec3;
using UV = glm::vec2;
using Normal = glm::vec3;

typedef struct Vertex {
	Position position;
	Normal Normal;
	UV TexCoords;
};

typedef struct Face {
	Arr3 vertexIdx;
	Arr3 uv;
	Arr3 normalIdx;
	glm::vec3 color{ 1.f };
};
