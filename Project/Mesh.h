#pragma once
#include<iostream>
#include<memory>
#include<vector>
#include"OpenGLInit.h"
#include"Vertex.h"
#include"Texture.h"
#include"Light.h"
class Mesh {
public:
	std::vector<Vertex> vertices;
	std::vector<Arr3> indices;
	std::vector<Texture> textures;
	glm::mat4 transform{1.f};

	Mesh* parentMesh = nullptr;

	glm::vec3 origin{ 0 };
	glm::vec3 color{ 1.f };
	float opacity = 1.f;

	void setupMesh();
	void updateBuffer();

	glm::mat4 getWorldTransform(const glm::mat4& baseTransform = glm::mat4(1.f));

	virtual void draw(glm::mat4 mat = glm::mat4(1.f));
	Mesh(std::vector<Vertex> vertices, std::vector<Arr3> indices, std::vector<Texture> textures) 
		: vertices(vertices), indices(indices), textures(textures) {
		setupMesh();
	}
	virtual ~Mesh();
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	void move(const glm::vec3 offset) {
		transform = glm::translate(transform,offset);
	}

	void rotate(float degrees, glm::vec3 axis = { 0,1.f,0 }) {
		transform = glm::rotate(transform, glm::radians(degrees), axis);
	}

	void orbit(float degrees, glm::vec3 axis = { 0,1.f,0 }) {
		transform = glm::rotate(glm::mat4(1.f), glm::radians(degrees), axis) * transform;
	}

	void scale(glm::vec3 factors) {
		transform = glm::scale(transform, factors);
	}

	void scale(float rate) {
		transform = glm::scale(transform, glm::vec3(rate,rate,rate));
	}

public:
	//  render data
	GLuint VAO, VBO, EBO;
};

using MeshOwner = std::unique_ptr<Mesh>;

class PointLightMesh : public Mesh {
public:
	Light light;

	PointLightMesh(const Mesh& source, const glm::vec3& lightColor,float intensity = 1.f)
		: Mesh(source.vertices, source.indices, source.textures),light(glm::vec3(0.f), lightColor, intensity) {
		color = lightColor;
	}

	void syncLightPosition() {
		light.position = glm::vec3(getWorldTransform()[3]);
	}
};
