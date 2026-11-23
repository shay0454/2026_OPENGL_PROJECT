#pragma once
#include"Vertex.h"
#include"Mesh.h"
#include"Collision.h"
#include<iostream>
#include<map>
class Model {
public:
	std::vector<Mesh*> meshes;

	glm::vec3 origin{ 0.f };
	glm::mat4 transform{ 1.0f };


	Model(std::vector<Mesh*> m) :meshes(m){}
	Model(std::string sourceFileName) { loadModel(sourceFileName); }
	virtual void draw();
	virtual void update(float deltaTime) {}
	void updateBuffer();
	void loadModel(std::string sourceFileName);
};

class Conveyor : public Model {
public:
	std::vector<Mesh*> slats;
	Conveyor* nextTrack = this;
	float offset;
	float spacing;
	float length;
	glm::vec3 start;
	glm::vec3 end;

	float conveySpeed = 1.f;

	Conveyor() : Model(std::vector<Mesh*>{}),
		offset(0.f),
		spacing(0.5f),
		start(0.f),
		end(0.f)
	{
		// 실제 시작점·도착점 설정과 슬랫 생성
	}
	void setStartAndEnd(Mesh* convey) {
		length = 6.f;
		float beltHeight = 1.f;

		glm::vec3 center{ 0.f, beltHeight + 0.003f, 0.f };
		glm::vec3 halfLength{ 0.f, 0.f, length * 0.5f };

		start = center + halfLength;
		end = center - halfLength;
	}

	void update(float deltaTime) override {
		// 모델의 확대·축소까지 반영한 트랙 길이
		length = glm::length(glm::vec3(
			transform * glm::vec4(end - start, 0.f)));

		offset += conveySpeed * deltaTime / length;

		while (offset >= 1.f) {
			offset -= 1.f;
		}

		for (int i = 0; i < slats.size(); ++i) {
			float t = offset + float(i) / float(slats.size());

			if (t >= 1.f) {
				t -= 1.f;
			}
			slats[i]->transform = getTrackTransform(t);
			
		}
	}

	virtual glm::mat4 getTrackTransform(float phaseT) const {
		return glm::translate(
			glm::mat4(1.f), glm::mix(start, end, phaseT));
	}
};

class CornerConveyor : public Conveyor {
public :
	float radius = 1.f;
	glm::vec3 pivot;
	CornerConveyor() {
		start = glm::vec3(0.f, 1.003f, 1.f);
		end = glm::vec3(1.f, 1.003f, 0.f);
	}

	void update(float deltaTime) override {
		float worldRadius = glm::length(glm::vec3(
			transform * glm::vec4(radius, 0.f, 0.f, 0.f)));

		length = worldRadius * glm::radians(90.f);
		if (length <= 0.f) return;

		offset += conveySpeed * deltaTime / length;

		while (offset >= 1.f) offset -= 1.f;
		while (offset < 0.f)  offset += 1.f;

		// 현재 코너의 안쪽 회전 중심
		pivot = start + glm::vec3(radius, 0.f, 0.f);

		for (int i = 0; i < slats.size(); ++i) {
			float t = offset + float(i) / float(slats.size());

			if (t >= 1.f) t -= 1.f;
			
			slats[i]->transform = getTrackTransform(t);
		}
	}

	glm::mat4 getTrackTransform(float phaseT) const override {
		glm::vec3 center = start + glm::vec3(radius, 0.f, 0.f);

		return glm::translate(glm::mat4(1.f), center)
			* glm::rotate(
				glm::mat4(1.f),
				glm::radians(-90.f * phaseT),
				glm::vec3(0.f, 1.f, 0.f))
			* glm::translate(
				glm::mat4(1.f), glm::vec3(-radius, 0.f, 0.f));
	}
};

class BoxModel : public Model {
public:
	Conveyor* curTrack = nullptr;
	float t = 0.f;

	BoxModel() : Model(std::vector<Mesh*>{}) {}

	void update(float deltaTime) override {
		if (!curTrack) return;
		t += curTrack->conveySpeed * deltaTime/ curTrack->length;
		while (t > 1) {
			float remainingTime =(t - 1.f) * curTrack->length / curTrack->conveySpeed;

			curTrack = curTrack->nextTrack;

			// 남은 시간을 다음 트랙의 진행도로 변환
			t = remainingTime * curTrack->conveySpeed/ curTrack->length;
		}

		this->transform = curTrack->transform;

		for (auto mesh : meshes) {
			transform =curTrack->transform * curTrack->getTrackTransform(t);
		}
	}
};


class BallModel : public Model {
public:
	Mesh* room = nullptr;

	glm::vec3 position{ 0.f };
	glm::vec3 velocity{ 0.2f, 0.3f, -0.15f };
	float radius = 0.04f;

	BallModel() : Model(std::vector<Mesh*>{}) {}

	void update(float deltaTime) override {
		if (!room) return;

		// 월드 좌표에서 공 이동
		position += velocity * deltaTime;

		glm::mat4 roomToWorld = room->transform;
		glm::mat4 worldToRoom = glm::inverse(roomToWorld);

		// 충돌 검사를 위해 방 기준 좌표로 변환
		glm::vec3 localPosition = glm::vec3(
			worldToRoom * glm::vec4(position, 1.f));

		glm::vec3 localVelocity = glm::vec3(
			worldToRoom * glm::vec4(velocity, 0.f));

		float minPos = -0.5f + radius;
		float maxPos = 0.5f - radius;

		for (int axis = 0; axis < 3; ++axis) {
			if (localPosition[axis] > maxPos) {
				localPosition[axis] = maxPos;
				localVelocity[axis] = -glm::abs(localVelocity[axis]);
			}
			else if (localPosition[axis] < minPos) {
				localPosition[axis] = minPos;
				localVelocity[axis] = glm::abs(localVelocity[axis]);
			}
		}

		// 보정한 위치와 반사 방향을 월드 좌표로 복원
		position = glm::vec3(
			roomToWorld * glm::vec4(localPosition, 1.f));

		velocity = glm::vec3(
			roomToWorld * glm::vec4(localVelocity, 0.f));

		transform = glm::translate(glm::mat4(1.f), position);
	}
};

