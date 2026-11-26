#include<iostream>
#include"OpenGLInit.h"
#include"OpenGLShaderInit.h"
#include"Texture.h"
#include"Mesh.h"
#include"global.h"
#include"Random.h"
#include"TextureLoader.h"
#include"Light.h"

void resetGame() {

}

void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

}

void inputProcess(float deltaTime) {

}

void update(float deltaTime) {

}

int main() {
	if (OPENGLinit() != 0) {
		glfwTerminate();
		return 1;
	}

	texture = textureLoader.loadImg("assets/textures/palettes/gradient.png");
	make_vertexShaders();
	make_fragmentShaders();
	shaderProgramID = make_shaderProgram();

	float mapSize = 10.f;
	Mesh* plane = pg.loadMeshlFromObj("assets/models/primitives/plane.obj");
	plane->scale(mapSize);
	plane->color = { 0.776f, 0.710f, 0.541f };
	meshes.push_back(plane);

	Mesh* lightMarker = pg.loadMeshlFromObj("assets/models/primitives/tetrahedron.obj");
	lightMarker->opacity = 0.1f;
	PointLightMesh* lightPoint = new PointLightMesh(*lightMarker, { 1.f, 1.f, 1.f });
	lightPoint->move({ 0.f, 100.f, 0.f });
	lightPoint->scale(.2f);
	lightPoint->syncLightPosition();
	lightPoint->light.intensity = 1.f;

	meshes.push_back(lightPoint);
	lightManger.add(lightPoint->light);

	InitBuffer();
	glfwSetWindowUserPointer(window, curCamera);
	glfwSetKeyCallback(window, callback);

	float wallHeight = 1.f;
	float cameraMargin = 0.3f;
	glm::vec3 cameraOffset(0.f, 8.f, 6.f);
	glm::vec3 viewDirection = glm::normalize(cameraOffset);
	curCamera->cameraTarget = { 0.f, wallHeight * 0.5f, 0.f };
	curCamera->cameraPosition = curCamera->cameraTarget + cameraOffset;
	curCamera->perspectiveMode = false;
	curCamera->orthoHalfHeight =
		(mapSize * viewDirection.y + wallHeight * viewDirection.z) * 0.5f + cameraMargin;
	curCamera->orthoMinHalfWidth = mapSize * 0.5f + cameraMargin;

	float curTime = glfwGetTime();
	while (!glfwWindowShouldClose(window)) {
		float deltaTime = glfwGetTime() - curTime;
		curTime = glfwGetTime();
		inputProcess(deltaTime);
		update(deltaTime);
		UpdateBuffer();
		drawScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	lightManger.remove(lightPoint->light);
	meshes.clear();
	glfwDestroyWindow(window);
	glfwTerminate();
}
