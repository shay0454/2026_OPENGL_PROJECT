#pragma once

#include "Model.h"
#include "ParsingObj.h"
#include "Random.h"

#include <cmath>
#include <string>
#include <vector>

class LowPolyTank : public Model {
public:
    // 무기 형태
    enum class Weapon { SingleShot, Automatic, Cannon };

    glm::vec3 moveDirection = { 0,0,0 };

private:
    const int trackLinksPerSide = 24;
    const int wheelsPerSide = 4;
    MeshOwner hull;
    MeshOwner tracks;
    std::vector<MeshOwner> trackLinks = std::vector<MeshOwner>(trackLinksPerSide * 2);
    std::vector<MeshOwner> wheels = std::vector<MeshOwner>(wheelsPerSide * 2);
    MeshOwner turret;
    MeshOwner hatch;
    MeshOwner topLamp;

    Light topLight{ glm::vec3(0.f), glm::vec3(1.f, 0.9f, 0.76f), 1.f };
    std::vector<MeshOwner> barrels = std::vector<MeshOwner>(3);
    Weapon activeWeapon = Weapon::SingleShot;
    float rideHeight = 0.f;
    float bodyYaw = 0.f;
    float turretYaw = 0.f;
    float moveSpeed = 2.f;
    float wheelRotation = 0.f;
    float trackDistance = 0.f;

public:
    LowPolyTank(ParsingObj& loader)
        : Model(std::vector<Mesh*>{}),
          hull(loadMesh(loader, "assets/models/tank/tank_hull.obj", {0.49f, 0.55f, 0.23f})),
          tracks(loadMesh(loader, "assets/models/tank/tank_track_belt.obj", {0.19f, 0.19f, 0.19f})),
          turret(loadMesh(loader, "assets/models/tank/tank_turret.obj", {0.52f, 0.58f, 0.25f})),
          hatch(loadMesh(loader, "assets/models/tank/tank_hatch.obj", {0.37f, 0.44f, 0.17f})),
          topLamp(loadMesh(loader, "assets/models/primitives/tetrahedron.obj", {1.f, 0.9f, 0.76f})) {
        turret->parentMesh = hull.get();
        hatch->parentMesh = turret.get();

        topLamp->parentMesh = turret.get();
        topLamp->move({ 0.f, 0.23f, 0.08f });
        topLamp->scale(0.025f);

        const char* weaponFiles[] = {
            "assets/models/tank/tank_barrel_single.obj",
            "assets/models/tank/tank_barrel_automatic.obj",
            "assets/models/tank/tank_barrel_cannon.obj"
        };
        for (int i = 0; i < barrels.size(); i++) {
            barrels[i] = loadMesh(loader, weaponFiles[i], {0.37f, 0.43f, 0.17f});
            barrels[i]->parentMesh = turret.get();
        }

        for (auto& wheel : wheels) {
            wheel = loadMesh(loader, "assets/models/tank/tank_wheel.obj", {0.29f, 0.33f, 0.14f});
        }
        
        // 탱크 트랙 생성
        MeshOwner linkSource = loadMesh(loader, "assets/models/tank/tank_track_link.obj", {0.28f, 0.28f, 0.28f});
        for (auto& link : trackLinks) {
            link = std::make_unique<Mesh>(linkSource->vertices, linkSource->indices, linkSource->textures);
            link->color = linkSource->color;
        }

        // model에 관련 메시들 추가
        meshes = { hull.get(), tracks.get(), turret.get(), hatch.get()};
        for (auto& link : trackLinks) {
            meshes.push_back(link.get());
        }
        for (auto& wheel : wheels) {
            meshes.push_back(wheel.get());
        }
        meshes.push_back(barrels.front().get());
        updatePartTransforms();
    }

    void update(float deltaTime) override {
        moveTank(deltaTime);
        syncTopLightPosition();
    }

    float getMoveSpeed() const { return moveSpeed; }

