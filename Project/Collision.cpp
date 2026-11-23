#pragma once
#include"Collision.h"

OBB makeOBB(Mesh& mesh, glm::mat4& world) {
    OBB box;

    if (mesh.vertices.empty())
        return box;

    // mesh 자체의 최소·최대 좌표
    glm::vec3 minPos = mesh.vertices.front().position;
    glm::vec3 maxPos = minPos;

    for (const Vertex& vertex : mesh.vertices) {
        minPos = glm::min(minPos, vertex.position);
        maxPos = glm::max(maxPos, vertex.position);
    }

    glm::vec3 localCenter = (minPos + maxPos) * 0.5f;
    glm::vec3 localHalfSize = (maxPos - minPos) * 0.5f;

    // Mesh::draw()와 동일한 origin 보정
    glm::mat4 final =
        world * glm::translate(glm::mat4(1.f), -mesh.origin);

    box.center = glm::vec3(
        final * glm::vec4(localCenter, 1.f)
    );

    for (int i = 0; i < 3; ++i) {
        glm::vec3 direction(final[i]);
        float scale = glm::length(direction);

        if (scale > 0.f)
            box.axis[i] = direction / scale;

        box.halfSize[i] = localHalfSize[i] * scale;
    }

    return box;
}

bool separatedOnAxis(OBB& a,OBB& b,glm::vec3& axis) {
    float lengthSquared = glm::dot(axis, axis);

    // 평행한 축끼리 외적하면 방향이 거의 0이 된다.
    if (lengthSquared < 1e-12f)
        return false;

    float distance =
        std::abs(glm::dot(b.center - a.center, axis));

    float radiusA = 0.f;
    float radiusB = 0.f;

    for (int i = 0; i < 3; ++i) {
        radiusA += a.halfSize[i]
            * std::abs(glm::dot(a.axis[i], axis));

        radiusB += b.halfSize[i]
            * std::abs(glm::dot(b.axis[i], axis));
    }

    float tolerance = 1e-5f * std::sqrt(lengthSquared);

    return distance > radiusA + radiusB + tolerance;
}

bool intersects(OBB& a, OBB& b) {
    // 각 상자의 축 3개씩
    for (int i = 0; i < 3; ++i) {
        if (separatedOnAxis(a, b, a.axis[i]))
            return false;

        if (separatedOnAxis(a, b, b.axis[i]))
            return false;
    }

    // 두 상자의 축을 외적한 방향 9개
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            glm::vec3 axis =
                glm::cross(a.axis[i], b.axis[j]);

            if (separatedOnAxis(a, b, axis))
                return false;
        }
    }

    return true;
}