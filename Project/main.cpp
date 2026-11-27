#include<iostream>
#include"OpenGLInit.h"
#include"OpenGLShaderInit.h"
#include"Texture.h"
#include"Mesh.h"
#include"global.h"
#include"Random.h"
#include"TextureLoader.h"
#include"Light.h"
#include"LowPolyTank.h"

void resetGame() {

}

void callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_ESCAPE) {
			glfwSetWindowShouldClose(window, true);
		}
	}
}

void inputProcess(float deltaTime, LowPolyTank& tank) {
	tank.moveDirection = {
		static_cast<float>(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - static_cast<float>(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS),
		0.f,
		static_cast<float>(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) - static_cast<float>(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	};
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

	LowPolyTank* tank = new LowPolyTank(pg);
	tank->syncTopLightPosition();
	models.push_back(tank);
	lightManger.add(tank->getTopLight());
	

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
		inputProcess(deltaTime, *tank);
		update(deltaTime);
		tank->update(deltaTime);
		UpdateBuffer();
		drawScene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	lightManger.remove(tank->getTopLight());
	models.clear();
	delete tank;
	meshes.clear();
	glfwDestroyWindow(window);
	glfwTerminate();
}
