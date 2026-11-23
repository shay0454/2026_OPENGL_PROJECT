#pragma once
#include "OpenGLInit.h"
#include "Shape.h"
#include"Vertex.h"
#include<vector>
#include<array>
class Shape;


float floatToPixel(float pixel, float halfScreen);
glm::vec3 floatToPixel(glm::vec3 v);

class Shape {
public:
	GLenum mode;
	glm::vec3 origin;
	std::vector<Vertex> verticesByOrigin;
	std::vector<UV> texture;
	std::vector<arr3> indices;
	std::vector<Normal> normals;
	GLint vertexOffset = 0;
	GLint indexOffset = 0;
	GLfloat scale = 1.f;

	virtual void update(double deltaTime) {}
	void addCollider(class CollisionComponent* com);
	void removeCollider(class CollisionComponent* com);

	Shape(glm::vec3 origin, std::vector<Vertex> v, std::vector<arr3> indice) 
		:mode(GL_TRIANGLES), origin(origin), verticesByOrigin(v), indices(indice){}
};