    void setMoveSpeed(float speed) {
        moveSpeed = glm::max(0.f, speed);
    }

    void setWeapon(Weapon weapon) {
        int index = (int)weapon;
        if (!(index >= 0 && index < barrels.size())) return;

        for (int i = 0; i < meshes.size(); ++i) {
            if (meshes[i] == barrels[(int)activeWeapon].get()) {
                meshes[i] = barrels[index].get();
                activeWeapon = weapon;
                return;
            }
        }
    }
    Weapon getWeapon() const { return activeWeapon; }

    // 조명 위치 업데이트
    void syncTopLightPosition() {
        glm::mat4 modelPose = transform * glm::translate(glm::mat4(1.f), -origin);
        topLight.position = glm::vec3(topLamp->getWorldTransform(modelPose)[3]);
    }
    
    Light& getTopLight() { return topLight; }

    glm::vec3 getTopLightPosition() {
        glm::mat4 modelPose = transform * glm::translate(glm::mat4(1.f), -origin);
        return glm::vec3(topLamp->getWorldTransform(modelPose)[3]);
    }

    void move(const glm::vec3& offset) {
        transform[3] += glm::vec4(offset, 0.f);
    }

    void setBodyYaw(float degrees) {
        if (bodyYaw == degrees) return;

        const glm::vec4 position = transform[3];
        const auto rotation = glm::rotate(glm::mat4(1.f), glm::radians(degrees - bodyYaw), glm::vec3(0.f, 1.f, 0.f));
        transform = rotation * transform;
        transform[3] = position;
        bodyYaw = degrees;
    }

private:

    void moveTank(float deltaTime) {
        if (moveDirection == glm::vec3(0.f)) return;

        // 이동 방향 설정
        const glm::vec3 direction = glm::normalize(moveDirection);
        const float yaw = glm::degrees(std::atan2(-direction.x, -direction.z));
        setBodyYaw(yaw);
        move(direction * moveSpeed * deltaTime);

        // 모델이 향하는 방향으로 굴러가도록 실제 이동 거리만큼 회전
        const float wheelRadius = 0.083f;
        wheelRotation = std::fmod(wheelRotation - moveSpeed * deltaTime / wheelRadius,glm::two_pi<float>());
        trackDistance -= moveSpeed * deltaTime;
        updatePartTransforms();
    }

    static MeshOwner loadMesh(ParsingObj& loader, const char* filename, glm::vec3 color) {
        MeshOwner mesh(loader.loadMeshlFromObj(filename));
        if (mesh) {
            mesh->color = color;
        }
        return mesh;
    }

