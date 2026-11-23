#pragma once
#include"glm/glm.hpp"
#include"Vertex.h"
#include"Mesh.h"

struct OBB {
    glm::vec3 center;    // 월드 중심
    glm::vec3 axis[3];   // 회전된 상자의 X, Y, Z 방향
    glm::vec3 halfSize;  // 각 방향으로의 반길이
};

bool separatedOnAxis(OBB& a, OBB& b, glm::vec3& aixs);
OBB makeOBB(Mesh& mesh, glm::mat4& world);
bool intersects(OBB& a, OBB& b);