struct Node {
	glm::vec3 position;
	glm::vec3 rotation;

	Mesh* mesh;
	glm::mat4 wolrdTransform;

	std::vector<Node*> child;

	Node(Mesh* mesh, glm::vec3 pos) :mesh(mesh), position(pos) {}
};

class Robot : public Model {
public:
	Node* root;
	std::map<std::string, Node*> parts;

	glm::vec3 moveDirection{ 0.f };
	glm::vec3 position{ 0.f };

	float moveSpeed = 2.f;
	float walkTime = 0.f;
	float heading = 0.f;

	float verticalSpeed = 0.f;
	bool onGround = true;


	Robot() : Model(std::vector<Mesh*>{}), root(nullptr) {}
public:
	Node* getJoint(std::string name) {
		return parts.at(name);
	}
	void draw() override;
	void resigt(std::string name, Node* node) {
		parts.emplace(name, node);
	}
	void update(float deltaTime) override {
		bool moving =glm::dot(moveDirection, moveDirection) > 0.f && moveSpeed > 0.f;
		if (!onGround) {
			verticalSpeed -= 20.f * deltaTime; // 중력
			position.y += verticalSpeed * deltaTime;

			if (position.y <= 0.f) {
				position.y = 0.f;
				verticalSpeed = 0.f;
				onGround = true;
			}
		}
		if (moving) {
			// 대각선으로 움직여도 속도가 같도록 정규화
			glm::vec3 direction = glm::normalize(moveDirection);

			position += direction * moveSpeed * deltaTime;

			// 이동 방향을 바라보도록 회전
			heading = std::atan2(direction.x, direction.z);

			walkTime += deltaTime * moveSpeed * 3.5f;
		}
		else {
			walkTime = 0.f;
		}

		// 로봇 전체의 이동과 방향
		transform =
			glm::translate(glm::mat4(1.f), position)
			* glm::rotate(
				glm::mat4(1.f),
				heading,
				glm::vec3(0.f, 1.f, 0.f));

		float maxAngle = moveSpeed * 15.f;

		float swing = moving
			? std::sin(walkTime) * maxAngle
			: 0.f;

		// 기존 연결 위치를 기준으로 매번 새 행렬 계산
		auto swingJoint = [this](const std::string& name, float angle) {
			Node* node = getJoint(name);

			node->rotation.x = angle;

			node->mesh->transform =
				glm::translate(glm::mat4(1.f), node->position)
				* glm::rotate(
					glm::mat4(1.f),
					glm::radians(angle),
					glm::vec3(1.f, 0.f, 0.f));
			};

		swingJoint("leftShoulder", swing);
		swingJoint("rightShoulder", -swing);
		swingJoint("leftHip", -swing);
		swingJoint("rightHip", swing);

	}
	void updateWorldTransform(Node* node, const glm::mat4& parentWorld) {
		node->mesh->transform = parentWorld * node->mesh->transform;
		for (Node* ch : node->child) {
			updateWorldTransform(ch, node->mesh->transform);
		}
	}
	void drawNode(Node* node, const glm::mat4& parentWorld) {
		// 결과만 임시로 계산 — mesh.transform은 수정하지 않음
		glm::mat4 world = parentWorld * node->mesh->transform;

		// 기존 Mesh::draw가 내부에서 부모 * mesh.transform을 계산함
		node->mesh->draw(parentWorld);

		for (Node* child : node->child) {
			drawNode(child, world);
		}
	}

	bool collidesNode(Node* node, glm::mat4& parentWorld, OBB& obstacle) {
		if (!node)
			return false;

		glm::mat4 world = parentWorld;

		if (node->mesh) {
			// drawNode()와 같은 변환 순서
			world = parentWorld * node->mesh->transform;

			if (!node->mesh->vertices.empty()) {
				OBB bodyPart = makeOBB(*node->mesh, world);

				if (intersects(bodyPart, obstacle))
					return true;
			}
		}

		for (Node* child : node->child) {
			if (collidesNode(child, world, obstacle))
				return true;
		}

		return false;
	}

	bool collidesWith(OBB& obstacle) {
		glm::mat4 robotWorld =
			transform * glm::translate(glm::mat4(1.f), -origin);

		return collidesNode(root, robotWorld, obstacle);
	}

	void jump() {
		if (onGround) {
			verticalSpeed = 7.f;
			onGround = false;
		}
	}
};

class Truck : public Model {
public:
	// Rear-left inside corner of the cargo floor.
	glm::vec3 cargoAnchor{-1.08f, 1.46f, -2.98f};
	// Usable loading width (X) and length (Z).
	glm::vec2 cargoSize{2.16f, 4.16f};

	Truck() : Model(std::vector<Mesh*>{}) {}

	glm::mat4 getCargoTransform() const {
		return transform
			* glm::translate(glm::mat4(1.f), -origin)
			* glm::translate(glm::mat4(1.f), cargoAnchor);
	}

};