    void updateTrackLinks(const glm::mat4& trackTransform) {
        // 각 점은 (z, y) 순서이며, 이동 시 경로를 역방향으로 순환
        static const std::vector<glm::vec2> path = {
            { 0.30f, 0.23f }, { -0.31f, 0.23f }, { -0.36f, 0.23f },
            { -0.40f, 0.20f }, { -0.42f, 0.15f }, { -0.42f, 0.10f },
            { -0.40f, 0.06f }, { -0.36f, 0.03f }, { -0.31f, 0.02f },
            { 0.30f, 0.02f }, { 0.35f, 0.03f }, { 0.39f, 0.06f },
            { 0.41f, 0.10f }, { 0.41f, 0.15f }, { 0.39f, 0.20f },
            { 0.35f, 0.23f }
        };

        // 각 경로 구간의 길이와 궤도 전체 둘레를 계산
        // 마지막 점은 첫 번째 점과 연결
        std::vector<float> lengths(path.size());
        float perimeter = 0.f;
        for (int i = 0; i < static_cast<int>(path.size()); ++i) {
            lengths[i] = glm::length(path[(i + 1) % path.size()] - path[i]);
            perimeter += lengths[i];
        }
        // 이동 거리를 한 바퀴 범위로 줄이고, 음수인 경우 양수 위치로 변경
        trackDistance = std::fmod(trackDistance, perimeter);
        if (trackDistance < 0.f) trackDistance += perimeter;

        const float trackCenterX = 0.265f;
        for (int i = 0; i < static_cast<int>(trackLinks.size()); ++i) {
            // 좌우 궤도에 마디를 같은 간격으로 배치하고 이동 거리를 더함
            float distance = std::fmod(trackDistance + perimeter * static_cast<float>(i % trackLinksPerSide) / trackLinksPerSide, perimeter);
            int segment = 0;
            // 지나온 구간의 길이를 빼서 마디가 현재 놓인 구간을 찾는다
            while (segment + 1 < static_cast<int>(path.size()) && distance >= lengths[segment]) {
                distance -= lengths[segment++];
            }
            // 구간의 시작점과 남은 거리를 이용해 마디 위치와 회전 각도를 계산
            const glm::vec2 direction = path[(segment + 1) % path.size()] - path[segment];
            const glm::vec2 point = path[segment] + direction * (distance / lengths[segment]);
            const float angle = std::atan2(-direction.y, direction.x);
            const float x = i < trackLinksPerSide ? -trackCenterX : trackCenterX;
            glm::mat4 pose = glm::translate(glm::mat4(1.f), glm::vec3(x, point.y, point.x));
            pose = glm::rotate(pose, angle, glm::vec3(1.f, 0.f, 0.f));
            trackLinks[i]->transform = trackTransform * pose;
        }
    }

    void updatePartTransforms() {
        const glm::mat4 identity(1.f);
        const glm::vec3 xAxis(1.f, 0.f, 0.f);
        const glm::vec3 yAxis(0.f, 1.f, 0.f);
        // 각 부품이 부모 메시를 기준으로 붙는 위치
        const glm::vec3 turretSeat(0.f, 0.25f, 0.f);
        const glm::vec3 barrelSocket(0.f, 0.10f, -0.18f);
        const glm::vec3 hatchSeat(0.f, 0.20f, 0.06f);
        const float trackHeight = 0.21f;
        const float wheelCenterX = 0.32f;
        const float firstWheelZ = -0.30f;
        const float wheelSpacing = 0.20f;

        // 차체 높이에 맞춰 궤도 높이와 궤도 마디를 갱신한다.
        hull->transform = glm::translate(identity, glm::vec3(0.f, rideHeight, 0.f));
        const glm::vec3 trackScale(1.f, 1.f + rideHeight / trackHeight, 1.f);
        tracks->transform = glm::scale(identity, trackScale);
        updateTrackLinks(tracks->transform);
        // 좌우 바퀴를 같은 간격으로 놓고 이동에 따른 회전을 적용한다.
        for (int i = 0; i < wheels.size(); i++) {
            const float side = i < wheelsPerSide ? -1.f : 1.f;
            const float z = firstWheelZ + (i % wheelsPerSide) * wheelSpacing;
            glm::mat4 wheelPose = glm::translate(identity, glm::vec3(side * wheelCenterX, 0.12f, z));
            wheelPose = glm::rotate(wheelPose, wheelRotation, xAxis);
            // 왼쪽 바퀴 메시를 반전해 오른쪽에도 사용한다.
            if (side > 0.f) wheelPose = glm::scale(wheelPose, glm::vec3(-1.f, 1.f, 1.f));
            wheels[i]->transform = tracks->transform * wheelPose;
        }

        // 포신과 해치는 포탑을 부모로 사용하므로 포탑과 함께 회전한다.
        turret->transform = glm::rotate(glm::translate(identity, turretSeat), glm::radians(turretYaw), yAxis);
        const glm::mat4 barrelPose = glm::translate(identity, barrelSocket);
        for (auto& barrel : barrels) {
            barrel->transform = barrelPose;
        }
        hatch->transform = glm::translate(identity, hatchSeat);
    }
};

